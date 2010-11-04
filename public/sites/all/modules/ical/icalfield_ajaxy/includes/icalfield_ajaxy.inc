<?php
// $Id: icalfield_ajaxy.inc,v 1.1.2.1 2010/10/06 15:24:02 mfer Exp $

// Include all the parts to make this work.
include 'icalfield_ajaxy.utility.inc';
include 'icalfield_ajaxy.single.inc';
include 'icalfield_ajaxy.rule.inc';

/**
 * @file
 * Logic for the iCal Ajaxy functionality.
 *
 * @todo Document this.
 */

// @todo standardize on multiple or rule for nameing. (leaning rule)

/**
 * Process function fo the ical_ajaxy Form API widget.
 * This intercepts node-form submissions that were submitted by ctools AJAX.
 *
 * @see icalfield_elements()
 */
function icalfield_ajaxy_process($element, $edit, $form_state, $form) {
  // When the form data is passed in on submit/preview and there is an error
  // the form is NOT rebuilt. This poses a problem for us since we are
  // transforming between a ical value and a form to fill it out. So, we
  // clean up the submitted data so it can be properly displayed on the
  // first pass of the form in the event form_set_error is called.
  if ($form_state['submitted'] == 1) {
    $element = _icalfield_ajaxy_clean_post_data($element, $form_state);
  }

  $field_name = $element['#field_name'];
  $field = content_fields($field_name);
  drupal_add_js('misc/jquery.form.js');
  jquery_ui_add('ui.core');
  jquery_ui_add('ui.datepicker');
  jquery_ui_add('ui.slider');
  jquery_ui_add('ui.tabs');

  icalfield_ajaxy_setup();
  drupal_add_css(drupal_get_path('module', 'jquery_ui') .'/jquery.ui/themes/base/ui.all.css');
  drupal_add_js(libraries_get_path('timepicker') .'/js/timepicker.js');
  drupal_add_js(drupal_get_path('module', 'icalfield_ajaxy') .'/icalfield_ajaxy.js');
  drupal_add_css(drupal_get_path('module', 'icalfield_ajaxy') .'/icalfield_ajaxy.css');
  ctools_add_js('ajax-responder');

  // If the settings is added more than once it will affect the JavaScript. Drupal does not check
  // for settings being added more than once. This function can be run more than once in a page
  // and we only want this portion to run once per field.
  static $are_we_there_yet = array();
  if (!isset($are_we_there_yet[$field_name])) {
    drupal_add_js(array(
      'icalAjaxy' => array(
        $field_name => array(
          'id' => $element['#id'],
          'name' => $field_name,
          'values' => $element['#value'],
          'input_format' => trim(icalfield_ajaxy_format_to_js(icalfield_ajaxy_get_time_format_date($element) .' '. icalfield_ajaxy_get_time_format_time($element))),
        ),
      ),
    ), 'setting');
    $are_we_there_yet[$field_name] = TRUE;
  }

  $element['#value'] = _icalfield_ajaxy_clean_values($element['#value']);
  $element['#collapsible'] = TRUE;
  $element['#collapsed'] = FALSE;

  // This message is displayed and removed by JavaScript. JS is a dependency for
  // this wideget. Here we tell them that if it's missing or disabled.
  $element['items'] = array(
    '#type' => 'item',
    '#id' => $element['#id'] .'-items',
    '#value' => '<div id="'. $element['#id'] .'-no-js">'. t('JavaScript is disabled. Please enable JavaScript to use this form.') .'</div>',
  );

  // The actual form.
  $element['items'] += icalfield_ajaxy_multiple_form($element);

  // ctools ajax calls back to the node form. If this is happening and the form
  // has not being initially built we hand off the handling to the callback to
  // respond to the ajax request.
  if ($_POST['js'] == 1) {
    icalfield_ajaxy_manage_ajax_callback($element, $form_state);
  }

  return $element;
}

/**
 * Helper funtion; migrate the posted data from displayed form to the
 * internal data structures.
 *
 * @param array $element
 *   The form element being processed
 *
 * @return array
 *   A restructured form.
 */
function _icalfield_ajaxy_clean_post_data($element, &$form_state) {
  $values = array();
  $dummy = array();
  $changed = FALSE;
  // The single dates
  if (isset($element['#value']['data']['single']) && is_array($element['#value']['data']['single']) && !empty($element['#value']['data']['single'])) {
    foreach ($element['#value']['data']['single'] as $key => $value) {
      $vevent = _icalfield_ajaxy_build_vevent($value);
      $values[] = array(
        'name' => empty($value['name']) ? t('Default') .'-'. mt_rand() : $value['name'],
        'ical_id' => empty($value['ical_id']) ? uuid_uuid() : $value['ical_id'],
        'value' => empty($value['start']) ? '' : $vevent->createComponent($dummy),
      );
    }
    $changed = TRUE;
  }
  // The multiple dates
  if (isset($element['#value']['data']['multiple']) && is_array($element['#value']['data']['multiple']) && !empty($element['#value']['data']['multiple'])) {
    foreach ($element['#value']['data']['multiple'] as $key => $value) {
      $vevent = _icalfield_ajaxy_build_vevent($value);

      // The ical_id could be stored on the form stuff.
      // First we need to unserialize the value if it's serialized.
      if ($unserialized = unserialize($value['value'])) {
        $value['value'] = $unserialized;
      }
      if (isset($value['value']['ical_id']) && !empty($value['value']['ical_id'])) {
        // We copy the for ical_id so a new one is not generated.
        $value['ical_id'] = $value['value']['ical_id'];
      }

      $values[] = array(
        'name' => empty($value['name']) ? t('Default') .'-'. mt_rand() : $value['name'],
        'ical_id' => empty($value['ical_id']) ? uuid_uuid() : $value['ical_id'],
        'value' => $vevent->createComponent($dummy),
      );
    }
    $changed = TRUE;
  }

  // Only when we actually made a change to the data to we want to make the swap.
  // There are cases where good information is passed in and does not need to be cleaned.
  if ($changed) {
    $element['#default_value'] = $values;
    $element['#value'] = $values;
    form_set_value($element, $values, $form_state);
  }

  return $element;
}

/**
 * Helper function. Given data generate a vevent object.
 *
 * @param array $data
 *   Data about the event such as start and end dates.
 *
 * @return vevent
 */
function _icalfield_ajaxy_build_vevent(array $data) {
  $vevent = new vevent_fixed();
  $vevent->setConfig('nl', "\n");
  $vevent = new DrupalVEvent($vevent);

  if (!is_array($data['value']) && $unserialized = unserialize($data['value'])) {
    $data['value'] = $unserialized;
  }

  // The data is coming from CCK storage. We need to parse the value.
  if (isset($data['value']) && !empty($data['value']) && !is_array($data['value'])) {
    $vevent = DrupalVEvent::create($data['value']);
  }
  // The data is coming from a form. We need to build the information.
  elseif (isset($data['start'])) {
    $data['timezone'] = ($data['timezone']) ? $data['timezone'] : date_default_timezone_name(TRUE);
    $timezone = new DateTimezone($data['timezone']);
    if (!empty($data['start'])) {
      $start = new DateTime(str_replace('-', '', $data['start']), $timezone);

      // All day events have no time so we need to send FALSE in as the time instead.
      if ($data['all_day']) {
        $vevent->setDtstart($start->format('Y'), $start->format('m'), $start->format('d'), FALSE, FALSE, FALSE, FALSE, array('VALUE' => 'DATE'));
      }
      else {
        $vevent->setDtstart($start->format('Y'), $start->format('m'), $start->format('d'), $start->format('H'), $start->format('i'), FALSE, FALSE, array('TZID' => $data['timezone']));
      }

      // Set the timestamp to now.
      $now = new DateTime('now', $timezone);
      $vevent->setDtstamp($now->format('Y'), $now->format('m'), $now->format('d'), $now->format('H'), $now->format('i'), FALSE, array('TZID' => $data['timezone']));
    }
    if (!empty($data['end'])) {
      $end = new DateTime(str_replace('-', '', $data['end']), $timezone);

      // All day events do not have a time.
      if ($data['all_day']) {
        $vevent->setDtend($end->format('Y'), $end->format('m'), $end->format('d'), FALSE, FALSE, FALSE, FALSE, array('VALUE' => 'DATE'));
      }
      else {
        $vevent->setDtend($end->format('Y'), $end->format('m'), $end->format('d'), $end->format('H'), $end->format('i'), FALSE, FALSE, array('TZID' => $data['timezone']));
      }
    }
    if (!empty($data['cancel'])) {
      $vevent->setStatus('CANCELLED');
    }
  }
  else {
    $vevent = _icalfield_ajaxy_build_rule_vevent($data['value']);
  }

  return $vevent;
}


