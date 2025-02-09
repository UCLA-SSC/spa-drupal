<?php
// $Id: nodereference_explorer.views.inc,v 1.5 2009/08/21 07:37:43 gnindl Exp $
/**
 * @file
 * nodereference_explorer.views.inc
 * Views related hooks.
 * It contains currently the views plugin style and
 * row definition, as well a the default view responsible
 * for the referenced content types option of the
 * nodereference module.
 */

/**
 * Implementation of hook_views_plugin().
 * Definition of style and row plugins for views
 */
function nodereference_explorer_views_plugins() {
  return array(
    'style' => array( //declare the views_json style plugin
      'table_selectable' => array(
        'path' => drupal_get_path('module', 'views') . '/plugins',
        'title' => t('Table (Selectable)'),
        'theme' => 'views_view_table',
        'theme path' => drupal_get_path('module', 'views') . '/theme',
        'theme file' => 'theme.inc',
        'help' => t('Displays and wraps a table with selectable rows.'),
        'handler' => 'views_plugin_style_table',
        'uses row plugin' => FALSE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
      ),
    ),
    'row' => array( //declare the unformatted row plugin
      'fields_selectable' => array(
        'path' => drupal_get_path('module', 'views') . '/plugins',
        'title' => t('Fields (Selectable)'),
        'help' => t('(Displays and wraps each field in a selectable row.'),
        'handler' => 'views_plugin_row_fields',
        'theme' => 'nodereference_explorer_view_row_fields_selectable',
        'theme path' => NODE_REFERENCE_EXPLORER_MODULE_PATH . '/theme',
        'theme file' => 'theme.inc',
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
      ),
    )
  );
}

/**
 * Wrap views table plugin rows into selectable ones.
 * Preprocess hook is called before view is rendered
 * @param $vars
 * 	theming variables
 */
function nodereference_explorer_preprocess_views_view_table(&$vars) {
  if ($vars['view']->display_handler->options['style_plugin'] == 'table_selectable' ||
      $vars['view']->display_handler->view->plugin_name == 'table_selectable') {
    foreach ($vars['rows'] as $id => &$row) {
      $vars['row_classes'][$id][] = 'views-item-selectable';
      //get node id for each row, retrieve from result if nid is hidden
      $nid = isset($row['nid']) ? $row['nid'] : $vars['view']->result[$id]->nid;
      $element = array( //wrap id table fields with radio buttons
        '#type' => 'radio',
        '#id' => 'nodereference-explorer-row-'. $id,
        '#name' => 'nodereference-explorer-row',
        '#return_value' => $row['title'] . ' [nid:'. $nid .']',
      );
      $row['nid'] .= drupal_render($element);
    }
  }
}

/**
 * Implementation of hook_views_default_views().
 *
 * Default view used when referencing content types. Instead of accessing
 * the database directly, this view is assigned to do the work now. Additionally
 * any views can be placed here which are exported (better performance and versioning).
 * @return default views
 */
