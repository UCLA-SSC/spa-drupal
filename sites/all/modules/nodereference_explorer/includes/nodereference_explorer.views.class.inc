<?php
// $Id: nodereference_explorer.views.class.inc,v 1.5 2009/07/28 15:14:18 gnindl Exp $
/**
 * @file
 * nodereference_explorer.views.inc
 * Class responsible for widget and dialog initialization
 * TODO make this class responsible for views instantiation and execution only.
 * TODO leverage view encapsulation to support filters on node types in field
 */
class nodereference_explorer_views {

  public $view;  //referenced view
  public $view_args; //view arguments
  public $field; //referenced field

  private $selection; // the string that identifies the selection

  /**
   * @return unknown
   */
  public function get_selection() {
    return $this->selection;
  }

  /**
   * @param unknown_type $view_name
   */
  public function set_selection($selection) {
    $this->selection = $selection;
  }

  /**
   * Initialize the handler (ids, options, field)
   * @param $field
   *   cck field
   */
  public function __construct($field) {
    $this->field  = $field;

    //default view
    if (empty($this->field['advanced_view']) || $this->field['advanced_view'] == '--') {
      //pretend the field to be a view (for function _nodereference_potential_references())
      $this->field['advanced_view'] = NODE_REFERENCE_EXPLORER_DEFAULT_VIEW_NAME;

      $this->view = views_get_view(NODE_REFERENCE_EXPLORER_DEFAULT_VIEW_NAME);
      //get possible references to content types
      $node_ids = array_keys(_nodereference_potential_references_standard($this->field));
      //set node ids as view arguments
      $this->view_args = implode(',', $node_ids);
    }
    else {
      $this->view = views_get_view($this->field['advanced_view']);
    }

    // Get arguments for the view.
    if (!empty($this->field['advanced_view_args'])) {
      if (!empty($this->view_args)) //append to existing arguments
         $this->view_args .= ',';
      $this->view_args .= explode(',', $this->field['advanced_view_args']);

    }
  }

  /**
   * Get a specific view item by node ids
   * @param $value node id or form state value of field
   * @return rendered view item
   */
  public function get_item($value) {
    if (is_array($value) && isset($value['nid'])) {
      // taken from nodereference_autocomplete_validate() in nodereference.module
      preg_match('/^(?:\s*|(.*) )?\[\s*nid\s*:\s*(\d+)\s*\]$/', $value['nid'], $matches);
      if (!empty($matches)) {
        // Explicit [nid:n].
        $nid = $matches[2];
      }
    }
    elseif (!is_array($value)) {
      $nid = $value;
    }
    //$match = isset($this->field['widget']['autocomplete_match']) ? $this->field['widget']['autocomplete_match'] : 'contains';
    $output = views_embed_view($this->view->name, $this->field['widget']['content_preview'], $nid);
  if (!is_array($output)) //return rendered output for pages
      return $output;
    else { //if an array then output rendered for blocks
      return $output['content'];
    }
  }

  /**
   * Get a filter form for a display
   * @param $display_id display identifier
   * @return rendered filter form
   */
  public function get_exposed_form($display_id) {
    $view = $this->view->clone_view();
    $view->set_display($display_id);
    $view->init_handlers(); //initialize display handlers
    $form_state = array(
      'view' => $view,
      'display' => $view->display_handler->display,
      'method' => 'get',
      'rerender' => TRUE,
      'no_redirect' => TRUE,
    );
    $form = drupal_build_form('views_exposed_form', $form_state); //create the filter form
    return $form;
  }

  /**
   * Get all available displays of the assigend
   * view.
   * @return array of display objects
   */
  public function get_all_displays() {
    $displays = $this->view->display;

    if (!empty($this->field['widget']['content_preview']) && $this->field['widget']['content_preview'] != '--')
      unset($displays[$this->field['widget']['content_preview']]); //hide preview in dialog window
    else {
      unset($displays['preview']);
    }
    return $displays; //get all available displays
  }

  /**
   * Get a display by id.
   * @param $display_id display identifier
   * @return rendered display
   */
  public function get_display($display_id) {
    $view = $this->view->clone_view();
    $view->set_display($display_id); //set display, e. g. thumbnails or table

    $title = drupal_get_title();

    if (isset($this->view_args)) {//limit out be node ids
      $output = views_embed_view($view->name, $display_id, $this->view_args);
    }
    else {
      $output = views_embed_view($view->name, $display_id); //render the display
    }
    drupal_set_title($title);
    if (!is_array($output)) //return rendered output for pages
      return $output;
    else { //if an array then output rendered for blocks
      return $output['content'];
    }
  }
}