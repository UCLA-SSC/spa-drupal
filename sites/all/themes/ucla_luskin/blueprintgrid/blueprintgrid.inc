<?php
// $Id: twocol.inc,v 1.1.2.1 2008/12/16 21:27:58 merlinofchaos Exp $

/**
 * Implementation of hook_panels_layouts().
 */
function ucla_luskin_blueprintgrid_panels_layouts() {
  $items['blueprintgrid'] = array(
    'title' => t('Luskin Blueprint Grid'),
    'icon' => 'blueprintgrid.png',
    'theme' => 'panels_blueprintgrid',
    'css' => 'blueprintgrid.css',
    'panels' => array('top' => t('Top'), 'bread' => t('Bread'), 'left' => t('Left side'), 'right' => t('Right side'), 'center' => t('Center'), 'center_left' => t('Center 33 Left'), 'center_mid' => t('Center 33 Mid'), 'center_right' => t('Center 33 Right'), 'center_50_left' => t('Center 50 Left'), 'center_50_right' => t('Center 50 Right')) 
  );

  return $items;
}