// @todo Combine with _icalfield_ajaxy_build_vevent
function _icalfield_ajaxy_build_rule_vevent($data) {
  $vevent = new vevent_fixed();
  $vevent->setConfig('nl', "\n");
  $vevent = new DrupalVEvent($vevent);

  if (empty($data)) {
    return $vevent;
  }

  // The data is coming from CCK storage. We need to parse the value.
  if (!is_array($data)) {
    $vevent = DrupalVEvent::create($data);
  }
  // The data is coming from a form. We need to build the information.
  else {
    $now = new DateTime('now', new DateTimezone($data['timezone']));
    $vevent->setDtstamp($now->format('Y'), $now->format('m'), $now->format('d'), $now->format('H'), $now->format('i'), FALSE, array('TZID' => $data['timezone']));
    $start = new DateTime($data['start'], new DateTimezone($data['timezone']));
    if ($data['all_day']) {
      $vevent->setDtstart($start->format('Y'), $start->format('m'), $start->format('d'), FALSE, FALSE, FALSE, FALSE, array('VALUE' => 'DATE'));
    }
    else {
      $vevent->setDtstart($start->format('Y'), $start->format('m'), $start->format('d'), $start->format('H'), $start->format('i'), FALSE, $data['timezone']);
    }
    if (!empty($data['end'])) {
      $end = new DateTime($data['end'], new DateTimezone($data['timezone']));
      if ($data['all_day']) {
        $vevent->setDtend($end->format('Y'), $end->format('m'), $end->format('d'), FALSE, FALSE, FALSE, FALSE, array('VALUE' => 'DATE'));
      }
      else {
        $vevent->setDtend($end->format('Y'), $end->format('m'), $end->format('d'), $end->format('H'), $end->format('i'), FALSE, FALSE, array('TZID' => $data['timezone']));
      }
    }
    // Generate rule
    $options = array();

    // Figure out duration.
    if ($data['duration'] == 'number'  && !empty($data['duration_number_occur']) && is_numeric($data['duration_number_occur'])) {
      $options['COUNT'] = $data['duration_number_occur'];
    }
    elseif ($data['duration'] == 'date' && !empty($data['duration_end_by']) && $date_data = new DateTime($data['duration_end_by'], new DateTimezone($data['timezone']))) {
      $options['UNTIL'] = date_format_date($date_data, 'custom', 'Ymd') .'T'. date_format_date($date_data, 'custom', 'GisT');
    }

    switch($data['tabs_selection']) {
      case 'daily':
        $options['FREQ'] = 'DAILY';
        if ($data['tabs']['daily']['select'] == 'days' && !empty($data['tabs']['daily']['select_days']) && is_numeric($data['tabs']['daily']['select_days'])) {
          $options['INTERVAL'] = $data['tabs']['daily']['select_days'];
        }

        // If we are on every weekday (M-F) then we have to go weekly and use
        // a byday for each of the week days.
        elseif ($data['tabs']['daily']['select'] == 'daily') {
          $options['FREQ'] = 'WEEKLY';
          $options['WKST'] = 'SU';
          $week_days = array('MO', 'TU', 'WE', 'TH', 'FR');
          foreach ($week_days as $k => $v) {
            $options['BYDAY'][] = array('DAY' => $v);
          }
        }
        break;
      case 'weekly':
        $options['FREQ'] = 'WEEKLY';
        if (!empty($data['tabs']['weekly']['weeks']) && is_numeric($data['tabs']['weekly']['weeks'])) {
          $options['INTERVAL'] = $data['tabs']['weekly']['weeks'];
          $options['WKST'] = 'SU';
        }
        if (!empty($data['tabs']['weekly']['days'])) {
          $weekly_days_byday = array();
          foreach ($data['tabs']['weekly']['days'] as $key => $value) {
            $weekly_days_byday[] = array('DAY' => icalfield_ajaxy_byday($key));
          }
          $options['BYDAY'] = $weekly_days_byday;
        }
        break;
      case 'monthly':
        $options['FREQ'] = 'MONTHLY';
        if ($data['tabs']['monthly']['select'] == 'days') {
          if (!empty($data['tabs']['monthly']['select_day_text']) && is_numeric($data['tabs']['monthly']['select_day_text'])) {
            $options['BYMONTHDAY'] = $data['tabs']['monthly']['select_day_text'];
          }
          if (!empty($data['tabs']['monthly']['select_months']) && is_numeric($data['tabs']['monthly']['select_months'])) {
            $options['INTERVAL'] = $data['tabs']['monthly']['select_months'];
          }
        }
        elseif($data['tabs']['monthly']['select'] == 'daily') {
          $options['BYDAY'][] = array(
            0 => icalfield_ajaxy_name_to_number($data['tabs']['monthly']['select_week']),
            'DAY' => icalfield_ajaxy_byday($data['tabs']['monthly']['select_monthly_day']),
          );
          if (!empty($data['tabs']['monthly']['select_daily_months']) && is_numeric($data['tabs']['monthly']['select_daily_months'])) {
            $options['INTERVAL'] = $data['tabs']['monthly']['select_daily_months'];
          }
        }
        break;
      case 'yearly':
        $options['FREQ'] = 'YEARLY';
        if (!empty($data['tabs']['yearly']['year']) && is_numeric($data['tabs']['yearly']['year'])) {
          $options['INTERVAL'] = $data['tabs']['yearly']['year'];
        }
        if ($data['tabs']['yearly']['select'] == 'short') {
          $options['BYMONTH'] = icalfield_ajaxy_bymonth($data['tabs']['yearly']['select_month']);
          $options['BYMONTHDAY'] = (int)$data['tabs']['yearly']['select_day'];
        }
        elseif($data['tabs']['yearly']['select'] == 'long') {
          $options['BYMONTH'] = icalfield_ajaxy_bymonth($data['tabs']['yearly']['select_long_month']);
          $options['BYDAY'][] = icalfield_ajaxy_name_to_number($data['tabs']['yearly']['select_week']) . icalfield_ajaxy_byday($data['tabs']['yearly']['select_days']);
        }
        break;
    }

    // Set timezone.
    $params = array(
      'TZID' => $data['timezone'],
    );

    $vevent->setProperty('rrule', $options, $params);

    // The event is canceled.
    if (!empty($data['cancel'])) {
      $vevent->setStatus('CANCELLED');
    }

    // Attach exdates.
    if (!empty($data['exdates'])) {
      // @todo: Convert all of this to use the DrupalVEvent and not the vevent.

      foreach ($data['exdates'] as $key => $exdate) {
        // We need a consistent timezone.
        $dateTimezone = new DateTimeZone($exdate['params']['TZID']);

        // Loop through all the dates and add them.
        foreach ($exdate['value'] as $k => $v) {
          $value = $vevent->vEventDateToDateTime($v, $dateTimezone);
          $vevent->addExdate($value);
        }
      }
    }
  }

  return $vevent;
}

/**
 * Validate the element through Form API and convert the form information into something we can save.
 *
 * @param array $element
 * @param array $form_state
 */
function icalfield_ajaxy_validate($element, &$form_state) {
  // Drupal adds the form fields that were submitted alongside the ones we set the
  // values to be. They cause problems for CCK (and shouldn't be there anyway).
  // Here we filter them out.
  if (!empty($form_state['values'][$element['#field_name']])) {
    foreach ($form_state['values'][$element['#field_name']] as $key => $value) {
      if (!is_numeric($key)) {
        unset($form_state['values'][$element['#field_name']][$key]);
      }
    }
  }
}

/**
 * Create a multiple ical input field.
 *
 * @param array $element
 *   The value information for the element.
 * @return array
 *   A set of form elements for FAPI to render.
 */
function icalfield_ajaxy_multiple_form(array $element = array()) {
  $form = array();

  // The controls wrapper provides a wrapper around the add/update forms. This
  // is used in styling this part of the form independantly from the editor.
  $form['controls_prefix'] = array(
    '#value' => '<div class="ical-ajaxy-multiple-controls">',
  );

  // And the single date part of the form.
  $form['single'] = icalfield_ajaxy_multiple_form_single_instance($element);

  // The rule form.
  $form['multiple'] = icalfield_ajaxy_multiple_form_multiple_instance($element);

  // The end of the wrapper around the forms.
  $form['controls_suffix'] = array(
    '#value' => '</div>',
  );

  // The editor is the visual place to manage existing dates and rules.
  $form['editor'] = icalfield_ajaxy_multiple_form_editor($element);

  // Data is where all the information to be processed for preview/save
  // will be stored. This is the part that matters for saving stuff.
  $form['data'] = array();
  $value = ($element['#value']) ? $element['#value'] : array();
  $singles = _icalfield_ajaxy_get_singles($element, $value);
  $fake_form_state = array('submitted' => FALSE);
  $form['data']['single'] = array(
    '#value' => drupal_render(form_builder('ical_ajaxy_multiple_data', icalfield_ajaxy_build_data_elements($element['#id'] .'-data-single', $singles, array($element['#field_name'], 'data', 'single')), $fake_form_state)),
  );

  // Filter the rules from the
  $rules = _icalfield_ajaxy_get_rules($value);

  // Convert the ical vevent into structured data.
  array_walk($rules, '_icalfield_ajaxy_convert_rules_data');

  $form['data']['multiple'] = array(
    '#value' => drupal_render(form_builder('ical_ajaxy_multiple_data', icalfield_ajaxy_build_rrule_elements($element['#id'] .'-data-rules', $rules, array($element['#field_name'], 'data', 'multiple')), $fake_form_state)),
  );

  return $form;
}

/**
 * Check if a vevent has canceled set.
 *
 * @param vevent $vevent
 *   The vevent object.
 */
function _icalfield_ajaxy_vevent_to_cancel(vevent $vevent) {
  $status = FALSE;
  if ($vevent->getProperty('status') == 'CANCELLED') {
    $status = TRUE;
  }
  return $status;
}

function _icalfield_ajaxy_vevent_to_start(array $element, $vevent) {
  if ($start = $vevent->getProperty('dtstart', FALSE, TRUE)) {
    if (isset($start['params']['TZID']) && !empty($start['params']['TZID'])) {
      $timezone = $start['params']['TZID'];
    }
    else {
      $timezone = date_default_timezone_name(TRUE);
    }
    if ($vevent->isAllDay()) {
      $time_string = $start['value']['month'] .'/'. $start['value']['day'] .'/'. $start['value']['year'];
    }
    else {
      $time_string = $start['value']['month'] .'/'. $start['value']['day'] .'/'. $start['value']['year'] .' '. $start['value']['hour'] .':'. $start['value']['min'] .':'. $start['value']['sec'];
    }
    $start = new DateTime($time_string, new DateTimezone($timezone));

    // Using date_format_date instead for format_date because it handles the
    // i18n while allowing more of the available formats for date/time display.
    if ($vevent->isAllDay()) {
      return date_format_date($start, 'custom', icalfield_ajaxy_get_time_format_date($element));
    }
    else {
      return date_format_date($start, 'custom', icalfield_ajaxy_get_time_format_date($element) .' '.  icalfield_ajaxy_get_time_format_time($element));
    }
  }
  else {
    return '';
  }
}

function _icalfield_ajaxy_vevent_to_end(array $element, $vevent) {
  if ($end = $vevent->getProperty('dtend', FALSE, TRUE)) {
    if (isset($end['params']['TZID']) && !empty($end['params']['TZID'])) {
      $timezone = $end['params']['TZID'];
    }
    else {
      $timezone = date_default_timezone_name(TRUE);
    }
    if ($vevent->isAllDay()) {
       $time_string = $end['value']['month'] .'/'. $end['value']['day'] .'/'. $end['value']['year'];
    }
    else {
      $time_string = $end['value']['month'] .'/'. $end['value']['day'] .'/'. $end['value']['year'] .' '. $end['value']['hour'] .':'. $end['value']['min'] .':'. $end['value']['sec'];
    }
    $end = new DateTime($time_string, new DateTimezone($timezone));

    // Using date_format_date instead for format_date because it handles the
    // i18n while allowing more of the available formats for date/time display.
    if ($vevent->isAllDay()) {
      return date_format_date($end, 'custom', icalfield_ajaxy_get_time_format_date($element));
    }
    else {
      return date_format_date($end, 'custom', icalfield_ajaxy_get_time_format_date($element) .' '. icalfield_ajaxy_get_time_format_time($element));
    }
  }
  else {
    return '';
  }
}

/**
 * Generate the form array to add/update a single date on the rrule form.
 *
 * @param array $element
 *   The form element (ical ajaxy)
 * @param $parents
 *   Any parents for this part of the element. This is used internally for
 *   AJAX generated sections of forms.
 */
