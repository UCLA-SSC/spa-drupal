<?php
// $Id: icalfield_ajaxy.rule.inc,v 1.1.2.1 2010/10/06 15:24:02 mfer Exp $

/**
 * @file
 * The handling code for rules.
 */

/**
 * Menu callback; dispatch for updating single dates.
 *
 * @param string $op
 *   The operation. Examples are update, cancel, and delete.
 * @param string $id
 *   The id for the single date. This is the ical_id.
 */
function icalfield_ajaxy_rule_dispatch($op, $field_name, $id, $arg = NULL) {
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

      $parents = array($field_name, 'items', 'multiple');
      $data = icalfield_ajaxy_multiple_form_get_rule_data($element);
      $data = icalfield_ajaxy_get_existing('multiple', $element, $id);
      $data['value'] = unserialize($data['value']);
      $data['value']['items']['multiple']['ical_id'] = $id;

      $form = array();
      $form[$element['#field_name']]['items']['multiple'] = icalfield_ajaxy_multiple_form_multiple_instance($element, $data['value'], FALSE);
      $form[$element['#field_name']]['#tree'] = TRUE;
      $rule_form = drupal_render(form_builder('ical_ajaxy_multiple_rules', $form, $fake_form_state));

      $output[] = ctools_ajax_command_replace('#'. $element['#id'] .'-multiple-wrapper', $rule_form);
      ctools_ajax_render($output);
    case 'cancel':
      $output = array();
      // We fake the form state for the form builder.
      $fake_form_state = array('submitted' => FALSE);
      // Faking the parents to stick it how we want in the form.
      $parents = array($field_name, 'data', 'multiple');
      // The data should be the single item that was sent back by our link.
      $data = icalfield_ajaxy_multiple_form_get_rule_link_data($element);

      // We iterate over the one item that was just canceled.
      foreach ($data as $key => $value) {
        // Flip the value for canceled.
        $value['value']['cancel'] = ($value['value']['cancel'] == 1) ? FALSE : TRUE;

        // Serialize the value to be passed as text to the page.
        $value['value'] = serialize($value['value']);

        $data_element = drupal_render(form_builder('ical_ajaxy_multiple_data', icalfield_ajaxy_build_rrule_element($element['#id'] .'-data-rules', $key, $value, $parents), $fake_form_state));
        $output[] = ctools_ajax_command_replace('#'. $element['#id'] .'-data-rules-'. $key, $data_element);
      }

      ctools_ajax_render($output);
    case 'exclude':
      // Exclude a single date.
      $output = array();
      // We fake the form state for the form builder.
      $fake_form_state = array('submitted' => FALSE);
      // Faking the parents to stick it how we want in the form.
      $parents = array($field_name, 'data', 'multiple');
      // The data should be the single item sent back from our link.
      $data = icalfield_ajaxy_multiple_form_get_rule_link_data($element);

      // We iterate over the one item.
      foreach ($data as $key => $value) {
        // Add the exdate.
        $timezone = new DateTimeZone($value['value']['timezone']);
        // Verify the date is a numer. We are expecting a unix timestamp.
        if ($exdate = new DateTime(rawurldecode($arg), $timezone)) {
          $vevent = _icalfield_ajaxy_build_vevent($value);

          // We need to detect if the exdate is on the item. If not add it, if
          // its there remove it. This is a not so simple flip.
          $existingExdates = $vevent->getExdates();

          // Search for a match.
          $found = FALSE;
          if (!empty($existingExdates)) {
            // Loop through the dates.
            foreach ($existingExdates as $k => $v) {
              if ($v == $exdate) {
                // We have a match so remove it.
                $vevent->removeExdate($exdate);
                $found = TRUE;
              }
            }
          }

          // No existing was found so we add it.
          if (!$found) {
            $vevent->addExdate($exdate);
          }

          $dummy = array();
          $value['value'] = $vevent->createComponent($dummy);
          $value = _icalfield_ajaxy_field_to_data($value, TRUE);

          // Convert it to a $_POST data array and send back.
          $data_element = drupal_render(form_builder('ical_ajaxy_multiple_data', icalfield_ajaxy_build_rrule_element($element['#id'] .'-data-rules', $key, $value, $parents), $fake_form_state));
          $output[] = ctools_ajax_command_replace('#'. $element['#id'] .'-data-rules-'. $key, $data_element);

          // Render stuff.
          ctools_ajax_render($output);
        }

        // There was an error!
        else {
          ctools_ajax_render_error('Invalid data type.');
        }
      }

    default:
      // We have an error.
      // @todo Report errors to screen or watchdog?
      die();
  }
}

/**
 * Helper function; return the events with rrules.
 *
 * @param array $values
 *   The cck values.
 *
 * @return array
 *   The values with rrules on the vevents.
 */
function _icalfield_ajaxy_get_rules(array $values = array()) {
  return array_filter($values, '_icalfield_ajaxy_get_rules_callback');
}

/**
 * See if there is an RRULE on the CCK value.
 */
