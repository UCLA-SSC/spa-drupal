<?php
// $Id: icalfield_ajaxy.lookup.inc,v 1.1.2.1 2010/10/06 15:24:02 mfer Exp $

/**
 * @file
 * Provide a lookup for existing nodes.
 */
class ICalFieldAjaxyLookup {

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
   * Constructs a new lookup object.
   *
   * @param $node
   *   The node with an ical field on it that we will be looking up instances of.
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
    // all of this searching.
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
   * Finds an existing instance node, if one exists.
   *
   * @param $ical_id
   *   The ical id for the instance against which we are searching.
   * @param DateTime $date
   *    The timestamp against which to match.
   * @return int
   *   The found node id, or FALSE if there wasn't one.
   */
  public function getExistingInstance($ical_id, DateTime $date) {

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
        AND df.{$this->dateFieldDb['columns']['value']['column']} = '%s' ", $this->eventNode->nid, $this->icalField['field_name'], $ical_id, $date_db->format(DATE_DATETIME_FORMAT)));

    return $existing_nid;
  }
}
