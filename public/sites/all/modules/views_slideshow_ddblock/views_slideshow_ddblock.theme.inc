<?php
// $Id: views_slideshow_ddblock.theme.inc,v 1.1.2.2 2009/09/02 08:21:52 ppblaauw Exp $

/**
 *  @file
 *  Theme & preprocess functions for the Views Slideshow: ddblock module.
 */

/**
 *  content preprocess function.
 */
function template_preprocess_views_slideshow_ddblock(&$vars) {
  // simplify variable names
  $options = $vars['options']['ddblock'];
  
  // DDblock template used
  if ($options['template'] <> 'none') {
    $view = $vars['view'];
  
    // get view result
    $rows = $view->result;
    $rows1 = &$view->field;
    
    // sort the result using ddblock, none for using order of view
    if (!empty($rows)) {
      switch ($options['settings']['order']) {
        case 'random':
          shuffle($rows);
          break;
        case 'asc' :
          asort($rows);
          break;
        case 'desc':
          rsort($rows);
          break;
        case 'none':
          break;
      }

      // set jquery cycle settings
      $settings['viewsSlideshowDdblockContent'][$vars['id']] = array(
      'block'                      => $vars['id'],
      'setDimensions'              => $options['template'],
      'contentContainer'           => $options['content_container']['container'],
      'custom'                     => $options['custom']['custom_jquery'],
      'fx'                         => $options['settings']['fx'],
      'speed'                      => $options['settings']['speed'],
      'timeOut'                    => $options['settings']['timeout'],
      'pause'                      => $options['settings']['pause'],
      'next'                       => $options['settings']['next'],
      'overflow'                   => $options['settings']['overflow'],
      'pager'                      => $options['settings']['pager_settings']['pager'],
      'pagerContainer'             => $options['settings']['pager_settings']['pager_container'],
      'pagerEvent'                 => $options['settings']['pager_settings']['pager_event'],
      'pagerFast'                  => $options['settings']['pager_settings']['pager_fast'],
      'pagerPause'                 => $options['settings']['pager_settings']['pager_pause'],
      'pager2'                     => $options['settings']['pager2'],
      'pager2Event'                => $options['settings']['pager2_settings']['pager2_event'],
      'pager2PagerHide'            => $options['settings']['pager2_settings']['pager2_pager']['pager2_pager_hide'],
      'pager2SlideHide'            => $options['settings']['pager2_settings']['pager2_slide']['pager2_slide_hide'],
      'slideText'                  => $options['settings']['slide_text'],
      'slideTextjQuery'            => $options['settings']['slide_text_settings']['slide_text_jquery'],
      'slideTextPosition'          => $options['settings']['slide_text_settings']['slide_text_position'],
      'slideTextContainer'         => $options['settings']['slide_text_settings']['slide_text_container'],
      'slideTextEffectBefore'      => $options['settings']['slide_text_settings']['slide_jquery']['slide_text_before_effect'],
      'slideTextEffectBeforeSpeed' => $options['settings']['slide_text_settings']['slide_jquery']['slide_text_before_speed'],
      'slideTextEffectAfter'       => $options['settings']['slide_text_settings']['slide_jquery']['slide_text_after_effect'],
      'slideTextEffectAfterSpeed'  => $options['settings']['slide_text_settings']['slide_jquery']['slide_text_after_speed'],
      );
    
      // set template variables
      $vars['views_slideshow_ddblock_slider_settings']['delta']               = $vars['id'];
      $vars['views_slideshow_ddblock_slider_settings']['debug_info']          = $options['debug_info'];
      $vars['views_slideshow_ddblock_slider_settings']['output_type']         = 'view_fields';
      $vars['views_slideshow_ddblock_slider_settings']['pager']               = $options['settings']['pager_settings']['pager'];
      $vars['views_slideshow_ddblock_slider_settings']['pager_position']      = $options['settings']['pager_settings']['pager_position'];
      $vars['views_slideshow_ddblock_slider_settings']['pager2']              = $options['settings']['pager2'];
      $vars['views_slideshow_ddblock_slider_settings']['pager2_position']     = $options['settings']['pager2_settings']['pager2_position'];
      $vars['views_slideshow_ddblock_slider_settings']['pager2_pager_prev']   = $options['settings']['pager2_settings']['pager2_pager']['pager2_pager_prev'];
      $vars['views_slideshow_ddblock_slider_settings']['pager2_pager_next']   = $options['settings']['pager2_settings']['pager2_pager']['pager2_pager_next'];
      $vars['views_slideshow_ddblock_slider_settings']['pager2_slide_prev']   = $options['settings']['pager2_settings']['pager2_slide']['pager2_slide_prev'];
      $vars['views_slideshow_ddblock_slider_settings']['pager2_slide_next']   = $options['settings']['pager2_settings']['pager2_slide']['pager2_slide_next'];
      $vars['views_slideshow_ddblock_slider_settings']['slide_text']          = $options['settings']['slide_text'];
      $vars['views_slideshow_ddblock_slider_settings']['slide_text_position'] = $options['settings']['slide_text_settings']['slide_text_position'];
      $vars['views_slideshow_ddblock_slider_settings']['view_name']           = $view->name;
      $vars['views_slideshow_ddblock_slider_settings']['view_display_id']     = $view->current_display;
      
      
      if ($options['imgcache_toggle'] == 1) {
        $vars['views_slideshow_ddblock_slider_settings']['imgcache_toggle']     = $options['imgcache']['imgcache_toggle'];
        $vars['views_slideshow_ddblock_slider_settings']['imgcache_pager_item'] = $options['imgcache']['imgcache_pager_item'];
        $vars['views_slideshow_ddblock_slider_settings']['imgcache_slide']      = $options['imgcache']['imgcache_slide'];
      }

      if ($vars['views_slideshow_ddblock_slider_settings']['slide_text_position'] == "top" || 
          $vars['views_slideshow_ddblock_slider_settings']['slide_text_position'] == "bottom") {
        $vars['views_slideshow_ddblock_slider_settings']['slide_direction'] = "horizontal";
      }
      else {
        $vars['views_slideshow_ddblock_slider_settings']['slide_direction'] = "vertical";
      }
      $vars['views_slideshow_ddblock_slider_settings']['template'] = $options['template'];
      if ($vars['views_slideshow_ddblock_slider_settings']['template'] == 'custom' ) {
        $vars['views_slideshow_ddblock_slider_settings']['custom_template'] = $options['custom_template'];
      }
      
      $vars['views_slideshow_ddblock_content'] = $rows;
    
      // Add the required JS and CSS.
      // get module path to views_slideshow_ddblock module.
      $path = drupal_get_path('module', 'views_slideshow_ddblock');

      //add jcycle plugin
      drupal_add_js($path .'/js/jquery.cycle.all.min.js', 'module');

      //add easing plugin
      //drupal_add_js($path .'/js/jquery.easing.1.1.1.js', 'module');

      // add ddblock js file
      drupal_add_js($path .'/js/json2.pack.js', 'module');

      // add ddblock js file
      drupal_add_js($path .'/js/views_slideshow_ddblock.js', 'module');

      // add Cascading style sheet
      //drupal_add_css($path .'/views_slideshow_ddblock.css', 'module', 'all', TRUE);

      drupal_add_js($settings, 'setting');
    
      // additional candidate template files
      if ($options['template'] == 'custom') {
        $vars['template_files'][] = 'views-slideshow-ddblock-cycle-block-content-'. $options['custom_template'];
        $vars['template_files'][] = 'views-slideshow-ddblock-cycle-block-content-'. $vars['id'];
      }
      else {
        $vars['template_files'][] = 'views-slideshow-ddblock-cycle-block-content-'. $options['template'];
        $vars['template_files'][] = 'views-slideshow-ddblock-cycle-block-content-'. $vars['id'];
      }
    
      // pager content settings
      $pager_settings['delta']               = $vars['id'];
      $pager_settings['debug_info']          = $options['debug_info'];
      $pager_settings['output_type']         = 'view_fields';
      $pager_settings['pager']               = $options['settings']['pager_settings']['pager'];
      $pager_settings['pager_container']     = $options['settings']['pager_settings']['pager_container'];
      $pager_settings['pager_event']         = $options['settings']['pager_settings']['pager_event'];
      $pager_settings['pager_position']      = $options['settings']['pager_settings']['pager_position'];
      $pager_settings['pager2']              = $options['settings']['pager2'];
      $pager_settings['pager2_position']     = $options['settings']['pager2_settings']['pager2_position'];
      $pager_settings['pager2_pager_prev']   = $options['settings']['pager2_settings']['pager2_pager']['pager2_pager_prev'];
      $pager_settings['pager2_pager_next']   = $options['settings']['pager2_settings']['pager2_pager']['pager2_pager_next'];
      $pager_settings['pager2_slide_prev']   = $options['settings']['pager2_settings']['pager2_slide']['pager2_slide_prev'];
      $pager_settings['pager2_slide_next']   = $options['settings']['pager2_settings']['pager2_slide']['pager2_slide_next'];
      $pager_settings['template']            = $options['template'];
      $pager_settings['custom_template']     = $options['custom_template'];
      $pager_settings['view_name']           = $view->name;
      $pager_settings['view_display_id']     = $view->current_display;

      if ($options['imgcache_toggle'] == 1) {
        $pager_settings['imgcache_pager_item'] = $options['imgcache']['imgcache_pager_item'];
      }
      
      $vars['views_slideshow_ddblock_pager_content'] = theme(
        'views_slideshow_ddblock_pager_content',
        $pager_settings,
        $vars['views_slideshow_ddblock_content']
      );
    }
  }  
}
/**
 *  Pager-items preprocess function.
 */
function template_preprocess_views_slideshow_ddblock_pager_content(&$vars) {
  if (!empty($vars['views_slideshow_ddblock_content'])) {

    //simplify variable
    $settings = $vars['views_slideshow_ddblock_pager_settings'];

    // additional candidate template files
    if ($settings['template'] == 'custom') {
      $vars['template_files'][] = 'views-slideshow-ddblock-cycle-pager-content-'. $settings['custom_template'];
      $vars['template_files'][] = 'views-slideshow-ddblock-cycle-pager-content-'. $settings['delta'];
    }
    else {
      $vars['template_files'][] = 'views-slideshow-ddblock-cycle-pager-content-'. $settings['template'];
      $vars['template_files'][] = 'views-slideshow-ddblock-cycle-pager-content-'. $settings['delta'];
    }
  }
}  
