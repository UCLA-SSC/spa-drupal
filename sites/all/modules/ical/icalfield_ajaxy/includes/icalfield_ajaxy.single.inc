<?php
// $Id: icalfield_ajaxy.single.inc,v 1.1.2.1 2010/10/06 15:24:02 mfer Exp $

/**
 * @file
 * The handling code for single dates.
 */

/**
 * Menu callback; dispatch for updating single dates.
 *
 * @param string $op
 *   The operation. Examples are update, cancel, and delete.
 * @param string $id
 *   The id for the single date. This is the ical_id.
 */
function icalfield_ajaxy_single_dispatch($op, $field_name, $id) {
  icalfield_ajaxy_setup();
  $element = array(
    '#field_name' => $field_name,
    '#name' => $field_name,
    '#parents' => array($field_name),
    '#array_parents' => array($field_name),
    '#tree' => TRUE,
    '#post' => $_POST,
  );
  $element['#id'] = form_clean_id('edit-'. implode('-', $element['#parents']));
  ctools_include('ajax');
  switch ($op) {
    case 'update':
      $output = array();

      $fake_form_state = array('submitted' => FALSE);
      $parents = array($field_name, 'items', 'single');
      $data = icalfield_ajaxy_multiple_form_get_single_dates($element);
      $data = icalfield_ajaxy_get_existing('single', $element, $id);
      $single_form = drupal_render(form_builder('ical_ajaxy_multiple_single', icalfield_ajaxy_multiple_form_single_instance($element, $parents, $data), $fake_form_state));
      $output[] = ctools_ajax_command_replace('#'. $element['#id'] .'-single-wrapper', $single_form);
      ctools_ajax_render($output);
    case 'cancel':
      $output = array();
      $fake_form_state = array('submitted' => FALSE);
      $parents = array($field_name, 'data', 'single');
      $data = icalfield_ajaxy_multiple_form_get_cancel_data($element);
      foreach ($data as $key => $value) {
        // Flip the value for canceled.
        $value['cancel'] = ($value['cancel'] == 1) ? FALSE : TRUE;
        $data_element = drupal_render(form_builder('ical_ajaxy_multiple_data', icalfield_ajaxy_build_data_element($element['#id'] .'-data-single', $key, $value, $parents), $fake_form_state));
        $output[] = ctools_ajax_command_replace('#'. $element['#id'] .'-data-single-'. $key, $data_element);
      }
      ctools_ajax_render($output);
    default:
      // We have an error.
      // @todo Report errors to screen or watchdog?
      die();
  }
}

/**
 * Get the single date rules as the structured form data.
 *
 * @param array $element
 *   The form element this is attached to.
 * @param $values
 *   The CCK values to filter through to find the single dates.
 *
 * @return array
 *   An array of the single dates in the structured data for the single form.
 */
function _icalfield_ajaxy_get_singles(array $element, array $values = array()) {
  $singles = array();
  if (!empty($values)) {
    foreach ($values as $key => $value) {
      $vevent = _icalfield_ajaxy_build_vevent($value);
      $rule = $vevent->getProperty('rrule', 0, TRUE);
      if (empty($rule)) {
        $singles[] = array(
          'start' => _icalfield_ajaxy_vevent_to_start($element, $vevent),
          'end' => _icalfield_ajaxy_vevent_to_end($element, $vevent),
          'ical_id' => $value['ical_id'],
          'timezone' => (isset($vevent->dtstart['params']['TZID'])) ? $vevent->dtstart['params']['TZID'] : '',
          'name' => $value['name'],
          'cancel' => $vevent->isCanceled(),
          'all_day' => $vevent->isAllDay(),
        );
      }
    }
  }
  return $singles;
}

/**
 * Helper function; get the data for canceling a single date
 *
 * @param array $element
 *   The element we are getting getting date information for.
 * @param bool $dupes
 *   Duplicate checking for form compared to data based on ical id.
 */
function icalfield_ajaxy_multiple_form_get_cancel_data(array $element) {
  // The existing dates, if any, already on the form.
  if (!empty($element['#post'][$element['#field_name']]['data']['single'])) {
    foreach($element['#post'][$element['#field_name']]['data']['single'] as $key => $value) {
      $return[$key] = array(
        'start' => $value['start'],
        'end' => $value['end'],
        'ical_id' => $value['ical_id'],
        'name' => $value['name'],
        'timezone' => $value['timezone'],
        'cancel' => $value['cancel'],
      );
    }
  }

  return $return;
}

