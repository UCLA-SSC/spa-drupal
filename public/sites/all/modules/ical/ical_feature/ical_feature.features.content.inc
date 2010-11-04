<?php

/**
 * Implementation of hook_content_default_fields().
 */
function ical_feature_content_default_fields() {
  $fields = array();

  // Exported field: field_event_description
  $fields['event-field_event_description'] = array(
    'field_name' => 'field_event_description',
    'type_name' => 'event',
    'display_settings' => array(
      'weight' => '-4',
      'parent' => '',
      'label' => array(
        'format' => 'above',
      ),
      'teaser' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'text',
    'required' => '0',
    'multiple' => '0',
    'module' => 'text',
    'active' => '1',
    'text_processing' => '1',
    'max_length' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'widget' => array(
      'rows' => '5',
      'size' => 60,
      'default_value' => array(
        '0' => array(
          'value' => '',
          'format' => '1',
          '_error_element' => 'default_value_widget][field_event_description][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Description',
      'weight' => '-4',
      'description' => '',
      'type' => 'text_textarea',
      'module' => 'text',
    ),
  );

  // Exported field: field_ical
  $fields['event-field_ical'] = array(
    'field_name' => 'field_ical',
    'type_name' => 'event',
    'display_settings' => array(
      'weight' => '-2',
      'parent' => '',
      'label' => array(
        'format' => 'hidden',
      ),
      'teaser' => array(
        'format' => 'ical',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'ical',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'ical',
    'required' => '1',
    'multiple' => '1',
    'module' => 'icalfield',
    'active' => '1',
    'icalinstance_enabled' => 1,
    'icalinstance_node_type' => 'event_instance',
    'icalinstance_reference_field' => 'field_ical_reference',
    'icalinstance_date_field' => 'field_event_date',
    'icalinstance_count' => '20',
    'icalinstance_status_field' => 'field_event_status',
    'widget' => array(
      'default_time' => '8:00 am',
      'default_value' => NULL,
      'default_value_php' => NULL,
      'label' => 'iCal',
      'weight' => '-2',
      'description' => '',
      'type' => 'ical_ajaxy',
      'module' => 'icalfield_ajaxy',
    ),
  );

  // Exported field: field_event_date
  $fields['event_instance-field_event_date'] = array(
    'field_name' => 'field_event_date',
    'type_name' => 'event_instance',
    'display_settings' => array(
      'weight' => '3',
      'parent' => '',
      'label' => array(
        'format' => 'above',
      ),
      'teaser' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'date',
    'required' => '1',
    'multiple' => '0',
    'module' => 'date',
    'active' => '1',
    'granularity' => array(
      'year' => 'year',
      'month' => 'month',
      'day' => 'day',
      'hour' => 'hour',
      'minute' => 'minute',
    ),
    'timezone_db' => 'UTC',
    'tz_handling' => 'date',
    'todate' => 'optional',
    'repeat' => 0,
    'repeat_collapsed' => '',
    'default_format' => 'medium',
    'widget' => array(
      'default_value' => 'blank',
      'default_value_code' => '',
      'default_value2' => 'same',
      'default_value_code2' => '',
      'input_format' => 'm/d/Y - H:i:s',
      'input_format_custom' => '',
      'increment' => 1,
      'text_parts' => array(),
      'year_range' => '-3:+3',
      'label_position' => 'above',
      'label' => 'Event Date',
      'weight' => '3',
      'description' => '',
      'type' => 'date_text',
      'module' => 'date',
    ),
  );

  // Exported field: field_event_description
  $fields['event_instance-field_event_description'] = array(
    'field_name' => 'field_event_description',
    'type_name' => 'event_instance',
    'display_settings' => array(
      'weight' => '-4',
      'parent' => '',
      'label' => array(
        'format' => 'above',
      ),
      'teaser' => array(
        'format' => 'inherit_field_ical_reference',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'inherit_field_ical_reference',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'text',
    'required' => '0',
    'multiple' => '0',
    'module' => 'text',
    'active' => '1',
    'text_processing' => '1',
    'max_length' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'widget' => array(
      'rows' => '5',
      'size' => 60,
      'default_value' => array(
        '0' => array(
          'value' => '',
          '_error_element' => 'default_value_widget][field_event_description][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Description',
      'weight' => '-4',
      'description' => '',
      'type' => 'text_textarea',
      'module' => 'text',
    ),
  );

  // Exported field: field_event_status
  $fields['event_instance-field_event_status'] = array(
    'field_name' => 'field_event_status',
    'type_name' => 'event_instance',
    'display_settings' => array(
      'weight' => '4',
      'parent' => '',
      'label' => array(
        'format' => 'above',
      ),
      'teaser' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'text',
    'required' => '1',
    'multiple' => '0',
    'module' => 'text',
    'active' => '1',
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'widget' => array(
      'default_value' => array(
        '0' => array(
          'value' => '',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Event Status',
      'weight' => '4',
      'description' => '',
      'type' => 'icalstatus',
      'module' => 'icalstatus',
    ),
  );

  // Exported field: field_ical_reference
  $fields['event_instance-field_ical_reference'] = array(
    'field_name' => 'field_ical_reference',
    'type_name' => 'event_instance',
    'display_settings' => array(
      'weight' => '2',
      'parent' => '',
      'label' => array(
        'format' => 'above',
      ),
      'teaser' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'icalreference',
    'required' => '1',
    'multiple' => '0',
    'module' => 'icalreferencefield',
    'active' => '1',
    'widget' => array(
      'default_value' => array(
        '0' => array(
          'nid' => '',
          'field' => '',
          'property_id' => '',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'iCal Reference',
      'weight' => '2',
      'description' => '',
      'type' => 'icalreference_text',
      'module' => 'icalreferencefield',
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Description');
  t('Event Date');
  t('Event Status');
  t('iCal');
  t('iCal Reference');

  return $fields;
}