function icalfield_ajaxy_multiple_form_single_instance(array $element, array $parents = array(), array $data = array(), $add = FALSE) {
  $form = array(
    '#prefix' => '<div id="'. $element['#id']. '-single-wrapper" class="icalfield-ajaxy-form-wrapper">',
    '#suffix' => '</div>',
  );

  if ($add || empty($data)) {
    $form['title'] = array(
      '#value' => '<h3>'. t('Add Individual Date/Time') .'</h3>',
    );
  }
  else {
    $timezone = ($data['timezone']) ? $data['timezone'] : date_default_timezone_name(TRUE);
    $start = new DateTime($data['start'], new DateTimezone($timezone));
    $form['title'] = array(
      '#value' => '<h3>'. t('Update: ') . date_format_date($start, 'custom', 'g:i a T') .'</h3>',
    );
  }

  // Include any status messages if being presented via ajax.
  if ($element['#post']['js'] == 1) {
    $form['messages'] = array(
      '#value' => theme('status_messages'),
    );
  }

  // Generate a default start date/time.
  $field = content_fields($element['#field_name']);
  $widget = $field['widget'];
  $default_start = '';
  if (!empty($widget['default_time'])) {
    $default_start = new DateTime('now', new DateTimeZone(empty($data['timezone']) ? date_default_timezone_name(TRUE) : $data['timezone']));
    // Get the hours and minutes dealing with am/pm if present.
    $parts = explode(':', $widget['default_time'], 2);
    $parts2 = explode(' ', $parts[1], 2);
    if (strtolower($parts2[1]) == 'pm') {
      $parts[0] += 12;
    }
    if ($parts[0] == 24) {
      $parts[0] = 0;
    }
    $default_start->setTime($parts[0], $parts2[0]);
    $default_start = date_format_date($default_start, 'custom', icalfield_ajaxy_get_time_format_date(array()) .' '. icalfield_ajaxy_get_time_format_time(array()));
  }

  $form['start'] = array(
    '#type' => 'textfield',
    '#title' => 'Start',
    '#value' => empty($data['start']) ? $default_start : $data['start'],
    '#attributes' => array('class' => 'ical-ajaxy-datetime ical-ajaxy-start-date'),
    '#prefix' => '<div class="icalfield-ajaxy-datetime-wrapper clear-block">',
    '#suffix' => '<span class="ui-icon ui-icon-calendar icalfield-ajaxy-cal-icon"></span></div>',
  );
  $form['end'] = array(
    '#type' => 'textfield',
    '#title' => 'End',
    '#value' => empty($data['end']) ? '' : $data['end'],
    '#attributes' => array('class' => 'ical-ajaxy-datetime ical-ajaxy-end-date'),
    '#prefix' => '<div class="icalfield-ajaxy-datetime-wrapper clear-block">',
    '#suffix' => '<span class="ui-icon ui-icon-calendar icalfield-ajaxy-cal-icon"></span></div>',
  );
  $form['all_day'] = array(
    '#type' => 'checkbox',
    '#title' => t('All day event.'),
    '#value' => empty($data['all_day']) ? '' : $data['all_day'],
    '#attributes' => array('class' => 'icalfield-ajaxy-all-day'),
  );
  $form['ical_id'] = array(
    '#type' => 'hidden',
    '#value' => empty($data['ical_id']) ? '' : $data['ical_id'],
  );
  $form['name'] = array(
    '#type' => 'hidden',
    '#value' => empty($data['name']) ? t('default') .'-'. mt_rand() : $data['name'],
  );
  $form['timezone'] = array(
    '#type' => 'hidden',
    '#value' => empty($data['timezone']) ? date_default_timezone_name(TRUE) : $data['timezone'],
  );
  $form['cancel'] = array(
    '#type' => 'hidden',
    '#value' => empty($data['cancel']) ? '' : 1,
  );

  // Add buttons to the form, dependent on the state.
  if ($add || empty($data)) {
    $form['add'] = array(
      '#type' => 'button',
      '#value' => t('Add'),
      '#attributes' => array('class' => 'ctools-use-ajax'),
      // Giving the button a unique name provides a way to provide unique
      // actions for this button.
      '#name' => $element['#field_name'] .'[single][op][add]',
    );
  }
  else {
    $form['update'] = array(
      '#type' => 'button',
      '#value' => t('Update'),
      '#attributes' => array('class' => 'ctools-use-ajax'),
      // Giving the button a unique name provides a way to provide unique
      // actions for this button.
      '#name' => $element['#field_name'] .'[single][op][update]',
    );
    $form['cancel-button'] = array(
      '#type' => 'button',
      '#value' => t('Cancel'),
      '#attributes' => array('class' => 'ctools-use-ajax'),
      // Giving the button a unique name provides a way to provide unique
      // actions for this button.
      '#name' => $element['#field_name'] .'[single][op][cancel]',
    );
  }

  // If parents have been passed in add them to the elements of the form. This is
  // used internally when we generate sections of forms for AJAX responses and
  // the depth needs to be passed in.
  if (!empty($parents)) {
    foreach (element_children($form) as $key) {
      $form[$key]['#parents'] = $parents;
      $form[$key]['#parents'][] = $key;
    }
  }
  // For a button, with ctools, you can specify a different path callback
  // by adding a hidden element with a class that is the same as the id
  // for the button plus '-url' added to the end. The path is relative so we
  // need to add the base path.
  //  $form['add-url'] = array(
  //    '#type' => 'hidden',
  //    '#value' => base_path() .'icalfield/ajaxy/test/'. $element['#field_name'],
  //    '#name' => '',
  //    '#attributes' => array('class' => 'edit-field-ical-items-single-add-url'),
  //  );
  return $form;
}