/**
 * Theme the individual dates.
 */
function theme_icalfield_ajaxy_multiple_form_editor_dates(array $element, array $data = array()) {
  $dates = array();
  $found = FALSE;
  if (!empty($data)) {

    // Setup for the event link. This in information we only need to generate
    // once. To do this we need the node type. Currently we only support the
    // node/[nid]/edit page for this feature. There needs to be a saved field
    // to the database to make sure there are no orphaned instance nodes.
    $lookup = FALSE;
    $item = menu_get_item();
    // The node edit page.
    if ($item['path'] == 'node/%/edit') {
      $node = menu_get_object();
      $lookup = new ICalFieldAjaxyLookup($node);
    }

    foreach ($data as $key => $value) {
      if (!empty($value['start'])) {
        $found = TRUE;
        $timezone = ($value['timezone']) ? $value['timezone'] : date_default_timezone_name(TRUE);
        $start = new DateTime(str_replace('-', '', $value['start']), new DateTimezone($timezone));
        $cancel_text = $value['cancel'] ? t('Canceled') : t('Cancel');
        $cancel_class = $value['cancel'] ? 'ical-ajaxy-canceled' : '';

        // Edit links can only show where there is an existing instance node.
        // If there is no existing instance node the vevent may not vet be saved
        // to the database and in that case we do not want to create orphaned
        // instance nodes.
        $edit_link = '';
        $link_nid = ($lookup) ? $lookup->getExistingInstance($value['ical_id'], $start) : FALSE;
        if ($link_nid) {
          $edit_link = l('Edit', 'node/'. $link_nid .'/edit', array('attributes' => array('class' => 'automodal ui-icon ui-icon-pencil', 'title' => t('Edit Instance'))));
        }

        $time_text = ($value['all_day']) ? t('All day') : date_format_date($start, 'custom', 'g:i a T');

        $dates[] = array(
          array(
            'data' => l(t('Remove'), 'icalfield/ajaxy/single/remove/'. $element['#field_name'] .'/'. $value['ical_id'], array('attributes' => array('class' => 'ical-ajaxy-single-remove ui-icon ui-icon-trash', 'ical_id' => $value['ical_id'], 'title' => t('Remove')))),
            'class' => $cancel_class,
          ),
          array(
            // @todo Use time formatting according to the configuration for this CCK field instance.
            'data' => l(date_format_date($start, 'custom', 'n/j/Y'), 'icalfield/ajaxy/single/update/'. $element['#field_name'] .'/'. $value['ical_id'], array('attributes' => array('class' => 'ical-ajaxy-single-update', 'ical_id' => $value['ical_id'], 'title' => t('Update')))),
            'class' => $cancel_class,
          ),
          array(
            // @todo Use time formatting according to the configuration for this CCK field instance.
            'data' => l($time_text, 'icalfield/ajaxy/single/update/'. $element['#field_name'] .'/'. $value['ical_id'], array('attributes' => array('class' => 'ical-ajaxy-single-update', 'ical_id' => $value['ical_id'], 'title' => t('Update')))),
            'class' => $cancel_class,
          ),
          array(
            'data' => $edit_link,
            'class' => $cancel_class,
          ),
          array(
            'data' => l($cancel_text, 'icalfield/ajaxy/single/cancel/'. $element['#field_name'] .'/'. $value['ical_id'], array('attributes' => array('class' => 'ical-ajaxy-single-cancel ui-icon ui-icon-cancel', 'ical_id' => $value['ical_id'], 'title' => t('Cancel')))),
            'class' => $cancel_class,
          ),
        );
      }
    }
  }
  $output = '<div id="'. $element['#id'] .'-editor-items-dates" class="icalfield-ajaxy-header"><div class="ical-ajaxy-editor-title clear-block"><span class="icalfield-ajaxy-rule-title"><span class="ui-icon ui-icon-triangle-1-e ui-icon-triangle-1-s icalfield-ajaxy-collapse-button"></span>'. t('Individual Date/Time') .'</span></div>';

  // Display message about the table being updated.
  if ($element['#post']['js'] == 1) {
    $output .= '<div class="icalfield-ajaxy-message">'. theme('status_messages') .'</div>';
  }
  $header = array(t('Remove'), t('Date'), t('Time'), t('Edit'), t('Cancel'));
  $output .= '<div class="icalfield-ajaxy-table">'. theme('table', $header, $dates) .'</div>';
  $output .= '</div>';
  return ($found) ? $output : '';
}