function nodereference_explorer_views_default_views() {
  $views = array();

  /*
   * View 'nodereference_explorer_default'
   */
  $view = new view;
  $view->name = 'nodereference_explorer_default';
  $view->description = 'Generic nodereference explorer view including three default displays (table, thumbnails and list';
  $view->tag = '';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  //default display
  $handler = $view->new_display('default', 'Defaults', 'default');

  //set default fields for the view
  $handler->override_option('fields', array(
    'nid' => array(
      'label' => t('Node Id'),
      'link_to_node' => 0,
      'exclude' => 0,
      'id' => 'nid',
      'table' => 'node',
      'field' => 'nid',
      'relationship' => 'none',
    ),
    'title' => array(
      'label' => t('Title'),
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
    ),
    'type' => array(
      'label' => t('Type'),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
    ),
    'changed' => array(
      'label' => t('Last changed date'),
      'date_format' => 'small',
      'custom_date_format' => '',
      'exclude' => 0,
      'id' => 'changed',
      'table' => 'node',
      'field' => 'changed',
      'relationship' => 'none',
    ),
    'name' => array(
      'label' => t('User'),
      'id' => 'name',
      'table' => 'users',
      'field' => 'name',
    ),
    'status' => array(
      'label' => t('Published'),
      'id' => 'status',
      'table' => 'node',
      'field' => 'status',
    ),
  ));

  $handler->override_option('arguments', array(
  'nid' => array(
    'default_action' => 'ignore',
    'style_plugin' => 'default_summary',
    'style_options' => array(),
    'wildcard' => 'all',
    'wildcard_substitution' => 'All',
    'title' => '',
    'breadcrumb' => '',
    'default_argument_type' => 'fixed',
    'default_argument' => '',
    'validate_type' => 'none',
    'validate_fail' => 'not found',
    'break_phrase' => 1,
    'not' => 0,
    'id' => 'nid',
    'table' => 'node',
    'field' => 'nid',
    'validate_user_argument_type' => 'uid',
    'validate_user_roles' => array(),
    'relationship' => 'none',
    'default_options_div_prefix' => '',
    'default_argument_user' => 0,
    'default_argument_fixed' => '',
    'default_argument_php' => '',
    'validate_argument_node_type' => array(),
    'validate_argument_node_access' => 0,
    'validate_argument_nid_type' => 'nid',
    'validate_argument_vocabulary' => array(),
    'validate_argument_type' => 'tid',
    'validate_argument_transform' => 0,
    'validate_user_restrict_roles' => 0,
    'validate_argument_php' => '',
  ),
));
   nodereference_explorer_add_inc('includes/nodereference_explorer.util');
  $handler->override_option('filters', array(
    'type' => array(
      'operator' => 'in',
      //set all node types as default, underlying nodereference field has to care about narrowing filter options
      'value' => nodereference_explorer_util_get_all_referenceable_node_types(),
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'type_op',
        'identifier' => 'type',
        'label' => t('Node: Type'),
        'optional' => 1,
        'single' => 1,
        'remember' => 0,
        'reduce' => 1,
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'title' => array(
      'operator' => 'contains',
      'value' => '',
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 1,
        'operator' => 'title_op',
        'identifier' => 'title',
        'label' => t('Node: Title'),
        'optional' => 1,
        'remember' => 0,
      ),
      'case' => 1,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'changed' => array(
      'operator' => 'between',
      'value' => array(
        'type' => 'date',
        'value' => '',
        'min' => '',
        'max' => '',
      ),
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 1,
        'operator' => 'changed_op',
        'identifier' => 'changed_min',
        'label' => t('Node: Updated date'),
        'optional' => 1,
        'remember' => 0,
      ),
      'id' => 'changed',
      'table' => 'node',
      'field' => 'changed',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
  ));

  $handler->override_option('access', array(
    'type' => 'none',
  ));

  $handler->override_option('header_format', '1');
  $handler->override_option('header_empty', 0);
  $handler->override_option('use_ajax', TRUE);
  $handler->override_option('items_per_page', 6);
  $handler->override_option('use_pager', '1');
  $handler->override_option('style_plugin', 'table_selectable');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 1,
    'order' => 'desc',
    'columns' => array(
      'nid' => 'nid',
      'title' => 'title',
      'type' => 'type',
      'changed' => 'changed',
      'name' => 'name',
      'status' => 'status',
    ),
    'info' => array(
      'nid' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'title' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'type' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'changed' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'name' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'status' => array(
        'sortable' => 1,
        'separator' => '',
      ),
    ),
    'default' => 'changed',
  ));

  //table display
  $handler = $view->new_display('page', 'Table', 'table');
  $handler->override_option('style_plugin', 'table_selectable');
  $handler->override_option('path', 'nodereference_explorer/menu_display_table');
  $handler->override_option('menu', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
  ));

  //grid display
  $handler = $view->new_display('page', 'Grid', 'grid');
  $handler->override_option('style_plugin', 'grid');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'columns' => '6',
    'alignment' => 'horizontal',
  ));
  $handler->override_option('row_plugin', 'fields_selectable');
  $handler->override_option('row_options', array(
    'inline' => array(),
    'separator' => '',
  ));
  $handler->override_option('path', 'nodereference_explorer/menu_display_thumbnail');
  $handler->override_option('menu', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
  ));

  //list display
  $handler = $view->new_display('page', 'List', 'list');
  $handler->override_option('style_plugin', 'list');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'type' => 'ul',
  ));
  $handler->override_option('row_plugin', 'fields_selectable');
  $handler->override_option('row_options', array(
    'inline' => array(
      'nid' => 'nid',
      'title' => 'title',
      'type' => 'type',
      'changed' => 'changed',
      'name' => 'name',
      'status' => 'status',
    ),
    'separator' => '',
  ));

  $handler->override_option('path', 'nodereference_explorer/menu_display_list');
  $handler->override_option('menu', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));

  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
  ));

  //preview display
  $handler = $view->new_display('page', 'Preview', 'preview');
  $handler->override_option('exposed_block', TRUE);

  $handler->override_option('arguments', array(
  'nid' => array(
    'default_action' => 'empty',
    'style_plugin' => 'default_summary',
    'style_options' => array(),
    'wildcard' => 'all',
    'wildcard_substitution' => 'All',
    'title' => '',
    'breadcrumb' => '',
    'default_argument_type' => 'fixed',
    'default_argument' => '',
    'validate_type' => 'none',
    'validate_fail' => 'not found',
    'break_phrase' => 0,
    'not' => 0,
    'id' => 'nid',
    'table' => 'node',
    'field' => 'nid',
    'validate_user_argument_type' => 'uid',
    'validate_user_roles' => array(
      '2' => 0,
      '3' => 0,
    ),
    'relationship' => 'none',
    'default_options_div_prefix' => '',
    'default_argument_user' => 0,
    'default_argument_fixed' => '',
    'default_argument_php' => '',
    'validate_argument_node_type' => array(
      'artist' => 0,
      'contact' => 0,
      'document' => 0,
      'election' => 0,
      'event' => 0,
      'event_page' => 0,
      'feature' => 0,
      'image' => 0,
      'multimedia' => 0,
      'multimedia_gallery' => 0,
      'music' => 0,
      'news' => 0,
      'page' => 0,
      'photo' => 0,
      'photo_gallery' => 0,
      'presentation' => 0,
      'press_release' => 0,
      'session' => 0,
      'story' => 0,
      'test' => 0,
      'video' => 0,
    ),
    'validate_argument_node_access' => 0,
    'validate_argument_nid_type' => 'nid',
    'validate_argument_vocabulary' => array(
      '3' => 0,
      '1' => 0,
      '4' => 0,
      '2' => 0,
    ),
    'validate_argument_type' => 'tid',
    'validate_argument_transform' => 0,
    'validate_user_restrict_roles' => 0,
    'validate_argument_php' => '',
    'override' => array(
      'button' => 'Use default',
    ),
  ),
));
  $handler->override_option('style_plugin', 'default');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'columns' => '6',
    'alignment' => 'horizontal',
  ));
  $handler->override_option('row_plugin', 'fields');
  $handler->override_option('row_options', array(
    'inline' => array(),
    'separator' => '',
  ));
  $handler->override_option('path', 'nodereference_explorer/menu_display_preview');
  $handler->override_option('menu', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
  ));


  $views[$view->name] = $view;
  return $views;
}