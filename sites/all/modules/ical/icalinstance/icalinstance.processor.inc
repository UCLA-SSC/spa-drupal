<?php

/**
 * Processor for generating relevant instance nodes.
 *
 * There are so many quasi-separate parts that share derived data that it's just
 * easier to make this an object than to make it a series of functions.
 */
class ICalInstanceProcessor {

  /**
   * Settings for the processor derived from the configuration of the field.
   *
   * @var array
   */
  protected $settings = array();

  /**
   * The event node we are processing.
   *
   * @var stdClass
   */
  protected $eventNode;

  /**
   * CCK field definition for the ical field that we will be processing.
   *
   * @var array
   */
  protected $icalField;

  /**
   * CCK field definition for the iCalReference field we will be using on instance nodes.
   *
   * @var array
   */
  protected $referenceFieldInfo;

  /**
   * Database storage definition for the reference field.
   *
   * @var array
   */
  protected $referenceFieldDb;

  /**
   * CCK field definition for the Date field we will be using on instance nodes.
   *
   * @var array
   */
  protected $dateFieldInfo;

  /**
   * Database storage definition for the date field.
   *
   * @var array
   */
  protected $dateFieldDb;

  /**
   * The timezone that the date field we're using wants its data in.
   *
   * @var DateTimeZone
   */
  protected $dateZoneDb;

  /**
   * The instance events that should trigger a hook and rule event.
   *
   * @var array
   */
  protected $instanceEvents = array(
    'create' => TRUE,
    'update' => TRUE,
    'prune'  => TRUE,
    'save'   => TRUE,
  );

  /**
   * Constructs a new processor object.
   *
   * @param $node
   *   The node with an ical field on it that we will be processing.
   */
  public function __construct($node) {
    $this->eventNode = $node;
    $this->settings = icalinstance_get_settings($this->eventNode->type);

    // Yes, the following will break if there is more than one ical field on the
    // node.  I don't know why anyone would want to do that, frankly. :-) Arguably
    // it should be made a configuration option, but meh.
    foreach (icalfield_get_field_instances('ical') as $name => $field) {
      if (isset($this->eventNode->$name)) {
        $this->icalField = $field;
        break;
      }
    }

    // Derive the field information that we are going to need repeatedly for
    // all of this processing.
    $this->referenceFieldInfo = content_fields($this->settings['icalinstance_reference_field']);
    $this->referenceFieldDb = content_database_info($this->referenceFieldInfo);

    $this->dateFieldInfo = content_fields($this->settings['icalinstance_date_field']);
    $this->dateFieldDb = content_database_info($this->dateFieldInfo);

    // We'll need to mutate dates to the DB format, so just go ahead and
    // derive that.
    // @todo There are cases where this timezone is empty. Need to fix this.
    $this->dateZoneDb = new DateTimeZone($this->dateFieldInfo['timezone_db']);
  }

  /**
   * Sets whether or not to trigger a hook and rules event for certain actions.
   *
   * This option exists primarily to work around certain Rules configurations
   * which may otherwise result in an action firing for a bulk update of instances
   * and individually for each instance as it is saved.
   *
   * @param string $type
   *   The type of instance node event.  Possible values are create, update,
   *   prune (delete), and save (which happens for both created and updated).
   * @param boolean $status
   *   TRUE to fire the specified event, FALSE otherwise.
   * @return ICalInstanceProcessor
   *   The called object.
   */
  public function setInstanceEvent($type, $status = TRUE) {
    $this->instanceEvents[$type] = $status;

    return $this;
  }