function icalfield_ajaxy_multiple_form_multiple_instance(array $element = array(), $data = array(), $add = TRUE) {
  $form = array(
    '#prefix' => '<div id="'. $element['#id']. '-multiple-wrapper" class="icalfield-ajaxy-form-wrapper">',
    '#suffix' => '</div>',
  );

  if ($add || empty($data)) {
    $form['section_name'] = array(
      '#value' => '<h3>'. t('Add Recurring Date/Time') .'</h3>',
    );
  }
  else {
    $form['section_name'] = array(
      '#value' => '<h3>'. t('Update') .' '. check_plain($data['name']) .'</h3>',
    );
  }

  // Include any status messages if being presented via ajax.
  if ($element['#post']['js'] == 1) {
    $form['messages'] = array(
      '#value' => theme('status_messages'),
    );
  }

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Rule Name'),
    '#default_value' => isset($data['name']) ? $data['name'] : '',
  );

  // Generate a default start date/time.
  $field = content_fields($element['#field_name']);
  $widget = $field['widget'];
  $default_start = '';
  if (!empty($widget['default_time'])) {
    $default_start = new DateTime('now', new DateTimeZone(empty($data['timezone']) ? date_default_timezone_name(TRUE) : $data['timezone']));
    // Get the hours and minutes dealing with am/pm if present.
    $parts = explode(':', $widget['default_time'], 2);
    $parts2 = explode(' ', $parts[1], 2);
    if (strtolower($parts2[1]) == 'pm') {
      $parts[0] += 12;
    }
    if ($parts[0] == 24) {
      $parts[0] = 0;
    }
    $default_start->setTime($parts[0], $parts2[0]);
    $default_start = date_format_date($default_start, 'custom', icalfield_ajaxy_get_time_format_date(array()) .' '. icalfield_ajaxy_get_time_format_time(array()));
  }

  $form['start'] = array(
    '#type' => 'textfield',
    '#title' => 'Start',
    '#value' => empty($data['start']) ? $default_start : $data['start'],
    '#attributes' => array('class' => 'ical-ajaxy-datetime ical-ajaxy-start-date'),
    '#prefix' => '<div class="icalfield-ajaxy-datetime-wrapper clear-block">',
    '#suffix' => '<span class="ui-icon ui-icon-calendar icalfield-ajaxy-cal-icon"></span></div>',
  );
  $form['end'] = array(
    '#type' => 'textfield',
    '#title' => 'End',
    '#value' => empty($data['end']) ? '' : $data['end'],
    '#attributes' => array('class' => 'ical-ajaxy-datetime ical-ajaxy-end-date'),
    '#prefix' => '<div class="icalfield-ajaxy-datetime-wrapper clear-block">',
    '#suffix' => '<span class="ui-icon ui-icon-calendar icalfield-ajaxy-cal-icon"></span></div>',
  );
  $form['all_day'] = array(
    '#type' => 'checkbox',
    '#title' => t('All day event.'),
    '#default_value' => isset($data['all_day']) ? $data['all_day'] : '',
    '#attributes' => array('class' => 'icalfield-ajaxy-all-day'),
  );

  // Adding form elements to the label of checkboxes or radios is not directly
  // by Form API. In order to add form elements there we need to create the
  // element, pass it though form_builder() to add in all the extras that are
  // automatically generated, pass that output through drupal_render to build
  // the element and put that output as the title.
  //
  // For proper nesting of element names and ids we need to set the #parents
  // property to be the value containing the nesting we are looking for.
  // Example:
  // $fake_element = array(
  //   '#type' => 'textfield',
  //   '#size' => 2,
  //   '#parents' => array(
  //     $element['#field_name'], 'foo', 'bar',
  //   ),
  // );
  // $fake_form_state = array('submitted' => FALSE);
  // $element = array(
  //   'foo' => array(
  //     '#options' => array(
  //       'one' => drupal_render(form_builder('some_name_for_form', $fake_element, $fake_form_state)),
  //       'two' => t('Test'),
  //     ),
  //     '#type' => 'radios',
  //   ),
  // );
  //
  // The $element radio would have a name of field_name[foo] and the textfield
  // on added to the one option would have a name of field_name[foo][bar]
  $fake_form_state = array('submitted' => FALSE);

  // Days, Weeks, Months, and Textfields are used more than once. Here we setup
  // defaults to be used later.
  $default_textfield = array(
    '#type' => 'textfield',
    '#size' => 2,
  );
  $default_weeks = array(
    '#type' => 'select',
    '#options' => array(
      'first' => t('First'),
      'second' => t('Second'),
      'third' => t('Third'),
      'fourth' => t('Fourth'),
      'fifth' => t('Fifth'),
    ),
  );
  $default_days = array(
    '#type' => 'select',
    '#options' => array(
      'sun' => t('Sunday'),
      'mon' => t('Monday'),
      'tue' => t('Tuesday'),
      'wed' => t('Wednesday'),
      'thu' => t('Thursday'),
      'fri' => t('Friday'),
      'sat' => t('Saturday'),
    ),
  );
  $default_months = array(
    '#type' => 'select',
    '#options' => array(
      'January' => t('January'),
      'February' => t('February'),
      'March' => t('March'),
      'April' => t('April'),
      'May' => t('May'),
      'June' => t('June'),
      'July' => t('July'),
      'August' => t('August'),
      'September' => t('September'),
      'October' => t('October'),
      'November' => t('November'),
      'December' => t('December'),
    ),
  );

  // Store the selected tab. A rule can only have one type.
  // When this section is sent to the form the JavaScript grabs the value from
  // here and makes sure the proper tab is selected.
  $form['tabs_selection'] = array(
    '#type' => 'hidden',
    '#value' => isset($data['tabs_selection']) ? $data['tabs_selection'] : strtolower(t('Daily')),
    '#attributes' => array('class' => 'icalfield-ajaxy-selected-tab-value'),
  );

  // The individual tabs for ui.tabs to generate the tabs from. Did not use
  // theme_item_list because of extra markup added by the theme function.
  $form['tabs'] = array(
    '#prefix' => '<div id="' . $element['#id'] .'-multiple-tabs" class="ical-ajaxy-tabs">',
    '#suffix' => '</div>',
    'tabs' => array(
      '#value' => '<ul class="ical-ajaxy-tab-container">
         <li><a href="#'. $element['#id'] .'-multiple-tabs-daily" class="ical-ajaxy-tab ical-ajaxy-tab">'. t('daily') .'</a></li>
         <li><a href="#'. $element['#id'] .'-multiple-tabs-weekly" class="ical-ajaxy-tab">'. t('weekly') .'</a></li>
         <li><a href="#'. $element['#id'] .'-multiple-tabs-monthly" class="ical-ajaxy-tab">'. t('monthly') .'</a></li>
         <li><a href="#'. $element['#id'] .'-multiple-tabs-yearly" class="ical-ajaxy-tab">'. t('yearly') .'</a></li>
      </ul>',
    ),
  );

  // The days tab does recurring dates based on days.
  $days = $default_textfield;
  $days['#prefix'] = '<div class="ical-ajaxy-clear-block"><div class="ical-ajaxy-float">'. t('Every') .'</div>';
  $days['#suffix'] = '<div class="ical-ajaxy-float">'. t('Day(s)') .'</div></div>';
  $days['#parents'] = array(
    $element['#field_name'], 'items', 'multiple', 'tabs', 'daily', 'select_days',
  );
  $days['#default_value'] = isset($data['tabs']['daily']['select_days']) ? $data['tabs']['daily']['select_days'] : '1';

  $form['tabs']['daily'] = array(
    '#prefix' => '<div id="'. $element['#id'] .'-multiple-tabs-daily" class="ical-ajaxy-tab-daily">',
    '#suffix' => '</div>',
    'select' => array(
      '#options' => array(
        'days' => drupal_render(form_builder('ical_ajaxy_multiple_daily_days', $days, $fake_form_state)),
        'daily' => t('Every Week Day'),
      ),
      '#type' => 'radios',
      '#default_value' => isset($data['tabs']['daily']['select']) ? $data['tabs']['daily']['select'] : 'days',
    ),
  );

  // Weekly is simple enough we don't need magic. Yay!
  $form['tabs']['weekly'] = array(
    '#prefix' => '<div id="'. $element['#id'] .'-multiple-tabs-weekly" class="ical-ajaxy-tab-weekly">',
    '#suffix' => '</div>',
    'weeks' => array(
      '#type' => 'textfield',
      '#prefix' => '<div class="ical-ajaxy-clear-block clear-block"><div class="ical-ajaxy-float">'. t('Recur every') .'</div>',
      '#suffix' => '<div class="ical-ajaxy-float">'. t('week(s) on:') .'</div></div>',
      '#size' => 2,
      '#default_value' => isset($data['tabs']['weekly']['weeks']) ? $data['tabs']['weekly']['weeks'] : '1',
    ),
    'days' => array(
      '#options' => $default_days['#options'],
      '#type' => 'checkboxes',
      '#default_value' => isset($data['tabs']['weekly']['days']) ? $data['tabs']['weekly']['days'] : array(),
    ),
  );

  // The complexity that is the Months tab.
  $day_text = $default_textfield;
  $day_text['#prefix'] = '<div class="ical-ajaxy-float">'. t('Day') .'</div>';
  $day_text['#parents'] = array(
    $element['#field_name'], 'items', 'multiple', 'tabs', 'monthly', 'select_day_text',
  );
  $today = new DateTime('now', new DateTimeZone(($data['timezone']) ? $data['timezone'] : date_default_timezone_name(TRUE)));
  $day_text['#default_value'] = isset($data['tabs']['monthly']['select_day_text']) ? $data['tabs']['monthly']['select_day_text'] : $today->format('j');
  $day_text = drupal_render(form_builder('ical_ajaxy_multiple_monthly_day_text', $day_text, $fake_form_state));

  $months = $default_textfield;
  $months['#prefix'] = '<span class="icalfield-ajaxy-positioning">'. ('of every') .'</span>';
  $months['#suffix'] = '<span class="icalfield-ajaxy-positioning">'. t('Month') .'</span>';
  $months['#parents'] = array(
    $element['#field_name'], 'items', 'multiple', 'tabs', 'monthly', 'select_months',
  );
  $months['#default_value'] = isset($data['tabs']['monthly']['select_months']) ? $data['tabs']['monthly']['select_months'] : '1';

  $daily_months = $months;
  $daily_months['#parents'] = array(
    $element['#field_name'], 'items', 'multiple', 'tabs', 'monthly', 'select_daily_months',
  );
  $daily_months['#default_value'] = isset($data['tabs']['monthly']['select_daily_months']) ? $data['tabs']['monthly']['select_daily_months'] : '';

  $months = drupal_render(form_builder('ical_ajaxy_multiple_monthly_months', $months, $fake_form_state));
  $daily_months = drupal_render(form_builder('ical_ajaxy_multiple_monthly_daily_months', $daily_months, $fake_form_state));
  $week = $default_weeks;
  $week['#prefix'] = '<div class="ical-ajaxy-float">'. t('The') .'</div>';
  $week['#parents'] = array(
    $element['#field_name'], 'items', 'multiple', 'tabs', 'monthly', 'select_week',
  );
  $week['#default_value'] = isset($data['tabs']['monthly']['select_week']) ? $data['tabs']['monthly']['select_week'] : '';

  $week = drupal_render(form_builder('ical_ajaxy_multiple_monthly_week', $week, $fake_form_state));
  $day_select = $default_days;
  $day_select['#parents'] = array(
    $element['#field_name'], 'items', 'multiple', 'tabs', 'monthly', 'select_monthly_day',
  );
  $day_select['#default_value'] = isset($data['tabs']['monthly']['select_monthly_day']) ? $data['tabs']['monthly']['select_monthly_day'] : '';

  $day_select = drupal_render(form_builder('ical_ajaxy_multiple_monthly_day_select', $day_select, $fake_form_state));
  $form['tabs']['monthly'] = array(
    '#prefix' => '<div id="'. $element['#id'] .'-multiple-tabs-monthly" class="ical-ajaxy-tab-monthly">',
    '#suffix' => '</div>',
    'select' => array(
      '#options' => array(
        'days' => '<div class="ical-ajaxy-clear-block">'. $day_text . $months .'</div>',
        'daily' => '<div class="ical-ajaxy-clear-block">'. $week . $day_select . $daily_months .'</div>',
      ),
      '#type' => 'radios',
      '#default_value' => isset($data['tabs']['monthly']['select']) ? $data['tabs']['monthly']['select'] : 'days',
    ),
  );

  // Recurring yearly stuff.
  $month = $default_months;
  $month['#parents'] = array(
    $element['#field_name'], 'items', 'multiple', 'tabs', 'yearly', 'select_month',
  );
  $month['#default_value'] = isset($data['tabs']['yearly']['select_month']) ? $data['tabs']['yearly']['select_month'] : $today->format('F');

  $long_month = $month;
  $long_month['#parents'] = array(
    $element['#field_name'], 'items', 'multiple', 'tabs', 'yearly', 'select_long_month',
  );
  $long_month['#default_value'] = isset($data['tabs']['yearly']['select_long_month']) ? $data['tabs']['yearly']['select_long_month'] : '';

  $month = drupal_render(form_builder('ical_ajaxy_multiple_yearly_month', $month, $fake_form_state));
  $long_month = drupal_render(form_builder('ical_ajaxy_multiple_yearly_month', $long_month, $fake_form_state));
  $week = $default_weeks;
  $week['#parents'] = array(
    $element['#field_name'], 'items', 'multiple', 'tabs', 'yearly', 'select_week',
  );
  $week['#default_value'] = isset($data['tabs']['yearly']['select_week']) ? $data['tabs']['yearly']['select_week'] : '';

  $week = drupal_render(form_builder('ical_ajaxy_multiple_yearly_week', $week, $fake_form_state));
  $day_select = $default_days;
  $day_select['#parents'] = array(
    $element['#field_name'], 'items', 'multiple', 'tabs', 'yearly', 'select_days',
  );
  $day_select['#default_value'] = isset($data['tabs']['yearly']['select_days']) ? $data['tabs']['yearly']['select_days'] : '';

  $day_select = drupal_render(form_builder('ical_ajaxy_multiple_yearly_day_select', $day_select, $fake_form_state));
  $day = $default_textfield;
  $day['#parents'] = array(
    $element['#field_name'], 'items', 'multiple', 'tabs', 'yearly', 'select_day',
  );
  $day['#default_value'] = isset($data['tabs']['yearly']['select_day']) ? $data['tabs']['yearly']['select_day'] : $today->format('j');

  $day = drupal_render(form_builder('ical_ajaxy_multiple_yearly_day', $day, $fake_form_state));
  $form['tabs']['yearly'] = array(
    '#prefix' => '<div id="'. $element['#id'] .'-multiple-tabs-yearly"  class="ical-ajaxy-tab-yearly">',
    '#suffix' => '</div>',
    'year' => array(
      '#type' => 'textfield',
      '#prefix' => '<div class="ical-ajaxy-clear-block"><div class="ical-ajaxy-float">'. t('Recur every') .'</div>',
      '#suffix' => '<div class="ical-ajaxy-float">'. t('Year(s)') .'</div></div>',
      '#size' => 2,
      '#default_value' => isset($data['tabs']['yearly']['year']) ? $data['tabs']['yearly']['year'] : '1',
    ),
    'select' => array(
      '#options' => array(
        'short' => '<div class="ical-ajaxy-clear-block"><div class="ical-ajaxy-float">'. t('On') .'</div>'. $month . $day .'</div>',
        'long' => '<div class="ical-ajaxy-clear-block"><div class="ical-ajaxy-float">'. t('The') .'</div>'. $week . $day_select .'<span class="icalfield-ajaxy-positioning">'. t('of') .'</span>'. $long_month .'</div>',
      ),
      '#type' => 'radios',
      '#default_value' => isset($data['tabs']['yearly']['select']) ? $data['tabs']['yearly']['select'] : 'short',
    ),
  );
  // The end of the tabs.

  $number_occur = array(
    '#prefix' => '<div class="ical-ajaxy-float">'. t('End After') .'</div>',
    '#suffix' => '<div class="ical-ajaxy-float">'. t('Occurrences') .'</div>',
    '#type' => 'textfield',
    '#parents' => array(
      $element['#field_name'], 'items', 'multiple', 'duration_number_occur',
    ),
    '#size' => 4,
    '#default_value' => isset($data['duration_number_occur']) ? $data['duration_number_occur'] : '1',
  );
  $end_by = array(
    '#prefix' => '<div class="ical-ajaxy-float">'. t('End by') .'</div>',
    '#suffix' => '<span class="ui-icon ui-icon-calendar icalfield-ajaxy-cal-icon-date"></span>',
    '#type' => 'textfield',
    '#attributes' => array('class' => 'ical-ajaxy-datepicker'),
    '#parents' => array(
      $element['#field_name'], 'items', 'multiple', 'duration_end_by',
    ),
    '#size' => 20,
    '#default_value' => isset($data['duration_end_by']) ? $data['duration_end_by'] : '',
  );
  $form['duration'] = array(
    '#options' => array(
      'number' => '<div class="ical-ajaxy-duration ical-ajaxy-clear-block">'. drupal_render(form_builder('ical_ajaxy_multiple_number_occur', $number_occur, $fake_form_state)) .'</div>',
      'date' => '<div class="ical-ajaxy-duration ical-ajaxy-duration-date ical-ajaxy-clear-block">'. drupal_render(form_builder('ical_ajaxy_multiple_end_by', $end_by, $fake_form_state)) .'</div>',
    ),
    '#type' => 'radios',
    '#prefix' => '<div class="ical-ajaxy-duration ical-ajaxy-clear-block">',
    '#suffix' => '</div>',
    '#default_value' => isset($data['duration']) ? $data['duration'] : 'number',
  );
  $form['timezone'] = array(
    '#type' => 'hidden',
    '#value' => isset($data['timezone']) ? $data['timezone'] : date_default_timezone_name(TRUE),
  );
  $form['cancel'] = array(
    '#type' => 'hidden',
    '#value' => isset($data['cancel']) ? $data['cancel'] : '',
  );

  // Exdates are not stored on the form intentionally. If a rule is updated the
  // exdates are wiped out and new ones need to be added. When a rule changes
  // it could happen in a manner where none of the exdates are relevant any
  // longer. So, upon refresh they are wiped out.

  // The ical_id is used for updates. We need to pass it around on updates
  // to know what to update.
  $form['ical_id'] = array(
    '#type' => 'hidden',
    '#value' =>isset($data['ical_id']) ? $data['ical_id'] : '',
  );

  // Add buttons to the form, dependent on the state.
  if ($add || empty($data)) {
    $form['add'] = array(
      '#type' => 'button',
      '#value' => t('Add'),
      '#attributes' => array('class' => 'ctools-use-ajax'),
      // Giving the button a unique name provides a way to provide unique
      // actions for this button.
      '#name' => $element['#field_name'] .'[multiple][op][add]',
    );
  }
  else {
    $form['update'] = array(
      '#type' => 'button',
      '#value' => t('Update'),
      '#attributes' => array('class' => 'ctools-use-ajax'),
      // Giving the button a unique name provides a way to provide unique
      // actions for this button.
      '#name' => $element['#field_name'] .'[multiple][op][update]',
    );
    $form['cancel-button'] = array(
      '#type' => 'button',
      '#value' => t('Cancel'),
      '#attributes' => array('class' => 'ctools-use-ajax'),
      // Giving the button a unique name provides a way to provide unique
      // actions for this button.
      '#name' => $element['#field_name'] .'[multiple][op][cancel]',
    );
  }




  return $form;
}

