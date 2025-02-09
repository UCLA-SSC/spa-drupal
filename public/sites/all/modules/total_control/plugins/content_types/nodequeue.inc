<?php

/**
 * @file nodequeue.inc
 *
 * "Nodequeue" content type. It shows users with permissions all of
 * the nodequeues and provides links directly manage them.
 *
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Admin - NodeQueues'),
  'description' => t('Provides links to administer NodeQueues.'),
  // 'single' => TRUE means has no subtypes.
  'single' => TRUE,
  // Constructor.
  'content_types' => array('nodequeue_content_type'),
  // Name of a function which will render the block.
  'render callback' => 'nodequeue_content_type_render',
  // The default context.
  'defaults' => array(),
  // This explicitly declares the config form. Without this line, the func would be
  // ctools_plugin_example_nodequeue_content_type_edit_form.
  'edit form' => 'nodequeue_content_type_edit_form',
  // Icon goes in the directory with the content type.
  'icon' => 'icon_node_form.png',
  'category' => t('Total Control'),
);

/**
 * 'Admin title' callback for the content type.
 */
function total_control_nodequeue_content_type_admin_title($subtype, $conf, $context) {
  return t('Administer NodeQueues');
}

/**
 * 'Admin info' callback for the content type.
 */
function total_control_nodequeue_content_type_admin_info($subtype, $conf, $context) {
  $block = new stdClass();
  $block->title = t('Provides links to administer NodeQueues.');

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
function nodequeue_content_type_render($subtype, $conf, $args, $context) {
  $items = array();

  //Check for NodeQueue module.
  if (module_exists('nodequeue')) {
    // Get nodequeues.
    module_load_include('module', 'nodequeue', 'nodequeue');
    $queues = nodequeue_load_queues(nodequeue_get_all_qids(25));
    
    foreach ($queues as $queue) {
      if (!nodequeue_queue_access($queue)) {
        unset($queues[$queue->qid]);
      }
    }

    $options = array('query' => array('destination' => 'admin/dashboard'));
    $header = array(
      array('data' => t('Title'), 'field' => 'title', 'sort' => 'asc'),
      array('data' => t('Max'), 'field' => 'size'),
      array('data' => t('Operations')),
    );
    $rows = array();

    foreach ($queues as $queue) {
      $operations = array(l(t('Edit'), "admin/content/nodequeue/$queue->qid/view", $options));

      $rows[] = array(
        array('class' => 'nodequeue-title', 'data' => check_plain($queue->title)),
        array('class' => 'nodequeue-max-nodes', 'data' => $queue->size == 0 ? t('Infinite') : $queue->size),
        array('class' => 'nodequeue-operation', 'data' => implode(' | ', $operations)),
      );
    }
  }

  if (empty($rows)) {
    $rows[] = array(
      array(
        'data' => t('There are no nodequeues to display.'),
        'colspan' => 3,
      )
    );
  }

  if (user_access('administer nodequeue')) {
    $link = l(t('Nodequeue administration'), 'admin/content/nodequeue');
  }

  $block = new stdClass();
  $block->module = t('total_control');
  $block->title = t('Administer NodeQueues');
  $block->content = theme('total_control_admin_table', $header, $rows, $link);

  return $block;
}

/**
 * 'Edit form' callback for the content type.
 *
 * ctools_plugin_example_nodequeue_content_type_edit_form.
 *
 */
function nodequeue_content_type_edit_form(&$form, &$form_state) {
  $conf = $form_state['conf'];

  return $form;
}

/**
 * 'Edit form' submit callback for the content type.
 *
 * ctools_plugin_example_total_control_panel_pages_content_type_edit_form_submit.
 * The submit form stores the data in $conf.
 *
 */
function nodequeue_content_type_edit_form_submit(&$form, &$form_state) {
  foreach (array('item1', 'item2') as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}