  /**
   * Generates the instance nodes related to the specified event node.
   */
  public function execute() {

    $existing_nids = array();
    $created_nids = array();
    $delete_nids = array();
    $updated_nids = array();
    $future_instances = FALSE;

    $ical_field_name = $this->icalField['field_name'];

    $ical_field_data = icalfield_filter_empty($this->eventNode->$ical_field_name);
    foreach ($ical_field_data as $ical_field_instance) {
      $event = DrupalVEvent::create($ical_field_instance['value']);

      // This assumes the start and end dates are reasonably close to each other,
      // say on the same day, and are in the same timezone, and DST didn't change
      // between them. If that's not the case, what the heck are you doing?
      // An event with no end date is treated as an all-day event, which for
      // Date module means the start and end date/time are exactly the same.
      $end = $event->getEnd();
      $delta = $end ? $end->format('U') - $event->getStart()->format('U') : 0;

      // Find the most recent existing instance of this event to start counting
      // from. If there isn't one this returns NULL, which makes the iterator
      // start from the event's start time, which is what we want.
      $start = $this->findMostRecentInstance($ical_field_instance);

      // The status of the instance. This is an ical spec value on the status
      // of the event. For more information on the possible options see
      // @link http://www.kanzaki.com/docs/ical/status.html
      //
      // Since this is handled at the vevent level, all the instances generated
      // for the vevent will have the same status.
      $status = $event->getProperty('STATUS');

      foreach ($event->getIteratorCount($this->settings['icalinstance_count'], $start) as $i => $date) {
        // We assume that the end of each instance will be the same number of
        // seconds after the start of the instance.
        $end = clone($date);
        $end->modify("+ $delta seconds");

        // Create or update a node corresponding to this datetime and ical_id.
        if ($instance_node = $this->getExistingInstance($ical_field_instance, $date)) {
          // Add the existing node to our running list of nodes.
          $existing_nids[] = $instance_node->nid;

          // Set the details for the node. Since the status can change at any
          // time we need to update it here.
          $node_updated = $this->setInstanceDetails($instance_node, $ical_field_instance, $date, $end, $status);

          // Save the changes, if any.
          if ($node_updated) {
            node_save($instance_node);
            $updated_nids[] = $instance_node->nid;
          }
        }
        else {

          // Create a new instance node.
          $instance_node = $this->createInstance($ical_field_instance, $date, $end, $status);

          // Save the new node.
          node_save($instance_node);

          // Add the new node to the running list of nodes.
          $created_nids[] = $instance_node->nid;
        }
      }

      $num_instances = $this->getExistingInstanceCount($ical_field_instance);
      if ($event->hasFutureInstances(NULL, $num_instances)) {
        $future_instances = TRUE;
      }

      // Remove orphaned instance nodes.
      $allowed_nids = array_merge($created_nids, $existing_nids);
      $delete_nids = array_merge($delete_nids, $this->pruneOrphanedInstances($ical_field_instance, $allowed_nids));
    }

    // Prune orphaned instances from rules that no longer exist.
    $delete_nids = array_merge($delete_nids, $this->pruneOrphanedRules());

    $this->pruneInstances($delete_nids);

    // Let other modules do stuff with the nodes we just created or modified.

    if ($this->instanceEvents['create']) {
      module_invoke_all('icalinstance_create', $created_nids, $this->eventNode);
      if (module_exists('rules')) {
        rules_invoke_event('icalinstance_create', $created_nids, $this->eventNode);
      }
    }

    if ($this->instanceEvents['update']) {
      module_invoke_all('icalinstance_update', $updated_nids, $this->eventNode);
      if (module_exists('rules')) {
        rules_invoke_event('icalinstance_update', $updated_nids, $this->eventNode);
      }
    }

    if ($this->instanceEvents['save']) {
      $saved_nids = array_merge($created_nids, $updated_nids);
      module_invoke_all('icalinstance_save', $saved_nids, $this->eventNode);
      if (module_exists('rules')) {
        rules_invoke_event('icalinstance_save', $saved_nids, $this->eventNode);
      }
    }

    return $future_instances;
  }

