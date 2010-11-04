<?php

/**
 * Implementation of hook_rules_event_info().
 */
function icalinstance_rules_event_info() {
  $info['icalinstance_create'] = array(
    'label' => t('New iCal instances have been created'),
    'module' => 'iCal Instance',
    'arguments' => array(
      'nids' => array(
        'label' => t('Nodes just created'),
        'type' => 'icalinstance_nids',
      ),
      'event' => array(
        'label' => t('Event node'),
        'type' => 'node',
      ),
    ),
  );

  $info['icalinstance_update'] = array(
    'label' => t('iCal instances have been updated'),
    'module' => 'iCal Instance',
    'arguments' => array(
      'nids' => array(
        'label' => t('Nodes just updated'),
        'type' => 'icalinstance_nids',
      ),
      'event' => array(
        'label' => t('Event node'),
        'type' => 'node',
      ),
    ),
  );

  $info['icalinstance_save'] = array(
    'label' => t('iCal instances have been saved (created or updated)'),
    'module' => 'iCal Instance',
    'arguments' => array(
      'nids' => array(
        'label' => t('Nodes just saved'),
        'type' => 'icalinstance_nids',
      ),
      'event' => array(
        'label' => t('Event node'),
        'type' => 'node',
      ),
    ),
  );

  $info['icalinstance_prune'] = array(
    'label' => t('Orphaned iCal instances are going to be deleted'),
    'module' => 'iCal Instance',
    'arguments' => array(
      'nids' => array(
        'label' => t('Nodes about to be deleted'),
        'type' => 'icalinstance_nids',
      ),
      'event' => array(
        'label' => t('Event node'),
        'type' => 'node',
      ),
    ),
  );

  return $info;
}

/**
 * Implementation of hook_rules_action_info().
 */
function icalinstance_rules_action_info() {
  $actions['icalinstance_delete_all_instances'] = array(
    'label' => t('Delete all instances of an event'),
    'arguments' => array(
      'node' => array(
        'type' => 'node',
        'label' => t('Node'),
      ),
    ),
    'module' => 'iCal Instance',
  );

  $actions['icalinstance_enqueue_event'] = array(
    'label' => t('Enqueue an event node for processing'),
    'arguments' => array(
      'node' => array(
        'type' => 'node',
        'label' => t('Node'),
      ),
    ),
    'module' => 'iCal Instance',
  );

  return $actions;
}

/**
 * Implementation of hook_rules_data_type_info().
 */
function icalinstance_rules_data_type_info() {
  return array(
    'icalinstance_nids' => array(
      'label' => t('nids'),
      'class' => 'rules_data_type_icalinstance_nids',
      'savable' => FALSE,
      'identifiable' => FALSE,
      'use_input_form' => FALSE,
      'module' => 'iCal Instance',
    ),
  );
}


/**
 * Defines the user data type.
 */
class rules_data_type_icalinstance_nids extends rules_data_type {

}


/* ===== Action: Delete future instances ===== */


/**
 * Form callback for the views_atom_rules_action_add_to_feed action.
 */
function icalinstance_delete_all_instances_form($settings, &$form) {
  // Default to future-only.
  $settings += array('future_only' => 1);

  $form['future_only'] = array(
    '#type' => 'select',
    '#title' => t('Delete which instances'),
    '#options' => array(
      '0' => t('Delete all instances'),
      '1' => t('Delete only future instances'),
    ),
    '#default_value' => $settings['future_only'],
    '#description' => t('Warning: Deleting all instances will delete instances in the past as well, destroying any record of them. Generally future instances is sufficient.'),
  );
}

/**
 * Form submission callback for the views_atom_rules_action_add_to_feed action.
 */
function icalinstance_delete_all_instances_submit(&$settings, $form, $form_state) {
  $settings['future_only'] = $form_state['values']['future_only'];
}

/**
 * Rule implementation callback for deleting all future instances of an event.
 *
 * This is primarily useful when deleting an event node so that we can purge
 * any future instances while still leaving a historical record.
 *
 * @param $node
 *   The node that triggered this action.
 * @param $settings
 *   Configuration specified by the user.
 */
function icalinstance_delete_all_instances($node, $settings) {

  $ical_settings = icalinstance_get_settings($node->type);
  if ($settings && $ical_settings['icalinstance_enabled']) {
    // Since we have an "I'm about to delete lots of instances" event, fire that,
    // too, and let whatever is configured for that run as well.
    $nids = icalinstance_get_instance_nids($node);
    module_invoke_all('icalinstance_prune', $nids, $node);
    if (module_exists('rules')) {
      rules_invoke_event('icalinstance_prune', $nids, $node);
    }
    foreach ($nids as $nid) {
      node_delete($nid);
    }
  }
}


/* ===== Action: Enqueue an event for processing ===== */

/**
 * Rule implementation callback for queueing up an event node for processing.
 *
 * This action will also run processing on the event node once.  It if detects
 * that no further processing is necessary it will not be enqueued.
 *
 * @param $node
 *   The node that triggered this action.
 * @param $settings
 *   Configuration specified by the user.
 */
function icalinstance_enqueue_event($node, $settings) {

  // Note: This does presume there is only one ical field on any given node.
  // Why would you ever put more than one on?
  $ical_settings = icalinstance_get_settings($node->type);
  if ($ical_settings && $ical_settings['icalinstance_enabled']) {
    //dpm($node->nid, "Processing event node and possibly queuing for later");
    // Process the event data to generate instance nodes.
    $processor = new ICalInstanceProcessor($node);
    $needs_additional = $processor->execute();
    if ($needs_additional) {
      icalinstance_save($node->nid);
    }
  }
}

