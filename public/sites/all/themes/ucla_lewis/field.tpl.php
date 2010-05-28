<?php
$disp = false;
/* temporary fix to hide empty fields:
* only display the field if any of the items don't match the empty value conditions */
foreach ($items as $item) {
  if(!empty($item['view']) && $item['view'] != "<br />\n<br class=\"clear\" />") {
    $disp = true;
  }
}
if ($disp): ?>
<div class="field field-type-<?php print strtr($field_type, '_', '-') ?> field-<?php print strtr($field_name, '_', '-') ?>">
<h3 class="field-label"><?php print $label ?></h3>
<div class="field-items">
<?php foreach ($items as $item) { ?>
<div class="field-item"><?php print $item['view'] ?></div>
<?php } ?>
</div>
</div>
<?php endif; ?>