  /**
   * Delete instance nodes whose rule no longer exists.
   *
   * If a rule was deleted then its instances won't be pruned by the rule-specific
   * pruning logic in pruneOrphanedInstances().  Instead, we scan for any
   * instance that points to this node but a non-existent rule.  Assuming that
   * means the rule was deleted, we delete the nodes, too.
   *
   * @param unknown_type $allowed_nids
   */
  protected function pruneOrphanedRules() {
    $now = new DateTime(NULL, $this->dateZoneDb);
    $rule_ids = array();

    $ical_field_name = $this->icalField['field_name'];
    $ical_field_data = icalfield_filter_empty($this->eventNode->$ical_field_name);
    foreach ($ical_field_data as $ical_field_instance) {
      $rule_ids[] = $ical_field_instance['ical_id'];
    }

    // If there are no values to process we can return now.
    if (empty($rule_ids)) {
      return array();
    }

    $placeholders = db_placeholders($rule_ids, 'text');

    // If there are no $rule_ids, this will still work fine because empty
    // arrays merge cleanly.
    $values = array_merge(array($this->eventNode->nid, $this->icalField['field_name'], $now->format(DATE_DATETIME_FORMAT)), $rule_ids);

    $query = "SELECT rf.nid
      FROM {{$this->referenceFieldDb['table']}} rf
        INNER JOIN {{$this->dateFieldDb['table']}} df ON (rf.nid = df.nid AND rf.vid = df.vid)
      WHERE rf.{$this->referenceFieldDb['columns']['nid']['column']} = %d
        AND rf.{$this->referenceFieldDb['columns']['field']['column']} = '%s'
        AND df.{$this->dateFieldDb['columns']['value']['column']} > '%s'
        AND rf.{$this->referenceFieldDb['columns']['property_id']['column']} NOT IN ({$placeholders})";

    $result = db_query($query, $values);

    $nids = array();
    while ($record = db_fetch_object($result)) {
      $nids[] = $record->nid;
    }

    return $nids;
  }

  /**
   * Deletes instance nodes that may have been orphaned.
   *
   * An orphaned instance is an instance that is in the future but would not have
   * matched the most recent generation phase.  If such a node exists, it was
   * probably caused by an older version of the vevent.  If a vevent's datetime
   * or recurrence information changes, instances may get disassociated. Rather
   * than try to line them up and modify them in place, which is incredibly hard,
   * we simply delete any that don't line up anymore.
   *
   * @param $ical_field_instance
   *   The ical field instance against which we are searching.
   * @param $allowed_nids
   *   The nids that we have confirmed are valid on this recurrence, so don't
   *   delete those.
   */
  protected function pruneOrphanedInstances($ical_field_instance, $allowed_nids) {
    $now = new DateTime(NULL, $this->dateZoneDb);

    // If there are no $allowed_nids, this will still work fine because empty
    // arrays merge cleanly.
    $values = array_merge(array($this->eventNode->nid, $this->icalField['field_name'], $ical_field_instance['ical_id'], $now->format(DATE_DATETIME_FORMAT)), $allowed_nids);

    $query = "SELECT df.nid
      FROM {{$this->referenceFieldDb['table']}} rf
        INNER JOIN {{$this->dateFieldDb['table']}} df ON (rf.nid = df.nid AND rf.vid = df.vid)
      WHERE rf.{$this->referenceFieldDb['columns']['nid']['column']} = %d
        AND rf.{$this->referenceFieldDb['columns']['field']['column']} = '%s'
        AND rf.{$this->referenceFieldDb['columns']['property_id']['column']} = '%s'
        AND df.{$this->dateFieldDb['columns']['value']['column']} > '%s'";
    if ($allowed_nids) {
      $placeholders = db_placeholders($allowed_nids, 'int');
      $query .= " AND df.nid NOT IN ({$placeholders})";
    }

    $result = db_query($query, $values);

    $nids = array();
    while ($record = db_fetch_object($result)) {
      $nids[] = $record->nid;
    }

    return $nids;
  }