/**
 * Render the rrule editor.
 *
 * @param array $element
 *   The parent element.
 * @param array $values
 *   An array of elements within the editor
 * @return array
 *   An array for Form API to render
 */
function icalfield_ajaxy_multiple_form_editor(array $element, array $values = array()) {
  $form = array(
    '#type' => 'fieldset',
    '#title' => t('Editor'),
    '#prefix' => '<div class="ical-ajaxy-multiple-editor">',
    '#suffix' => '</div>',
  );
  $form['items'] = array(
    '#prefix' => '<div id="'. $element['#id'] .'-editor-items">',
    '#suffix' => '</div>',
    '#value' => '&nbsp;',
  );
  $values = ($element['#value']) ? $element['#value'] : array();

  // Add the rules to the editor.
  $rules = _icalfield_ajaxy_get_rules($values);
  if (!empty($rules)) {
    $form['items']['rule']['#value'] = '';
    foreach ($rules as $k => $v) {
      // Convert a vevent to a post data structure
      $value = _icalfield_ajaxy_field_to_data($v);
      $form['items']['rule']['#value'] .= theme('icalfield_ajaxy_multiple_form_editor_rrule', $element, $value) ."\n";
    }
  }

  // Add the singles after the rules.
  $singles = _icalfield_ajaxy_get_singles($element, $values);
  if (!empty($singles)) {
    $form['items']['singles'] = array(
      '#value' => theme('icalfield_ajaxy_multiple_form_editor_dates', $element, $singles),
    );
  }
  return $form;
}

/**
 * Implementation of hook_form_alter().
 *
 * Set the number of values to unlimited.
 */
function icalfield_ajaxy_form_content_field_edit_form_alter(&$form, $form_state) {
  if ($form['basic']['widget_type']['#default_value'] == 'ical_ajaxy' && isset($form['field']['multiple'])) {
    // The default value form does not work at the moment so we disable it.
    // @todo Fix the default value form.
    $form['widget']['default_value_fieldset']['#type'] = 'markup';
    $form['widget']['default_value']['#default_value'] = NULL;
    $children = element_children($form['widget']['default_value_fieldset']['default_value_widget']);
    foreach ($children as $k => $v) {
      $form['widget']['default_value_fieldset']['default_value_widget'][$v]['#type'] = 'value';
      $form['widget']['default_value_fieldset']['default_value_widget'][$v]['#default_value'] = NULL;
    }

    $form['field']['multiple']['#value'] = 1;
    $form['field']['multiple']['#default_value'] = 1;
    $form['field']['multiple']['#disabled'] = TRUE;
  }
}

/**
 * Direct the handling of an ajax callback.
 *
 * @param array $element
 */
function icalfield_ajaxy_manage_ajax_callback(array $element, &$form_state) {
  icalfield_ajaxy_setup();

  // Invoke ajaxy form validation. Start with standard form validation, then
  // move on to custom ajax specific validation for our use case.
  //
  // We preform validation manually because of the way the AJAX request happens.
  // The AJAX requests back to the page that initially generated the form. The
  // form element intercepts calls to its element instance that happened via
  // JavaScript. This happens before the form validation step so we manually
  // add the validation back in before proceeding.

  // In the ajax callback we do not want to check that the entire field
  // is required. This check should happen only when the node form is saved.
  $element['#required'] = FALSE;
  _form_validate($element, $form_state);
  icalfield_ajaxy_validate_ajax_callback($element, &$form_state);
  $success = !form_get_errors();

  // The single dates.
  $single_op = $_POST[$element['#field_name']]['single']['op'];
  $multiple_op = $_POST[$element['#field_name']]['multiple']['op'];

  if (!empty($single_op['add'])) {
    icalfield_ajaxy_single_add($element, $form_state, 'add', $success);
  }
  else if (!empty($single_op['update'])) {
    icalfield_ajaxy_single_add($element, $form_state, 'update', $success);
  }
  else if (!empty($single_op['cancel'])) {
    icalfield_ajaxy_single_cancel($element);
  }

  // Rules
  elseif (!empty($multiple_op['add'])) {
    icalfield_ajaxy_rule_add($element, $form_state, 'add', $success);
  }
  elseif (!empty($multiple_op['update'])) {
    icalfield_ajaxy_rule_add($element, $form_state, 'update', $success);
  }
  elseif (!empty($multiple_op['cancel'])) {
    icalfield_ajaxy_rule_cancel($element);
  }
}

/**
 * Validates node-form submissions via ctools ajax for adding rules or individual times.
 *
 * @param $element Array
 *    CCK field instance, possibly with data for the respective node.
 * @param &$form_state Array
 *    Form API state array for the form.
 */
function icalfield_ajaxy_validate_ajax_callback(array $element, array &$form_state) {
  // All errors generated here are for the same form field.  So there can never be more than one form error displayed per form-submission.
  $name = $element['#field_name'];

  // If we are canceling we do not want to run the data through the validation
  // since it is being thrown out. If we do not skip this form errors may be
  // thrown.
  if (isset($element['#post'][$name]['single']['op']['cancel']) || isset($element['#post'][$name]['multiple']['op']['cancel'])) {
    return;
  }

  // Validate start date present and parsable.
  if (isset($element['#post'][$name]['single']['op'])) {
    $start = $element['#post'][$name]['items']['single']['start'];
    $end = $element['#post'][$name]['items']['single']['end'];
    $all_day = $element['#post'][$name]['items']['single']['all_day'];
  }
  elseif (isset($element['#post'][$name]['multiple']['op'])) {
    $start = $element['#post'][$name]['items']['multiple']['start'];
    $end = $element['#post'][$name]['items']['multiple']['end'];
    $all_day = $element['#post'][$name]['items']['multiple']['all_day'];
  }
  // The start date is required.
  if (empty($start)) {
    form_set_error($name, t('You must add a <strong>Start</strong> date.'));
  }
  else {
    // Is the start date parsable?
    if (!strtotime($start)) {
      icalfield_ajaxy_validate_ajax_callback_invalid_date_error($element, t('Start'));
    }

    // The End date is optional.  Allow it to be empty.  But if set, is it parsable?
    if (!empty($end) && !strtotime($end)) {
      icalfield_ajaxy_validate_ajax_callback_invalid_date_error($element, t('End'));
    }

    // Is the end date later than the start date?
    if (!form_get_error($element) && ((strtotime($start) >= strtotime($end) && !$all_day) || ($all_day && strtotime($end) && strtotime($start) > strtotime($end)))) {
      form_set_error($name, t('The <strong>Start</strong> date must be <em>before</em> the <strong>End</strong> date.'));
    }
  }

  // Validate the additional fields / rules for recurrence.
  if (isset($element['#post'][$name]['multiple']['op'])) {
    // Validate ending filled in
    if (!isset($element['#post'][$name]['items']['multiple']['duration'])) {
      form_set_error($name, t('You must enter a <strong>Duration</strong>.'));
    }
    else {
      // in one of the durations and it's corresponding data available.
      if ($element['#post'][$name]['items']['multiple']['duration'] == 'date') {

        // If date was selected make sure a date was entered.
        if (empty($element['#post'][$name]['items']['multiple']['duration_end_by'])) {
          form_set_error($name, t('You must add an <strong>ending</strong> date.'));
        }

        // That the date is parseable.
        elseif (!strtotime($element['#post'][$name]['items']['multiple']['duration_end_by'])) {
          form_set_error($name, t('You must add a valid <strong>ending</strong> date.'));
        }

        // That only a date was entered. Warn them that the time will not be usable.
        else {
          $ending = date_parse($element['#post'][$name]['items']['multiple']['duration_end_by']);
          if ($ending['hour'] != 0 || $ending['minute'] != 0 || $ending['second'] != 0) {
            // A time was entered. Oops.
            form_set_error($name, t('You must add a valid <strong>ending</strong> date without a time.'));
          }
        }
      }
      elseif ($element['#post'][$name]['items']['multiple']['duration'] == 'number') {

        // If no occurance amount was entered.
        if (empty($element['#post'][$name]['items']['multiple']['duration_number_occur'])) {
          form_set_error($name, t('You must add a valid number of <strong>occurances</strong>.'));
        }

        // When the value of the occurance field is not a number. It must be a number.
        elseif (!is_numeric($element['#post'][$name]['items']['multiple']['duration_number_occur'])) {
          form_set_error($name, t('You must enter <strong>occurances</strong> as a number.'));
        }
      }
      else {
        form_set_error($name, t('An invalid <strong>Duration</strong> was entered.'));
      }
    }

    switch($element['#post'][$name]['items']['multiple']['tabs_selection']) {
      // Daily was selected.
      case 'daily':
        if (isset($element['#post'][$name]['items']['multiple']['tabs']['daily']['select']) && $element['#post'][$name]['items']['multiple']['tabs']['daily']['select'] == 'days') {
          // Make sure a value was entered is there and a real number.
          if (empty($element['#post'][$name]['items']['multiple']['tabs']['daily']['select_days']) || !is_numeric($element['#post'][$name]['items']['multiple']['tabs']['daily']['select_days'])) {
            form_set_error($name, t('An invalid number of <strong>Days</strong> was entered.'));
          }
        }
        // No daily rule was chosen
        elseif ($element['#post'][$name]['items']['multiple']['tabs']['daily']['select'] != 'daily') {
          form_set_error($name, t('You must enter values for <strong>Daily</strong> recurrences.'));
        }
        break;

      // Weekly was selected.
      case 'weekly':
        if (empty($element['#post'][$name]['items']['multiple']['tabs']['weekly']['weeks'])) {
          form_set_error($name, t('You must enter a valid number of <strong>Weeks</strong>.'));
        }
        if (!isset($element['#post'][$name]['items']['multiple']['tabs']['weekly']['days']) || (count($element['#post'][$name]['items']['multiple']['tabs']['weekly']['days']) < 1)) {
          form_set_error($name, t('You must choose one or more day of the week.'));
        }
        break;

      // Monthly was selected.
      case 'monthly':
        // Days of the month.
        if ($element['#post'][$name]['items']['multiple']['tabs']['monthly']['select'] == 'days') {
          if (empty($element['#post'][$name]['items']['multiple']['tabs']['monthly']['select_day_text']) || !is_numeric($element['#post'][$name]['items']['multiple']['tabs']['monthly']['select_day_text'])) {
            form_set_error($name, t('You must choose one or more day of the week.'));
          }
          if (empty($element['#post'][$name]['items']['multiple']['tabs']['monthly']['select_months']) || !is_numeric($element['#post'][$name]['items']['multiple']['tabs']['monthly']['select_months'])) {
            form_set_error($name, t('You must choose one or more day of the week.'));
          }
        }

        // Days in Weeks of the month.
        elseif ($element['#post'][$name]['items']['multiple']['tabs']['monthly']['select'] == 'daily') {
          if (!icalfield_ajaxy_name_to_number($element['#post'][$name]['items']['multiple']['tabs']['monthly']['select_week'])) {
            form_set_error($name, t('You must choose a week of the month.'));
          }
          if (!icalfield_ajaxy_byday($element['#post'][$name]['items']['multiple']['tabs']['monthly']['select_monthly_day'])) {
            form_set_error($name, t('You must choose a day of the week.'));
          }
          if (empty($element['#post'][$name]['items']['multiple']['tabs']['monthly']['select_daily_months']) || !is_numeric($element['#post'][$name]['items']['multiple']['tabs']['monthly']['select_daily_months'])) {
            form_set_error($name, t('You must choose a month recurrence.'));
          }
        }

        // Oops, nothing selected.
        else {
          form_set_error($name, t('You must select a <strong>Monthly</strong> option.'));
        }
        break;

      // Yearly selected.
      case 'yearly':
        // The year number of years
        if (empty($element['#post'][$name]['items']['multiple']['tabs']['yearly']['year']) || !is_numeric($element['#post'][$name]['items']['multiple']['tabs']['yearly']['year'])) {
          form_set_error($name, t('You must enter a valid number of <strong>Years</strong>.'));
        }

        // On month and day.
        if ($element['#post'][$name]['items']['multiple']['tabs']['yearly']['select'] == 'short') {
          if (!icalfield_ajaxy_bymonth($element['#post'][$name]['items']['multiple']['tabs']['yearly']['select_month'])) {
            form_set_error($name, t('You must a valid <strong>Month</strong>.'));
          }
          if (empty($element['#post'][$name]['items']['multiple']['tabs']['yearly']['select_day']) || !is_numeric($element['#post'][$name]['items']['multiple']['tabs']['yearly']['select_day'])) {
            form_set_error($name, t('You must enter a valid number of <strong>Years</strong>.'));
          }
        }

        // Detailed rule
        elseif ($element['#post'][$name]['items']['multiple']['tabs']['yearly']['select'] == 'long') {
          if (!icalfield_ajaxy_name_to_number($element['#post'][$name]['items']['multiple']['tabs']['yearly']['select_week'])) {
            form_set_error($name, t('You must choose a valid <strong>Week of the Month</strong>.'));
          }
          if (!icalfield_ajaxy_byday($element['#post'][$name]['items']['multiple']['tabs']['yearly']['select_days'])) {
            form_set_error($name, t('You must choose a valid <strong>Day of the Week</strong>.'));
          }
          if (!icalfield_ajaxy_bymonth($element['#post'][$name]['items']['multiple']['tabs']['yearly']['select_long_month'])) {
            form_set_error($name, t('You must choose a valid <strong>Month of the Year</strong>.'));
          }
        }

        else {
          form_set_error($name, t('You must select a <strong>Yearly</strong> option.'));
        }
        break;

      // Validate one of the 4 was chosen and filled in.
      default:
        form_set_error($name, t('You must enter recurrence information.'));
    }

    // Validate name filled in for rules
    if (empty($element['#post'][$name]['items']['multiple']['name'])) {
      form_set_error($name, t('You must enter a <strong>Rule Name</strong>.'));
    }
  }
}


