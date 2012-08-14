<?php
// $Id: headerimage-block.tpl.php,v 1.1 2008/04/07 07:46:01 sutharsan Exp $

/**
 * @file headerimage-block.tpl.php
 * Default theme implementation to display the header image block.
 *
 * Available variables:
 * - $node: rendered node object
 * - $unpublished: node is unpublished
 * - $content: node content
 *
 * @see template_preprocess_headerimage_block()
 */
?>

<?php print theme('imagecache', 'lewis_header', $node->field_head_image[0]['filepath'], $node->field_head_image[0]['data']['alt'], $node->field_head_image[0]['data']['title'], NULL); ?>
