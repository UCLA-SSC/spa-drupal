<?php
// $Id: content_handler_filter_many_to_one.inc,v 1.1.2.5 2009/08/14 19:15:10 markuspetrux Exp $

/**
 * @file
 * The subclass simply adds properties,
 * for field-specific subclasses to use if they need to.
 */
class content_handler_filter_many_to_one extends views_handler_filter_many_to_one {
  var $content_field;
  var $can_autocomplete = FALSE;

  function construct() {
    parent::construct();
    $this->content_field = content_fields($this->definition['content_field_name']);
    $this->additional_fields = $this->definition['additional fields'];
    $field = $this->content_field;
    $this->value_title = $field['widget']['label'];
    $this->can_autocomplete = in_array($field['module'], array('nodereference', 'userreference'));
  }

  function get_value_options() {
    if (empty($this->can_autocomplete) || $this->options['type'] == 'select') {
      $this->value_options = $this->allowed_values();
    }
    else {
      /* don't overwrite the value options */
    }
  }

  // Get allowed values from hook_allowed_values(), if any,
  // or from content_allowed_values();
  function allowed_values() {
    $field = $this->content_field;
    $function = $field['module'] .'_allowed_values';
    if ($this->value_form_type == 'select') {
      // Select elements accept multidimensional arrays to support optgroups.
      $options = function_exists($function) ? $function($field) : content_allowed_values($field, FALSE);
      // For selects, HTML should be filtered out and entities left unencoded.
      // See content_allowed_values / content_filter_xss / filter_xss.
      content_allowed_values_filter_html($options);
    }
    else {
      $options = function_exists($function) ? $function($field) : content_allowed_values($field);
    }
    return (array) $options;
  }

  function has_extra_options() { return $this->can_autocomplete; }

  function option_definition() {
    $options = parent::option_definition();

    $options['type'] = array('default' => 'select');

    return $options;
  }

  function extra_options_form(&$form, &$form_state) {
    $form['type'] = array(
      '#type' => 'radios',
      '#title' => t('Selection type'),
      '#options' => array('select' => t('Dropdown'), 'textfield' => t('Autocomplete')),
      '#default_value' => $this->options['type'],
    );
  }

  function value_form(&$form, &$form_state) {
    if ($this->options['type'] == 'textfield') {
      $form['value'] = array(
        '#tree' => TRUE,
        '#title' => t('@widget-label', array('@widget-label' => $this->value_title)),
        '#type' => 'nodereference_autocomplete',
        '#views' => TRUE,
        '#views_single' => $this->options['expose']['single'],
        '#default_value' => $this->value ? array('name' => $this->value) : array(),
        '#field_name' => $this->content_field['field_name'],
        '#type_name' => $this->content_field['type_name'],
        '#value_callback' => 'nodereference_autocomplete_value',
      );

      if (empty($form_state['exposed'])) {
        // Retain the helper option
        $this->helper->options_form($form, $form_state);
      }
    }
    else {
      parent::value_form($form, $form_state);
    }
  }

  function value_validate(&$form, &$form_state) {
    // We only validate if they've chosen the text field style.
    if ($this->options['type'] != 'textfield') {
      return;
    }

    $form_state['values']['options']['value'] = $form_state['values']['options']['value']['name'];
  }

  function accept_exposed_input($input) {
    if (empty($this->options['exposed'])) {
      return TRUE;
    }

    // If it's optional and there's no value don't bother filtering.
    if ($this->options['expose']['optional'] && empty($this->validated_exposed_input)) {
      return FALSE;
    }

    $rc = parent::accept_exposed_input($input);
    if ($rc) {
      // If we have previously validated input, override.
      if (isset($this->validated_exposed_input)) {
        $this->value = $this->validated_exposed_input;
      }
    }

    return $rc;
  }

  function exposed_validate(&$form, &$form_state) {
    if (empty($this->options['exposed'])) {
      return;
    }

    $identifier = $this->options['expose']['identifier'];

    // We only validate if they've chosen the text field style.
    if ($this->options['type'] != 'textfield') {
      if ($form_state['values'][$identifier] != 'All')  {
        $this->validated_exposed_input = (array) $form_state['values'][$identifier];
      }
      return;
    }

    if (empty($this->options['expose']['identifier'])) {
      return;
    }

    $this->validated_exposed_input = $form_state['values'][$identifier]['name'];
  }

  function value_submit($form, &$form_state) {
    // prevent array_filter from messing up our arrays in parent submit.
  }

  function expose_form_right(&$form, &$form_state) {
    parent::expose_form_right($form, $form_state);
    if ($this->options['type'] != 'select') {
      unset($form['expose']['reduce']);
    }
  }

  function admin_summary() {
    // set up $this->value_options for the parent summary
    $this->value_options = array();

    if ($this->value) {
      if ($this->content_field['module'] == 'nodereference') {
        $result = db_query(db_rewrite_sql('SELECT n.nid, n.title FROM {node} n WHERE n.nid IN (' . implode(', ', $this->value) . ')'));
      }
      elseif ($this->content_field['module'] == 'userreference') {
        $result = db_query('SELECT u.uid, u.name FROM {user} u WHERE u.uid IN (' . implode(', ', $this->value) . ')');
      }

      while ($node = db_fetch_object($result)) {
        $this->value_options[$node->nid] = $node->title .'[nid:'. $node->nid .']';
      }
    }
    return parent::admin_summary();
  }

}