function icalfield_ajaxy_validate_ajax_callback_invalid_date_error(array $element, $label) {
  $t_vars = array(
    '%label' => $label,
    '%date' => date(icalfield_ajaxy_get_time_format_date($element) .' '. icalfield_ajaxy_get_time_format_time($element)),
  );

  $message = t('The <strong>%label</strong> date must be formatted like "%date".  (Or something else compatible with <a href="http://www.php.net/manual/en/function.date.php"><code>strtotime()</code></a>.)', $t_vars);
  form_set_error($element['#field_name'], $message);
}

function icalfield_ajaxy_rule_add(array $element, &$form_state, $op = 'add', $validation_passed = FALSE) {
  ctools_include('ajax');
  $output = array();

  // Build new form
  $fake_form_state = array('submitted' => FALSE);
  $form = array();
  $data = array();

  // If validation passed the returned form should be empty with an add button.
  // Otherwise the data should be returned and the button be the same it was
  // before submit.
  if ($validation_passed) {
    $add = TRUE;
  }
  else {
    $add = ($op == 'add');

    // If there was an error the data should be cleaned from #post before
    // being returned to the browser.

    $data = $element['#post'][$element['#field_name']]['items']['multiple'];
    //$data = icalfield_ajaxy_filter_walk($data);
  }

  $form[$element['#field_name']]['items']['multiple'] = icalfield_ajaxy_multiple_form_multiple_instance($element, $data, $add);
  $form[$element['#field_name']]['#tree'] = TRUE;
  $rule_form = drupal_render(form_builder('ical_ajaxy_multiple_rules', $form, $fake_form_state));
  $output[] = ctools_ajax_command_replace('#'. $element['#id'] .'-multiple-wrapper', $rule_form);

  // Validation failed so we only return the form and display the errors.
  if (!$validation_passed) {
    ctools_ajax_render($output);
  }

  // Set a message about the update and that you cannot edit the details on
  // instances until the node is saved.
  if ($op == 'update') {
    drupal_set_message(t('Recurrence updated. New instances are not available to be edit until the %content_type is saved.', array('%content_type' => $form_state['values']['type'])));
  }
  else {
    drupal_set_message(t('New recurrence added. New instances are not available to be edit until the %content_type is saved.', array('%content_type' => $form_state['values']['type'])));
  }
  // Add the rule to the editor table.
  $data = icalfield_ajaxy_multiple_form_get_rule_data($element, TRUE);
  $current = icalfield_ajaxy_current_rule($data);

  $command = 'ctools_ajax_command_prepend';
  $id_namespace = $element['#id'];
  $id = "#$id_namespace-editor-items";
  if (icalfield_ajaxy_detect_existing_rule($element, $current['ical_id'])) {
    $command = 'ctools_ajax_command_replace';
    $id = "#$id_namespace-editor-items-rule-". $current['ical_id'];
  }
  $output[] = $command($id, theme('icalfield_ajaxy_multiple_form_editor_rrule', $element, $current));

  // Add data to page for processing
  if (!empty($data)) {
    // Add command to form to put data in it.
    $data_elements = drupal_render(form_builder('ical_ajaxy_multiple_data', icalfield_ajaxy_build_rrule_elements($element['#id'] .'-data-rules', $data, array($element['#field_name'], 'data', 'multiple')), $fake_form_state));
    $output[] = ctools_ajax_command_replace('#'. $element['#id'] .'-data-rules', $data_elements);
  }

  ctools_ajax_render($output);
}

/**
 * Handle ajax callback to cancel updating a rule to the multiple instance form.
 *
 * @param array $element
 */
function icalfield_ajaxy_rule_cancel(array $element) {
  ctools_include('ajax');
  $output = array();

  // Build new form
  $fake_form_state = array('submitted' => FALSE);
  $form = array();
  $data = array();

  $form[$element['#field_name']]['items']['multiple'] = icalfield_ajaxy_multiple_form_multiple_instance($element, $data, TRUE);
  $form[$element['#field_name']]['#tree'] = TRUE;
  $rule_form = drupal_render(form_builder('ical_ajaxy_multiple_rules', $form, $fake_form_state));
  $output[] = ctools_ajax_command_replace('#'. $element['#id'] .'-multiple-wrapper', $rule_form);

  ctools_ajax_render($output);
}

/**
 * Detect an existing rule in the form data.
 */
function icalfield_ajaxy_detect_existing_rule(array $element, $id) {
  $return = FALSE;

  if (!empty($element['#post'][$element['#field_name']]['data']['multiple']) && is_array($element['#post'][$element['#field_name']]['data']['multiple'])) {
    foreach ($element['#post'][$element['#field_name']]['data']['multiple'] as $key => $value) {
      if ($value['ical_id'] == $id) {
        $return = TRUE;
        continue;
      }
    }
  }

  return $return;
}

/**
 * Detect an existing rule in the form data.
 */
function icalfield_ajaxy_get_existing($type = 'single', array $element, $id) {
  $return = FALSE;

  if (!empty($element['#post'][$element['#field_name']]['data'][$type]) && is_array($element['#post'][$element['#field_name']]['data'][$type])) {
    foreach ($element['#post'][$element['#field_name']]['data'][$type] as $key => $value) {
      if ($value['ical_id'] == $id) {
        $return = $value;
        continue;
      }
    }
  }

  return $return;
}

/**
 * Get the current rule from the data structure.
 */
function icalfield_ajaxy_current_rule(array $data) {
  $return = $data[0];
  $return['value'] = unserialize($return['value']);
  return $return;
}

function icalfield_ajaxy_multiple_form_get_rule_data(array $element, $dupes = FALSE) {
  $check = FALSE;
  $id = NULL;
  $return = array();

  // The new date added via the form.
  if (isset($element['#post'][$element['#field_name']]['items']['multiple']) && !empty($element['#post'][$element['#field_name']]['items']['multiple'])) {
    // We need to get an ical_id. If one has not been set yet we need to
    // generate one. In all cases it needs to be added to ['value'] as well.
    $ical_id = (empty($element['#post'][$element['#field_name']]['items']['multiple']['ical_id'])) ? uuid_uuid() : $element['#post'][$element['#field_name']]['items']['multiple']['ical_id'];
    $ivalue = $element['#post'][$element['#field_name']]['items']['multiple'];
    $ivalue['ical_id'] = $ical_id;

    $return[] = array(
      'ical_id' => $ical_id,
      'name' => $element['#post'][$element['#field_name']]['items']['multiple']['name'],
      'value' => serialize($ivalue),
    );
    $id = $element['#post'][$element['#field_name']]['items']['multiple']['ical_id'];
    $check = TRUE;
  }
  // The existing dates, if any, already on the form.
  if (!empty($element['#post'][$element['#field_name']]['data']['multiple'])) {
    foreach($element['#post'][$element['#field_name']]['data']['multiple'] as $key => $value) {
      if (!$check || ($check && $id != $value['ical_id'])) {
        $return[] = array(
          'ical_id' => $value['ical_id'],
          'name' => $value['name'],
          'value' => $value['value'],
        );
      }
    }
  }

  return $return;
}

/**
 * Handle ajax callback to add and update a single instance to the multiple instance form.
 *
 * @param array $element
 */