function _icalfield_ajaxy_get_rules_callback($value) {
  $vevent = _icalfield_ajaxy_build_vevent($value);
  $rrule = $vevent->rrule[0];
  return !empty($rrule);
}

/**
 * The theme function for visualizing rules in the editor.
 *
 * @param array $element
 *   The element the editor lives on.
 * @param array $data
 *   The data for the individual rule.
 *
 * @return string
 *   The themed rule for the editor.
 */
function theme_icalfield_ajaxy_multiple_form_editor_rrule($element, $data) {
  $value = $data['value'];
  // Open the rule header and add the name as an update link.
  $title_classes = array('ical-ajaxy-editor-title', 'clear-block');
  if ($value['cancel']) {
    $title_classes[] = 'ical-ajaxy-canceled';
  }
  $title_classes = implode(' ', $title_classes);
  $output = '<div id="'. $element['#id'] .'-editor-items-rule-'. $data['ical_id'] .'" class="icalfield-ajaxy-header"><div class="'. $title_classes .'"><span class="icalfield-ajaxy-rule-title"><span class="ui-icon ui-icon-triangle-1-e ui-icon-triangle-1-s icalfield-ajaxy-collapse-button"></span>'. l($data['value']['name'] .' '. t('(edit recurrence)'), 'icalfield/ajaxy/rule/update/'. $element['#field_name'] .'/'. $data['ical_id'], array('attributes' => array('class' => 'ical-ajaxy-rule-update', 'ical_id' => $data['ical_id'], 'title' => t('Edit Recurrence')))) .'</span>';

  // The cancel link.
  $cancel_text = $value['cancel'] ? t('Canceled') : t('Cancel');
  $output .= '<span class="icalfield-ajaxy-rule-cancel">'. l($cancel_text, 'icalfield/ajaxy/rule/cancel/'. $element['#field_name'] .'/'. $value['ical_id'], array('attributes' => array('class' => 'ical-ajaxy-rule-cancel ui-icon ui-icon-cancel', 'ical_id' => $data['ical_id'], 'title' => t('Cancel')))) .'</span>';

  // The remove link.
  $output .= '<span class="icalfield-ajaxy-rule-remove">'. l(t('Remove'), 'icalfield/ajaxy/rule/remove/'. $element['#field_name'] .'/'. $value['ical_id'], array('attributes' => array('class' => 'ical-ajaxy-rule-remove ui-icon ui-icon-trash', 'ical_id' => $data['ical_id'], 'title' => t('Remove')))) .'</span>';

  // Close out the header row.
  $output .= '</div>';

  // Get a list of dates to display.
  $vevent = _icalfield_ajaxy_build_vevent($data);
  $start = $vevent->getStart();

  // There is currently no way to know the number of items we may have. Deriving
  // the number is not a trivial task with the way rules, exdates, rdates, etc
  // work and can be on a vevent. Instead we are assuming it will be 500 or less.
  // The iterator will do 500 or less items. If there are less it will just
  // generate the number there are. This is being hard coded at 500 because the
  // UI does not really handle that many.
  $iterator = $vevent->getIteratorCount(500, $start);

  // The iterator only gets actual dates. We want to display exdates as well and
  // mark them as not being included.
  $exdates = $vevent->getExdates();

  // Now, we have to have to add the two lists together. We assume they are
  // already in a sorted order.
  // We cannot use array sorting because our values are dateTime objects and
  // one of the lists is an iterator object.
  $exdates_sort = $exdates;
  $thereBeDates = array();
  foreach ($iterator as $key => $valueData) {
    // Go through the exdates and add those before the one form the iterator.
    if (!empty($exdates_sort)) {
      foreach ($exdates_sort as $k => $v) {
        if ($valueData > $v) {
          // Add the date to the array since it is before the iterators date.
          $thereBeDates[] = $v;
          // Remove the value from the exdates.
          unset($exdates_sort[$k]);
        }
      }
    }

    // Add the value from the iterator.
    $thereBeDates[] = $valueData;
  }

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

  $dates = array();
  foreach ($thereBeDates as $k => $v) {

    $classes = array();
    $classes[] = 'icalfield-ajaxy-rule-row';
    $exclude_classes = array('ical-ajaxy-rule-exclude', 'ui-icon', 'ui-icon-check');

    // Set the text if the item is included/excluded.
    if (in_array($v, $exdates)) {
      $excluded = t('Excluded');
      $classes[] = 'ical-ajaxy-excluded';
      $exclude_classes[] = 'ui-icon-circle-minus';
    }
    else {
      $excluded = t('Included');
    }

    // Edit links can only show where there is an existing instance node.
    // If there is no existing instance node the vevent may not vet be saved
    // to the database and in that case we do not want to create orphaned
    // instance nodes.
    $edit_link = '';
    if ($lookup && $link_nid = $lookup->getExistingInstance($value['ical_id'], $v)) {
      $edit_link = l('Edit', 'node/'. $link_nid .'/edit', array('attributes' => array('class' => 'automodal ui-icon ui-icon-pencil', 'title' => t('Edit Instance'))));
    }

    $classes = implode(' ', $classes);
    $exclude_classes = implode(' ', $exclude_classes);

    $time_text = ($vevent->isAllDay()) ? t('All day') : date_format_date($v, 'custom', 'g:i a T');

    $dates[] = array(
      array(
        // Since there is no uuid for these we are using the unix timestamp.
        'data' => l($excluded, 'icalfield/ajaxy/rule/exclude/'. $element['#field_name'] .'/'. $value['ical_id'] .'/'. rawurlencode($v->format(DATE_DATETIME_FORMAT)), array('attributes' => array('class' => $exclude_classes, 'ical_id' => $data['ical_id'], 'title' => t('Include Date')))),
        'class' => $classes,
      ),
      array(
        // We use date_format_date() to make it translation happy.
        'data' => date_format_date($v, 'custom', 'n/j/Y'),
        'class' => $classes,
      ),
      array(
        'data' => $time_text,
        'class' => $classes,
      ),
      array(
        'data' => $edit_link,
        'class' => $classes,
      ),
    );
  }

  // Display messages about the table being updated.
  if ($element['#post']['js'] == 1) {
    $output .= '<div class="icalfield-ajaxy-message">'. theme('status_messages') .'</div>';
  }
  // Display the dates in a table
  $header = array(t('Included'), t('Date'), t('Time'), t('Edit'));
  $output .= '<div class="icalfield-ajaxy-table">'. theme('table', $header, $dates, array('class' => 'icalfield-ajaxy-rule-table')) .'</div>';
  $output .= '</div>';
  return $output;
}

