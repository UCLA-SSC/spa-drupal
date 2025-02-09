<?php

/**
 * @file total_control.views_default.inc
 *
 * Default page.
 *
 */

/**
 * Implementation of hook_views_default_views().
 */
function total_control_views_default_views() {
  $files = file_scan_directory(drupal_get_path('module', 'total_control') . '/views', '.view');
  foreach ($files as $filepath => $file) {
    require $filepath;
    
    // Don't load the view if the module providing data is not enabled.
    if ( ($file->name == 'control_activity' && !module_exists('statistics')) 
      || ($file->name == 'control_comments' && !module_exists('comment'))
      || ($file->name == 'control_terms' && !module_exists('taxonomy')) 
      ) {
      continue;
    }
    else {
      if (isset($view)) {
        $views[$view->name] = $view;
      }
    }
    
  }
  return $views;
}