function icalfield_ajaxy_single_add(array $element, &$form_state, $op = 'add', $validation_passed = FALSE) {
  ctools_include('ajax');
  $output = array();

  $id_namespace = $element['#id'];
  $fake_form_state = array('submitted' => FALSE);
  $parents = array($element['#field_name'], 'items', 'single');

  $data = array();
  // If there was an error the data should be cleaned from #post before
  // being returned to the browser.
  if (form_get_error($element)) {
    $data = $element['#post'][$element['#field_name']]['items']['single'];
    //$data = icalfield_ajaxy_filter_xss_walk($data);
  }

  // If validation passed the returned form should be empty with an add button.
  // Otherwise the data should be returned and the button be the same it was
  // before submit.
  if ($validation_passed) {
    $add = TRUE;
  }
  else {
    $add = ($op == 'add');
  }
  $form = icalfield_ajaxy_multiple_form_single_instance($element, $parents, $data, TRUE);
  $single_form = drupal_render(form_builder('ical_ajaxy_multiple_single', $form, $fake_form_state));
  $output[] = ctools_ajax_command_replace("#$id_namespace-single-wrapper", $single_form);

  // If we failed validation we just return the form and do nothing else.
  if (!$validation_passed) {
    ctools_ajax_render($output);
  }

  // Generate the table for display.
  $update = ($op == 'update');
  $data = icalfield_ajaxy_multiple_form_get_single_dates($element, $update);

  // Since editing is not available on instances until they are saved we send
  // a message to the UI.
  if ($update) {
    drupal_set_message(t('Editing revised date not available until the %content_type is saved.', array('%content_type' => $form_state['values']['type'])));
  }
  else {
    drupal_set_message(t('New date added. Editing date specific details not available until the %content_type is saved.', array('%content_type' => $form_state['values']['type'])));
  }

  // If we remove the last date and try to add a new one we will not know if
  // there is already a date editor in the editor. Instead we always remove the
  // date editor and add it back.
  $output[] = ctools_ajax_command_remove('#'. $id_namespace .'-editor-items-dates');
  $output[] = ctools_ajax_command_append('#'. $id_namespace .'-editor-items', theme('icalfield_ajaxy_multiple_form_editor_dates', $element, $data));

  // Put the actual values on the form in a usable way.
  if (!empty($data)) {
    // Add command to form to put data in it.
    $data_elements = drupal_render(form_builder('ical_ajaxy_multiple_data', icalfield_ajaxy_build_data_elements("$id_namespace-data-single", $data, array($element['#field_name'], 'data', 'single')), $fake_form_state));
    $output[] = ctools_ajax_command_replace("#$id_namespace-data-single", $data_elements);
  }

  // ctools_ajax_render() prints and exits.
  ctools_ajax_render($output);
}

/**
 * Handle ajax callback to cancel updating a single instance to the multiple instance form.
 *
 * @param array $element
 */
function icalfield_ajaxy_single_cancel(array $element) {
  ctools_include('ajax');
  $output = array();

  // Create a fresh new form.
  $fake_form_state = array('submitted' => FALSE);
  $parents = array($element['#field_name'], 'items', 'single');
  $single_form = drupal_render(form_builder('ical_ajaxy_multiple_single', icalfield_ajaxy_multiple_form_single_instance($element, $parents), $fake_form_state));
  $output[] = ctools_ajax_command_replace('#'. $element['#id'] .'-single-wrapper', $single_form);

  // @todo Parse $_POST and add hidden elements for the dates.

  ctools_ajax_render($output);
}

/**
 * Helper function; get the single dates for the multiple form.
 *
 * @param array $element
 *   The element we are getting getting date information for.
 * @param bool $dupes
 *   Duplicate checking for form compared to data based on ical id.
 */
function icalfield_ajaxy_multiple_form_get_single_dates(array $element, $dupes = FALSE) {
  $check = FALSE;
  $id = NULL;
  $return = array();

  // @todo Clean up for when there is no value. Form errors and clean info.
  // The new date added via the form.
  if (isset($element['#post'][$element['#field_name']]['items']['single']) && !empty($_POST[$element['#field_name']]['items']['single'])) {
    $return[] = array(
      'start' => $element['#post'][$element['#field_name']]['items']['single']['start'],
      'end' => $element['#post'][$element['#field_name']]['items']['single']['end'],
      'ical_id' => (empty($element['#post'][$element['#field_name']]['items']['single']['ical_id'])) ? uuid_uuid() : $element['#post'][$element['#field_name']]['items']['single']['ical_id'],
      'name' => $element['#post'][$element['#field_name']]['items']['single']['name'],
      'timezone' => $element['#post'][$element['#field_name']]['items']['single']['timezone'],
      'cancel' => $element['#post'][$element['#field_name']]['items']['single']['cancel'],
      'all_day' => $element['#post'][$element['#field_name']]['items']['single']['all_day'],
    );
    $id = $element['#post'][$element['#field_name']]['items']['single']['ical_id'];
    $check = TRUE;
  }
  // The existing dates, if any, already on the form.
  if (!empty($element['#post'][$element['#field_name']]['data']['single'])) {
    foreach($element['#post'][$element['#field_name']]['data']['single'] as $key => $value) {
      if (!$check || ($check && $id != $value['ical_id'])) {
        $return[] = array(
          'start' => $value['start'],
          'end' => $value['end'],
          'ical_id' => $value['ical_id'],
          'name' => $value['name'],
          'timezone' => $value['timezone'],
          'cancel' => $value['cancel'],
          'all_day' => $value['all_day'],
        );
      }
    }
  }

  return $return;
}

function icalfield_ajaxy_build_data_elements($id, array $data = array(), array $parents = array()) {
  $elements = array(
    '#value' => '<div id="'. $id .'" class="ical-ajaxy-hidden">',
    '#suffix' => '</div>',
  );
  if (!empty($data)) {
    foreach ($data as $key => $value) {
      $elements[$key] = icalfield_ajaxy_build_data_element($id, $key, $value, $parents);
    }
  }
  return $elements;
}

function icalfield_ajaxy_build_data_element($id, $key, array $value = array(), array $parents = array()) {
  $value += array(
    'cancel' => FALSE,
  );

  $element = array();
  $element[$key] = array(
    '#value' => '<div id="'. $id .'-'. $key .'" class="ical-ajaxy-hidden">',
    '#suffix' => '</div>',
    'start' => array(
      '#type' => 'hidden',
      '#value' => $value['start'],
    ),
    'end' => array(
      '#type' => 'hidden',
      '#value' => $value['end'],
    ),
    'name' => array(
      '#type' => 'hidden',
      '#value' => $value['name'],
    ),
    'ical_id' => array(
      '#type' => 'hidden',
      '#value' => $value['ical_id'],
    ),
    'timezone' => array(
      '#type' => 'hidden',
      '#value' => $value['timezone'],
    ),
    'cancel' => array(
      '#type' => 'hidden',
      '#value' => $value['cancel'],
    ),
    'all_day' => array(
      '#type' => 'hidden',
      '#value' => $value['all_day'],
    ),
  );
  if (!empty($parents)) {
    // @todo Should this be a loop over the children? Or, just use #tree :/
    $element[$key]['start']['#parents'] = $parents;
    $element[$key]['start']['#parents'][] = $key;
    $element[$key]['start']['#parents'][] = 'start';
    $element[$key]['end']['#parents'] = $parents;
    $element[$key]['end']['#parents'][] = $key;
    $element[$key]['end']['#parents'][] = 'end';
    $element[$key]['name']['#parents'] = $parents;
    $element[$key]['name']['#parents'][] = $key;
    $element[$key]['name']['#parents'][] = 'name';
    $element[$key]['ical_id']['#parents'] = $parents;
    $element[$key]['ical_id']['#parents'][] = $key;
    $element[$key]['ical_id']['#parents'][] = 'ical_id';
    $element[$key]['timezone']['#parents'] = $parents;
    $element[$key]['timezone']['#parents'][] = $key;
    $element[$key]['timezone']['#parents'][] = 'timezone';
    $element[$key]['cancel']['#parents'] = $parents;
    $element[$key]['cancel']['#parents'][] = $key;
    $element[$key]['cancel']['#parents'][] = 'cancel';
    $element[$key]['all_day']['#parents'] = $parents;
    $element[$key]['all_day']['#parents'][] = $key;
    $element[$key]['all_day']['#parents'][] = 'all_day';
  }
  return $element;
}

/**
 * Reconstruct format for JavaScript
 *
 * @param string $format
 *   The PHP formattedstring.
 * @return string
 *   Format sting for javaScript
 */
function icalfield_ajaxy_format_to_js($format) {
  $format = strtr($format, icalfield_ajaxy_datepicker_format_replacements());
  $search = icalfield_ajaxy_timepicker_format_replacements();

  // The jQuery UI timepicker does not work with configurable formats.
  // It does display in a very similar manner to the allowed formats so
  // we just remove the time.
  // Note: Users may add formats that don't work with this.
  $format = str_replace($search, '', $format);
  return $format;
}

/**
 * Helper function to clean up the vevent record for the vevent
 * to parse.
 *
 * @param string $vevent
 * @return string
 */
function _icalfield_ajaxy_clean_vevent($vevent) {
  // The beginning and ending of the vevent aren't handled by the vevent parser. It is
  // only looking for properties and not the surrounding to them. I believe this is due
  // to the parser trying to be used by a calendar and not for an individual property.
  $vevent = str_replace(array('BEGIN:VEVENT', 'END:VEVENT'), '', $vevent);

  // The parser provided by iCalcreator takes the entire file as one line and
  // then pulls out just the first property. With multiple properties in the
  // vevent this just won't work. The properties are not picked up. Here we
  // convert each new line into somthing that can be parsed. Be warned,
  // mutliple properites cannot live on the same line.
  return explode("\r\n", $vevent);
}

/**
 * Clean unwanted cruft from the values array.
 *
 * The values, as CCK uses them, are an array of values with numeric keys.
 * To maintain that we are removing the bits that are part of the UI but
 * not used by CCK.
 *
 * @param array $values
 *   The values array.
 *
 * @return array
 *   Values array with cruft removed.
 */
function _icalfield_ajaxy_clean_values($values) {
  if (!empty($values)) {
    foreach ($values as $key => $value) {
      if (!is_numeric($key)) {
        unset($values[$key]);
      }
    }
  }
  return icalfield_filter_empty($values);
}

/**
 * Turn an ical rule cck field and turn it into the data structure needed for
 * storing in the page and for defaults into rule forms.
 *
 * Note: each field (vevent) can only contain one rrule for this to work. This
 * is how the current setup works.
 *
 * @param array $field
 *   The ical cck field containing a rule.
 *
 * @return
 *  - FALSE: if the ical field does not contain a rule.
 *  - An array with the key:
 *    - ical_id: uuid for the field.
 *    - name: the name of the rule.
 *    - value: the data array for the rule.
 */