  /**
   * Deletes the specified instances and lets other modules act on it.
   *
   * If there are a lot of nodes this could take a while.
   * Perhaps it could be converted to a secondary queue, but that would
   * run into race conditions with the "most recent instance" logic. Blargh.
   *
   * @param array $nids
   *   The nids of the instances to delete.
   */
  protected function pruneInstances(array $nids) {
    if ($nids) {
      if ($this->instanceEvents['prune']) {
        module_invoke_all('icalinstance_prune', $nids, $this->eventNode);
        if (module_exists('rules')) {
          rules_invoke_event('icalinstance_prune', $nids, $this->eventNode);
        }
      }

      foreach ($nids as $nid) {
        node_delete($nid);
      }
    }
  }

  /**
   * Finds the most recent existing instance we have already generated.
   *
   * @param $ical_field_instance
   *   The ical field instance against which we are searching.
   * @return DateTime
   *   The datetime of the most recent instance in the database, or NULL if
   *   one was not found.
   */
  protected function findMostRecentInstance($ical_field_instance) {
    $now = new DateTime(NULL, $this->dateZoneDb);

    $most_recent = db_fetch_object(db_query_range("SELECT df.nid, df.{$this->dateFieldDb['columns']['value']['column']} as dt
      FROM {{$this->referenceFieldDb['table']}} rf
        INNER JOIN {{$this->dateFieldDb['table']}} df ON (rf.nid = df.nid AND rf.vid = df.vid)
      WHERE rf.{$this->referenceFieldDb['columns']['nid']['column']} = %d
        AND rf.{$this->referenceFieldDb['columns']['field']['column']} = '%s'
        AND rf.{$this->referenceFieldDb['columns']['property_id']['column']} = '%s'
        AND df.{$this->dateFieldDb['columns']['value']['column']} < '%s'
      ORDER BY df.{$this->dateFieldDb['columns']['value']['column']} DESC", $this->eventNode->nid, $this->icalField['field_name'], $ical_field_instance['ical_id'], $now->format(DATE_DATETIME_FORMAT), 0, 1));

    if ($most_recent) {
      $date = new DateTime($most_recent->dt, $this->dateZoneDb);

      return $date;
    }

    return NULL;
  }

  /**
   * Gets the number of existing instance nodes for the specified rule.
   *
   * @param $ical_field_instance
   *   The ical field instance against which we are searching.
   * @return int
   *   The number of instances that already exist for the specified rule.
   */
  protected function getExistingInstanceCount($ical_field_instance) {

    $num_instances = db_result(db_query("SELECT COUNT(rf.nid)
      FROM {{$this->referenceFieldDb['table']}} rf
      WHERE rf.{$this->referenceFieldDb['columns']['nid']['column']} = %d
        AND rf.{$this->referenceFieldDb['columns']['field']['column']} = '%s'
        AND rf.{$this->referenceFieldDb['columns']['property_id']['column']} = '%s' ", $this->eventNode->nid, $this->icalField['field_name'], $ical_field_instance['ical_id']));

    return $num_instances;
  }

  /**
   * Finds an existing instance node, if one exists.
   *
   * @param $ical_field_instance
   *   The ical field instance against which we are searching.
   * @param DateTime $date
   *    The timestamp against which to match.
   * @return stdClass
   *   The found node, or FALSE if there wasn't one.
   */
  protected function getExistingInstance($ical_field_instance, DateTime $date) {

    $date_db = clone($date);
    $date_db->setTimezone($this->dateZoneDb);

    // @todo This could probably be optimized for the case that both fields are
    // on the same table.
    $existing_nid = db_result(db_query("SELECT rf.nid
      FROM {{$this->referenceFieldDb['table']}} rf
        INNER JOIN {{$this->dateFieldDb['table']}} df ON (rf.nid = df.nid AND rf.vid = df.vid)
      WHERE rf.{$this->referenceFieldDb['columns']['nid']['column']} = %d
        AND rf.{$this->referenceFieldDb['columns']['field']['column']} = '%s'
        AND rf.{$this->referenceFieldDb['columns']['property_id']['column']} = '%s'
        AND df.{$this->dateFieldDb['columns']['value']['column']} = '%s' ", $this->eventNode->nid, $this->icalField['field_name'], $ical_field_instance['ical_id'], $date_db->format(DATE_DATETIME_FORMAT)));

    return is_numeric($existing_nid) ? node_load($existing_nid) : $existing_nid;
  }

  /**
   * Creates a new instance node pointing to the current node and specified ical field instance.
   *
   * @param $ical_field_instance
   *   The field instance to which to reference.
   * @param $start
   *   The start of the instance.
   * @param $end
   *   The end of the instance.
   * @param $status
   *   The status of the instance (empty, TENTATIVE, CANCELED, etc.)
   * @return stdClass
   *   A new, unsaved, but populated node object.
   */
  protected function createInstance($ical_field_instance, DateTime $start, DateTime $end, $status = '') {
    $node = (object)array(
      'title' => $this->eventNode->title,
      'type' => $this->settings['icalinstance_node_type'],
      'uid' => $this->eventNode->uid,
      'status' => $this->eventNode->status,
      'sticky' => $this->eventNode->sticky,
      'promote' => $this->eventNode->promote,
      'language' => $this->eventNode->language,
    );

    $this->setInstanceDetails(&$node, $ical_field_instance, $start, $end, $status);

    return $node;
  }

  /**
   * Set the details for an iCal Instance required for the instance.
   *
   * @param $node
   *   The node the instances details are being set on.
   * @param $ical_field_instance
   *   The field instance to which to reference.
   * @param DateTime $start
   *   The start of the instance.
   * @param DateTime $end
   *   The end of the instance.
   * @param $status
   *   The status of the instance (empty, TENTATIVE, CANCELED, etc.)
   */
  protected function setInstanceDetails(&$node, $ical_field_instance, DateTime $start, DateTime $end, $status = '') {
    $node_updated = FALSE;

    // Because of all the indirect variable naming we do below, these have to be
    // local variables rather than object properties.
    $instance_reference_field_name = $this->settings['icalinstance_reference_field'];
    $instance_date_field_name = $this->settings['icalinstance_date_field'];
    $instance_status_field_name = $this->settings['icalinstance_status_field'];

    $instance_reference_field = array(
      'nid' => $this->eventNode->nid,
      'field' => $this->icalField['field_name'],
      'property_id' => $ical_field_instance['ical_id'],
    );

    $node->$instance_reference_field_name = array($instance_reference_field);

    $start_db = clone($start);
    $start_db->setTimezone($this->dateZoneDb);
    $end_db = clone($end);
    $end_db->setTimezone($this->dateZoneDb);

    $instance_date_field = array(
      'value' => $start_db->format(DATE_DATETIME_FORMAT), // Starting date.
      'value2' => $end_db->format(DATE_DATETIME_FORMAT), // Ending date.
      'timezone' => $start->format('e'), // America/Chicago (This is the zone the date should display in, because that's where it really is.)
      'timezone_db' => $this->dateFieldInfo['timezone_db'], // America/Chicago (This is the zone stored in the DB.  value and value2 should be in this zone.)
      'date_type' => 'date',        // I don't actually know what this is...
    );

    $node->$instance_date_field_name = array($instance_date_field);

    // This field is optional, so skip it if it does not apply.
    if ($instance_status_field_name) {
      $existing_field = (!empty($node->$instance_status_field_name)) ? $node->$instance_status_field_name : NULL;
      if (empty($existing_field) || $existing_field[0]['value'] != $status) {
        $instance_status_field = array(
          'value' => $status,
        );
        $node_updated = TRUE;
      }

      $node->$instance_status_field_name = array($instance_status_field);
    }

    return $node_updated;
  }
}