/**
 * Helper function; get the data for canceling a rule, removing a date, etc.
 *
 * This function is expecting the $_POST data that populated the element to have
 * come from a link on the form. That link serializes and only sends back
 * the single item we are working with. All others are ignored.
 *
 * @param array $element
 *   The element we are getting getting date information for.
 */
function icalfield_ajaxy_multiple_form_get_rule_link_data(array $element) {
  // The existing dates, if any, already on the form.
  // We cannot know the key we are using here because it could be almost anything.
  // Instead we iteratate over the one item.
  if (!empty($element['#post'][$element['#field_name']]['data']['multiple'])) {
    foreach($element['#post'][$element['#field_name']]['data']['multiple'] as $key => $value) {
      $return[$key] = array(
        'ical_id' => $value['ical_id'],
        'name' => $value['name'],
        'value' => unserialize($value['value']),
      );
    }
  }

  return $return;
}

// @todo Combine with icalfield_ajaxy_build_data_elements.

/**
 * Build out the data section of the element for the rules.
 *
 * @param $id
 *   The id for the wrapping div to the data.
 * @param $data
 *   The data array of items to be added to the data area.
 * @param $parents
 *   The parents, if any, to add to the form elements.
 */
function icalfield_ajaxy_build_rrule_elements($id, array $data = array(), array $parents = array()) {
  $elements = array(
    '#value' => '<div id="'. $id .'" class="ical-ajaxy-hidden">',
    '#suffix' => '</div>',
  );
  if (!empty($data)) {
    foreach ($data as $key => $value) {
      $elements[$key] = icalfield_ajaxy_build_rrule_element($id, $key, $value, $parents);
    }
  }
  return $elements;
}

/**
 * Generate an individual rule element for the data area.
 * @param string $id
 * @param int $key
 * @param array $value
 * @param array $parents
 */
function icalfield_ajaxy_build_rrule_element($id, $key, $value = array(), array $parents = array()) {

  // @todo revise in page storage mechanism to use vevents.
  // FIXME:
  if (!is_array($value)) $value = array('name' => NULL, 'ical_id' => NULL, 'value' => NULL);

  $value += array(
    'cancel' => FALSE,
  );

  $element = array();
  $element[$key] = array(
    '#value' => '<div id="'. $id .'-'. $key .'" class="ical-ajaxy-hidden">',
    '#suffix' => '</div>',
    'name' => array(
      '#type' => 'hidden',
      '#value' => $value['name'],
    ),
    'ical_id' => array(
      '#type' => 'hidden',
      '#value' => $value['ical_id'],
    ),
    'value' => array(
      '#type' => 'hidden',
      '#value' => $value['value'],
    ),
  );

  if (!empty($parents)) {
    // @todo Should this be a loop over the children?
    $element[$key]['name']['#parents'] = $parents;
    $element[$key]['name']['#parents'][] = $key;
    $element[$key]['name']['#parents'][] = 'name';
    $element[$key]['ical_id']['#parents'] = $parents;
    $element[$key]['ical_id']['#parents'][] = $key;
    $element[$key]['ical_id']['#parents'][] = 'ical_id';
    $element[$key]['value']['#parents'] = $parents;
    $element[$key]['value']['#parents'][] = $key;
    $element[$key]['value']['#parents'][] = 'value';
  }
  return $element;
}