function _icalfield_ajaxy_field_to_data(array $field, $serialize = FALSE) {
  $data = array(
    'ical_id' => $field['ical_id'],
    'name' => $field['name'],
  );

  // Build a vevent object.
  $vevent = _icalfield_ajaxy_build_vevent($field);

  // No rrule present return so FALSE.
  $rrule = (isset($vevent->rrule[0]['value']) && !empty($vevent->rrule[0]['value'])) ? $vevent->rrule[0]['value'] : FALSE;
  if (empty($rrule)) {
    return FALSE;
  }

  // Get rrule date with timezone included.
  // We are making a direct call rather than using getProperty because that method
  // is just not working here.
  $rrule = $vevent->getProperty('rrule', FALSE, TRUE);

  // The BYMONTH value can be an array. In the spec it is stored as something
  // like '10,11,12'. The widget only uses a single date. The vevent object
  // stores a single date as a string and multiple as an array. So, if there
  // is more than one value we take just the first one.
  if (!empty($rrule['value']['BYMONTH']) && is_array($rrule['value']['BYMONTH'])) {
    foreach ($rrule['value']['BYMONTH'] as $k => $v) {
      $rrule['value']['BYMONTH'] = $v;
      continue;
    }
  }

  $data['value'] = array();

  // Add the name to the value.
  $data['value']['name'] = $field['name'];

  $data['value']['all_day'] = $vevent->isAllDay();

  // The start and end dates and time
  $start = $vevent->getProperty('dtstart', FALSE, TRUE);
  $timezone = ($start['params']['TZID']) ? $start['params']['TZID'] : date_default_timezone_name(TRUE);

  if ($vevent->isAllDay()) {
    $time_string = $start['value']['month'] .'/'. $start['value']['day'] .'/'. $start['value']['year'];
  }
  else {
    $time_string = $start['value']['month'] .'/'. $start['value']['day'] .'/'. $start['value']['year'] .' '. $start['value']['hour'] .':'. $start['value']['min'] .':'. $start['value']['sec'];
  }
  $start = new DateTime($time_string, new DateTimezone($timezone));
  // @todo Change to a more consistent format.
  // Using date_format_date instead for format_date because it handles the
  // i18n while allowing more of the available formats for date/time display.
  if ($data['value']['all_day']) {
    $data['value']['start'] =  date_format_date($start, 'custom', icalfield_ajaxy_get_time_format_date(array()));
  }
  else {
    $data['value']['start'] =  date_format_date($start, 'custom', icalfield_ajaxy_get_time_format_date(array()) .' '. icalfield_ajaxy_get_time_format_time(array()));
  }

  $end = $vevent->getProperty('dtend', FALSE, TRUE);
  if (!empty($end)) {
    $timezone = ($end['params']['TZID']) ? $end['params']['TZID'] : date_default_timezone_name(TRUE);
    if ($data['value']['all_day']) {
      $time_string = $end['value']['month'] .'/'. $end['value']['day'] .'/'. $end['value']['year'];
    }
    else {
      $time_string = $end['value']['month'] .'/'. $end['value']['day'] .'/'. $end['value']['year'] .' '. $end['value']['hour'] .':'. $end['value']['min'] .':'. $end['value']['sec'];
    }
    $end = new DateTime($time_string, new DateTimezone($timezone));
    // @todo Change to a more consistent format.
    // Using date_format_date instead for format_date because it handles the
    // i18n while allowing more of the available formats for date/time display.
    if ($data['value']['all_day']) {
      $data['value']['end'] =  date_format_date($end, 'custom', icalfield_ajaxy_get_time_format_date(array()));
    }
    else {
      $data['value']['end'] =  date_format_date($end, 'custom', icalfield_ajaxy_get_time_format_date(array()) .' '. icalfield_ajaxy_get_time_format_time(array()));
    }
  }
  // Add timezone to data structure.
  $data['value']['timezone'] = $rrule['params']['TZID'];

  // Discover which rule is present.
  switch($rrule['value']['FREQ']) {
    case 'DAILY':
      // $data['tabs_selection'] is the active tab for interface.
      $data['value']['tabs_selection'] = 'daily';
      if (isset($rrule['value']['INTERVAL']) && !empty($rrule['value']['INTERVAL'])) {
        $data['value']['tabs']['daily']['select'] = 'days';
        $data['value']['tabs']['daily']['select_days'] = $rrule['value']['INTERVAL'];
      }
      // If there is no interval the event is every day.
      else {
        $data['value']['tabs']['daily']['select'] = 'days';
        $data['value']['tabs']['daily']['select_days'] = 1;
      }
      break;
    case 'WEEKLY':
      // There is a special case for week days (M-F). This is handled as a weekly
      // rule with a byday for M-F with week start of SU. We need to detect and
      // handle this first.
      $week_days = '';
      if (!empty($rrule['value']['BYDAY'])) {
        foreach ($rrule['value']['BYDAY'] as $k => $v) {
          $week_days .= $v['DAY'];
          // If we have a week days that occurs differently than every week we
          // do not have a set of week days.
          if (!empty($v[0]) && $v[0] > 1) {
            continue;
          }
        }
      }
      if ($rrule['value']['WKST'] == 'SU' && $week_days == 'MOTUWETHFR') {
        // The weekday special case.
        $data['value']['tabs_selection'] = 'daily';
        $data['value']['tabs']['daily']['select'] = 'daily';
      }
      else {
        $data['value']['tabs_selection'] = 'weekly';
        $data['value']['tabs']['weekly']['weeks'] = !empty($rrule['value']['INTERVAL']) ? $rrule['value']['INTERVAL'] : NULL;
        if (isset($rrule['value']['BYDAY']) && !empty($rrule['value']['BYDAY'])) {
          $byday = _icalfield_ajaxy_normalize_byday($rrule['value']['BYDAY']);
          foreach ($byday as $day) {
            $data['value']['tabs']['weekly']['days'][icalfield_ajaxy_byday_reverse($day['DAY'])] = icalfield_ajaxy_byday_reverse($day['DAY']);
          }
        }
      }
      break;
    case 'MONTHLY':
      $data['value']['tabs_selection'] = 'monthly';
      if (!empty($rrule['value']['BYDAY'])) {
        $byday = _icalfield_ajaxy_normalize_byday($rrule['value']['BYDAY']);
        $data['value']['tabs']['monthly']['select'] = 'daily';
        $data['value']['tabs']['monthly']['select_monthly_day'] = icalfield_ajaxy_byday_reverse($byday[0]['DAY']);
        $data['value']['tabs']['monthly']['select_week'] = icalfield_ajaxy_number_to_name($byday[0][0]);
        $data['value']['tabs']['monthly']['select_daily_months'] = $rrule['value']['INTERVAL'];
      }
      elseif(!empty($rrule['value']['BYMONTHDAY'])) {
        $data['value']['tabs']['monthly']['select'] == 'days';
        $data['value']['tabs']['monthly']['select_day_text'] = $rrule['value']['BYMONTHDAY'];
        $data['value']['tabs']['monthly']['select_months'] = $rrule['value']['INTERVAL'];
      }
      break;
    case 'YEARLY':
      $data['value']['tabs_selection'] = 'yearly';
      $data['value']['tabs']['yearly']['year'] = $rrule['value']['INTERVAL'];
      if (!empty($rrule['value']['BYMONTHDAY'])) {
        $data['value']['tabs']['yearly']['select'] = 'short';
        $data['value']['tabs']['yearly']['select_month'] = icalfield_ajaxy_bymonth_reverse($rrule['value']['BYMONTH']);
        $data['value']['tabs']['yearly']['select_day'] = $rrule['value']['BYMONTHDAY'];
      }
      else {
        $data['value']['tabs']['yearly']['select'] = 'long';
        $data['value']['tabs']['yearly']['select_long_month'] = icalfield_ajaxy_bymonth_reverse($rrule['value']['BYMONTH']);
        $byday = _icalfield_ajaxy_normalize_byday($rrule['value']['BYDAY']);
        if (is_numeric($byday[0][0])) {
          $data['value']['tabs']['yearly']['select_week'] = icalfield_ajaxy_number_to_name($byday[0][0]);
          $data['value']['tabs']['yearly']['select_days'] = icalfield_ajaxy_byday_reverse($byday[0]['DAY']);
        }
        else {
          $data['value']['tabs']['yearly']['select_days'] = icalfield_ajaxy_byday_reverse($byday[0]['DAY']);
        }
      }
      break;
  }

  // Calculate the duration.
  if (!empty($rrule['value']['COUNT'])) {
    $data['value']['duration'] = 'number';
    $data['value']['duration_number_occur'] = $rrule['value']['COUNT'];
  }
  elseif ($rrule['value']['UNTIL']) {
    $data['value']['duration'] = 'date';

    // There are cases where the UNTIL value can be an array of information.
    // So, we need to convert it.
    if (is_array($rrule['value']['UNTIL'])) {
      $_duration_end_by = new DateTime($vevent->_format_date_time($rrule['value']['UNTIL']));
      $rrule['value']['UNTIL'] = date_format_date($_duration_end_by, 'custom', icalfield_ajaxy_get_time_format_date(array()));
    }

    $data['value']['duration_end_by'] = $rrule['value']['UNTIL'];
  }

  $data['value']['cancel'] = FALSE;
  if ($vevent->getProperty('STATUS') == 'CANCELLED') {
    $data['value']['cancel'] = TRUE;
  }

  // The ical_id needs to be on the value for updates.
  $data['value']['ical_id'] = $field['ical_id'];

  // Store the exdates.
  $data['value']['exdates'] = $vevent->exdate;

  if ($serialize) {
    $data['value'] = serialize($data['value']);
  }
  return $data;
}

/**
 * Helper funtion to brige between converstion and interating over array.
 */
function _icalfield_ajaxy_convert_rules_data(&$field, $key) {
  $field = _icalfield_ajaxy_field_to_data($field, TRUE);
}

/**
 * Get the date formatter string for a CCK field.
 *
 * @param $element Array
 *    CCK field instance, possibly with data for the respective node.
 * @return String
 *    date()-compatible formatter.
 */
function icalfield_ajaxy_get_time_format_date(array $element) {

  // The format needs to be one that can be parsed by the DataTime class.
  return variable_get('icalfield_ajaxy_date_format_date', 'm/d/Y');
}

/**
 * Get the time formatter string for a CCK field.
 *
 * @param $element Array
 *    CCK field instance, possibly with data for the respective node.
 * @return String
 *    date()-compatible formatter.
 */
function icalfield_ajaxy_get_time_format_time(array $element) {

  // The format needs to be one that can be parsed by the DataTime class.
  return variable_get('icalfield_ajaxy_date_format_time', 'h:i a');
}

/**
 * Run filter_xss over all the items in an array.
 *
 * @param $item
 */
function icalfield_ajaxy_filter_walk(&$item) {
  if (is_array($item)) {
    array_walk($item, 'icalfield_ajaxy_filter_walk');
  }
  else {
    $item = check_plain($item);
  }
}

/**
 * For a given icalfield value and date find the existing node.
 *
 * This is based on the lookup in the ical instance processor.
 * @see ICalInstanceProcessor::getExistingInstance
 * @todo combine this with getExistingInstance or create lookup class.
 *
 * @param array $config
 *   An array of information needed to lookup the instance node:
 *   - nid: The node id for the event node.
 *   - field_name: The name of the field holding the ical info.
 *   - ical_id: The uuid for the field value we are looking for.
 * @param DateTime $date
 *   The date and time for the existing instance we are looking for.
 * @param DateTimeZone $dateZoneDb
 *   The database timezone.
 *
 * @return int
 *   The node id for the existing node or FALSE if one was not found.
 */
function icalfield_ajaxy_get_existing_node(array $config, DateTime $date, DateTimeZone $dateZoneDb) {

  // Since we are alterning the timezone to that of the one from the database
  // cloning the object is used so it is not altered in other places it may
  // be used.
  $date_db = clone($date);
  $date_db->setTimezone($dateZoneDb);

  // @todo This could probably be optimized for the case that both fields are
  // on the same table.
  $existing_nid = db_result(db_query("SELECT rf.nid
    FROM {{$this->referenceFieldDb['table']}} rf
      INNER JOIN {{$this->dateFieldDb['table']}} df ON (rf.nid = df.nid AND rf.vid = df.vid)
    WHERE rf.{$this->referenceFieldDb['columns']['nid']['column']} = %d
      AND rf.{$this->referenceFieldDb['columns']['field']['column']} = '%s'
      AND rf.{$this->referenceFieldDb['columns']['property_id']['column']} = '%s'
      AND df.{$this->dateFieldDb['columns']['value']['column']} = '%s' ", $config['nid'], $config['field_name'], $config['ical_id'], $date_db->format(DATE_DATETIME_FORMAT)));

  return $existing_nid;
}

/**
 * Setup actions that need to happen on all page requests.
 */
function icalfield_ajaxy_setup() {
  // The date popup module adds datepicker theming that hoses the default
  // jquery ui theming and it puts it on all pages. If that module is enabled
  // we need to do some css cleanup.
  if (module_exists('date_popup')) {
    drupal_add_css(drupal_get_path('module', 'icalfield_ajaxy') .'/datepicker.css', 'theme');
  }
}

/**
 * Take the different varations byday can sit inside the vevent and normalize them to one method.
 */
function _icalfield_ajaxy_normalize_byday($byday) {
  if (array_key_exists('DAY', $byday)) {
    $byday = array($byday);
  }
  return $byday;
}