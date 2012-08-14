<?php
// $Id: nodereference_explorer.menu.inc,v 1.4 2009/07/23 14:52:12 gnindl Exp $
/**
 * @file
 * nodereference_explorer.menu.inc
 * File containing all menu call backs of
 * the module.
 */

/**
 * Menu call back for showing a view item
 * @param $type_name content type
 * @param $field_name label of the CCK field
 * @param $index ascending number of widget (needed for multiple value per widget)
 * @return rendered view item
 */
function nodereference_explorer_preview($type_name, $field_name) {
  if (!empty($_GET['val'])) {
    preg_match('/^(?:\s*|(.*) )?\[\s*nid\s*:\s*(\d+)\s*\]$/', $_GET['val'], $matches);
    if (!empty($matches)) {
      // Explicit [nid:n].
      list(, $title, $nid) = $matches;
    }
  }
  //retrieve the field by name and content type
  $field = content_fields($field_name, $type_name);

  //get the selected rendered view item
  $handler = new nodereference_explorer_views($field);
  $output = $handler->get_item($nid);
  drupal_json(array('status' => TRUE, 'data' => $output));
}

/**
 * List of available displays for a preview. This is
 * automatically updated in the settings form.
 * @return
 *   JSON string
 */
function nodereference_explorer_preview_displays() {
  if (!empty($_GET['view'])) {
    $view_name = $_GET['view'];
    if ($view_name == '--')
      $view_name = NODE_REFERENCE_EXPLORER_DEFAULT_VIEW_NAME;
    $view = views_get_view($view_name);
    $preview_options = array();
    $preview_options['--'] = ''; //prepend no preview
    $displays = $view->display;
    foreach ($displays as $id => $display) //build optoins array
      $preview_options[$id] = $display->display_title;
    drupal_json(array('status' => TRUE, 'data' => $preview_options));
  }
}

/**
 * Dialog theme layer.
 * A dialog template is applied which is filled by field and widget
 * settings of a CCK field. Third party CCK fields can hook in here
 * and override/supply custom settings.
 * Necessary JS and CSS files are included in the JSON repsone as well.
 * @param $type_name
 * @param $field_name
 * @param $title
 * @return
 *  json string output of dialog html, as well as js and css references.
 */
function nodereference_explorer_dialog($type_name, $field_name, $title = '') {
  /* retrieve the field by name
   * @TODO the responsibility of the theme layer varies based on the dialog API
   * so the theme layer needs to accomodate this difference or be abstract
   * so that it doesn't care. right now, it's just a muddle.
   */
  $field = content_fields($field_name, $type_name);

  //a CCK field is manadatory, return if nothing present
  if (!$field) {
    return '';
  }

  //parse theme settings
  $theme = nodereference_explorer_util_parse_settings($field, 'theme'); //theme mode
  nodereference_explorer_util_theme_css($theme); //load necessary css files

  //parse dialog settings
  $dialog = nodereference_explorer_util_parse_settings($field, 'dialog');
  nodereference_explorer_util_jquery_ui_js($dialog); //add dialog related js files

  //tab lib and css
  nodereference_explorer_add_js('explorer.tabs');
  nodereference_explorer_add_css('explorer.tabs');
  //selection js
  nodereference_explorer_add_js('explorer.selection');
  nodereference_explorer_add_css('explorer.selection');

  //fill template for dialog content
  $handler = new nodereference_explorer_views($field);
  $handler->set_selection($title);
  foreach ($handler->get_all_displays() as $display) {
    $tabs[$display->id] = l($display->display_title, $_GET['q'], array('fragment' => $display->id));
    //$filters[$display->id] = $handler->get_exposed_form($display->id);
    $displays[$display->id] = $handler->get_display($display->id);
  }
  $output = theme('nodereference_explorer_dialog', $tabs, $filters, $displays);

  //difference between dialog api handled here
  switch ($field['widget']['dialog_api']) {
    case 'modalframe':

      modalframe_child_js(); //dialog internal area
      return drupal_get_form('nodereference_explorer_dialog_form', $output, $title); //form of internal area
    case NODE_REFERENCE_EXPLORER_MODULE_NAME:
    default:
      nodereference_explorer_add_js('explorer.dialog');
  }

  $actions = nodereference_explorer_util_parse_settings($field, 'action');
  /* TODO the scripts array contains additional settings and scripts that
   * ideally could inform some process of loading and setting those things.
   * instead, we're calling views_add_js('ajax_view') with the element..
   * $scripts = drupal_add_js(NULL);
   */
   $js = nodereference_explorer_util_get_js();
   //unset($dialog['api']);
   //settings needed by javascript
   $js['setting']['dialog'] = $dialog;
   $js['setting']['actions'] = $actions;

   //dynamically loaded css files
   $css = nodereference_explorer_util_get_css();

   drupal_json(array('status' => TRUE, 'js' => $js, 'css' => $css, 'data' => $output));
}

/**
 * Theme layer for modalframe api.
 * @param $type_name
 * @param $field_name
 */
function nodereference_explorer_modalframe($type_name, $field_name) {
  //TODO: Build abstract theme layer from which modalframe inherits
  $field = content_fields($field_name, $type_name);

  modalframe_parent_js();
  nodereference_explorer_add_js('explorer.modalframe');
  $dialog = nodereference_explorer_util_parse_settings($field, 'dialog');

  $js = nodereference_explorer_util_get_js();

  $css = nodereference_explorer_util_get_css(); //dynamically loaded css files

  $js['setting']['dialog'] = $dialog;

  drupal_json(array('status' => TRUE, 'js' => $js, 'css' => $css));
}

/**
 * Content of the modalframe dialog
 * @param $form_state
 * @param $dialog
 * @param $title
 * @return
 *   a form
 */
function nodereference_explorer_dialog_form($form_state, $dialog, $title) {
  $form['selection'] = array(
    '#type' => 'hidden',
    '#default_value' => $title,
  );
  $form['output'] = array(
    '#value' => $dialog,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );
  return $form;
}

/**
 * Modalframe form submit, i. e. confirmation of the selection
 * @param $form
 * @param $form_state
 */
function nodereference_explorer_dialog_form_submit($form, $form_state) {
  modalframe_close_dialog($form_state['values']); //modalframe is closed
}