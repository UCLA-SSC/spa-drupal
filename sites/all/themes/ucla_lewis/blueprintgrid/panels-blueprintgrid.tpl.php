<?php
// $Id: panels-twocol.tpl.php,v 1.1.2.1 2008/12/16 21:27:58 merlinofchaos Exp $
/**
 * @file
 * Template for a 2 column panel layout.
 *
 * This template provides a two column panel display layout, with
 * each column roughly equal in width.
 *
 * Variables:
 * - $id: An optional CSS id to use for the layout.
 * - $content: An array of content, each item in the array is keyed to one
 *   panel of the layout. This layout supports the following sections:
 *   - $content['left']: Content in the left column.
 *   - $content['right']: Content in the right column.
 */
?>



<div class="container panel-display clear-block" <?php if (!empty($css_id)) { print "id=\"$css_id\""; } ?>>
  <?php if ($content[top]): ?>
    <div class="span-24 last top-pane panel-top"><?php print $content[top]; ?></div>
  <?php endif; ?>  
  
   <?php if ($content[right]):?>
  <div class="blue-right  span-8 last panel-right <?php if ($breadcrumbshort) { print 'breadup';}?>"><?php print $content[right]; ?></div>
  <?php endif;?>
  <?php if (TRUE):?>
  <div class="<?php if ($content[right]) { print 'span-16'; } else { print 'span-24';}?> panel-bread">
        <?php print $content[bread];    
         $breadcrumb = drupal_get_breadcrumb(); 
        
         /*if (arg(0) == 'faculty-index' && arg(1) ){
          $taxname=ucwords(str_replace('-', ' ', arg(1)));
        
          
           $breadcrumb[1]='Department of '. $taxname;
           $trail=arg(0) .arg(1);
          //print_r(menu_get_active_trail());
           }*/
         if ($breadcrumb != ''  && arg(0) != 'home') {
			 $breadcrumb[] .= '<span class="bread-title"><a href="#">'.drupal_get_title() .'</a></span>';
			 $breadcrumbshort=array_slice($breadcrumb, 1, 4);
			 if ($breadcrumbshort) {
			   foreach ($breadcrumbshort as $crumb) {
				 $crumb=str_replace(' the ', ' <i>the</i> ', $crumb);
				 $crumb=str_replace(' of ', ' <i>of</i> ', $crumb);
				 $breadfix[]=$crumb;
				 }
			 }
			 
 		print '<div class="breadcrumb">'. implode(' <span class="sep">/</span> ',  $breadfix ) .'</div>';
        }?></div>
  <?php endif;?>
  <?php if ($content[left]):?>
  <div class="span-4 panel-left"><?php print $content[left]; ?></div>
  <?php endif;?>
  <?php if ($content[center_left]):?>
  <div class="span-4 panel-center-left"><?php print $content[center_left]; ?></div>
  <div class="span-4 panel-center-mid"><?php print $content[center_mid]; ?></div>
  <div class="span-4 last panel-center-right"><?php print $content[center_right]; ?></div>
  <?php endif;?>
  <?php if ($content[center]):?>
    <div class="<?php if ($content[right] && !$content[center_50_left]  && $content[left] ):?> span-12
                 <?php elseif($content[center_50_left] && !$content[right]): ?>span-20 last
                 <?php elseif ($content[left] && !$content[right]):?> span-20 last
                 <?php elseif (!$content[left] && !$content[right] && !$content[center_50_left]):?> span-24 last
				 <?php elseif (!$content[left] && $content[right] && !$content[center_50_left]):?> span-16
                 <?php else:?>
                  span-16 
                 <?php endif; ?> panel-center"><?php print $content[center]; ?></div>
  <?php endif;?>
   <?php if ($content[center_50_left]):?>
    <div class="<?php if ($content[left]):?>span-10  <?php else: ?> span-8 <?php endif; ?>  panel-center-50-left"><?php print $content[center_50_left]; ?></div>
    <div class="<?php if ($content[left]):?>span-10  last <?php else: ?> span-8 <?php endif; ?> panel-center-50-right "><?php print $content[center_50_right]; ?></div>
  <?php endif;?>
  
</div>