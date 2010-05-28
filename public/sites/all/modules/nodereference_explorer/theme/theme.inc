<?php
// $Id: theme.inc,v 1.4 2009/07/23 14:52:13 gnindl Exp $
/**
 * @file
 * theme.inc
 * Template preprocess function for preparing necessary template variables and doing
 * theme related tasks.
 */

/**
 * Prepare the dialog template variables and add css,
 * as well as javascript and javascript settings
 * @param $vars template variables
 */
function template_preprocess_nodereference_explorer_dialog(&$vars) {
}

/**
 * Theme the core widget
 * @param $element widget
 * @return rendered representation of autocomplete
 */
function theme_nodereference_explorer($element) {

  $field = content_fields($element['#field_name'], $element['#type_name']);

  //preload the theme due to better performance and working in IE 8
  $theme = nodereference_explorer_util_parse_settings($field, 'theme'); //theme mode
  nodereference_explorer_util_theme_css($theme); //load necessary css files

  //embrace widget with a wrapper
  $output = '<div class="nodereference-explorer">';

  //call plugin specific theming function
  $output .= $element['#plugin']->theme_nodereference_explorer($element);

  $output .= '</div>';
  return $output;
}

/**
 * Theme an action button.
 * @param $element
 *  button
 */
function theme_nodereference_explorer_button($element) {

  nodereference_explorer_add_css('explorer.form'); //theme for core widget (autocomplete)
  views_add_css('views'); //CSS for progress throbbing, exposed filter layout and misc views stuff...

  nodereference_explorer_add_js('explorer.actions'); //base action script for browse and remove

  //Currently it's just a thin wrapper around the theme_button()
  //function which only returns a button of type submit. The themed
  //representation is just adapted to achieve an input html tag
  //of the type button.
  $button = theme('button', $element);
  return str_ireplace('type="submit"', 'type="button"', $button);
}

/**
 * Template for selectable row fields. This is used by grid,
 * list and unformatted view plugins.
 * @param $vars template variables
 */
function template_preprocess_nodereference_explorer_view_row_fields_selectable(&$vars) {
  template_preprocess_views_view_fields($vars);

  static $counter; //count the calls to this method in a request
  if (!isset($counter))
    $counter = 1;

  if ($vars['view']->display_handler->options['row_plugin'] == 'fields_selectable') {

    if (isset($vars['fields']['nid'])) {
      $nid = $vars['fields']['nid']->raw;
    }
    else { //if no node id is rendered, get it from the view result. Note the counter holds the correct is computed by id minus index
      $nid = $vars['view']->result[$vars['id']-$counter]->nid;
      //complement nid attribute as necessary, but do not show
      $vars['fields']['nid']->raw = $nid;
      $vars['fields']['nid']->inline = true;
      $vars['fields']['nid']->inline_html = 'span';
      $vars['fields']['nid']->element_type = 'span';
      $vars['fields']['nid']->class = 'nid';
    }
   $nid = isset($vars['fields']['nid']) ? $vars['fields']['nid']->raw : $vars['view']->result[$vars['id']-$counter]->nid;
    $element = array(
      '#type' => 'radio',
      '#id' => form_clean_id('selectable'),
      '#name' => 'nodereference-explorer-dialog',
      '#return_value' => $vars['fields']['title']->raw . ' [nid:'. $nid .']',
    );
    $vars['fields']['nid']->content .= drupal_render($element);
  }
}