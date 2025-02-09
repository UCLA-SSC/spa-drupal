<?php

/**
 * @file create.inc
 *
 * "Create content" panels content type. It shows users links to
 * create new content of all types for which they have permissions,
 * and users with "administer content types" permission an optional
 * "configure" link as well
 *
 */
 
/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Create content'),
  'description' => t('Provides links to create new content.'),
  // 'single' => TRUE means has no subtypes.
  'single' => TRUE,
  // Constructor.
  'content_types' => array('total_control_create_content_type'),
  // Name of a function which will render the block.
  'render callback' => 'total_control_create_content_type_render',
  // The default context.
  'defaults' => array('types' => NULL),
  // This explicitly declares the config form. Without this line, the func would be
  // total_control_create_content_type_edit_form.
  'edit form' => 'total_control_create_content_type_edit_form',
  // Icon goes in the directory with the content type.
  'icon' => 'icon_node_form.png',
  'category' => t('Total Control'),
);

/**
 * 'Admin title' callback for the content type.
 *
 * ctools_plugin_example_total_control_panel_pages_content_type_admin_title.
 *
 */
function total_control_create_content_type_admin_title($subtype, $conf, $context) {
  return t('Create content');
}

/**
 * 'Admin info' callback for the content type.
 *
 * ctools_plugin_example_total_control_panel_pages_content_type_admin_info.
 *
 */
function total_control_create_content_type_admin_info($subtype, $conf, $context) {
  $block = new stdClass();
  $block->title = t('Provides links to create new content.');

  return $block;
}

/**
 * Run-time rendering of the body of the block.
 *
 * @param $subtype
 * @param $conf
 *   Configuration as done at admin time.
 * @param $args
 * @param $context
 *   Context - in this case we don't have any.
 *
 * @return
 *   An object with at least title and content members.
 */
function total_control_create_content_type_render($subtype, $conf, $panel_args, &$context) {
  $types = node_get_types('types');
  $create = array();  

  foreach ($types as $type => $object) {
    // compare against type option on pane config
    if ((isset($conf['types']) && $conf['types'][$type]) || ($conf == array())) {
      // check access to create nodes of this type
      if (node_access('create', $type)) { 
        $object_path = str_replace('_', '-', $object->type);
        $create[] = l(t('Add New ' . $object->name), 'node/add/' . $object_path);
      }
    }
  }

  $block = new stdClass();
  $block->module = t('total_control');
  $block->title = t('Create Content');
  $block->content =  theme('total_control_create', $create);

  return $block;
}

/**
 * 'Edit form' callback for the content type.
 *
 * ctools_plugin_example_total_control_panel_pages_content_type_edit_form.
 *
 */
function total_control_create_content_type_edit_form(&$form, &$form_state) {
  $conf = $form_state['conf'];

  $types = node_get_types('types');
  $type_options = array();
  $type_defaults = array();

  if (isset($conf['types'])) {
    $type_defaults = $conf['types'];
  }

  foreach ($types as $type => $object) {
    $type_options[$type] = $object->name;
    if (!array_key_exists($type, $type_defaults)) {
      $type_defaults[$type] = $type;
    }
  }

  $form['types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Include Create links for Content Types'),
    '#options' => $type_options,
    '#default_value' => $type_defaults,
    );

  return $form;
}

/**
 * 'Edit form' submit callback for the content type.
 *
 * ctools_plugin_example_total_control_panel_pages_content_type_edit_form_submit.
 * The submit form stores the data in $conf.
 *
 */
function total_control_create_content_type_edit_form_submit(&$form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  } 
}
