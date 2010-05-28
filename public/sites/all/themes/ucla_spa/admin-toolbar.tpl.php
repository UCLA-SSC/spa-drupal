
<div id='admin-toolbar' class='clear-block'>
       


         

  <?php if (!empty($tree_0)): ?>
    <div class='depth-0 clear-block'>
       <?php
          global $user;
       $user_roles = array_keys($user->roles);
     	
           if (in_array(4, $user_roles) || in_array(7, $user_roles) || in_array(8, $user_roles) || in_array(9, $user_roles) ):?>
             <div class="fac-dash"><a href="/admin/dashboard">Admin Dashboard</a></div>
           <?php endif; ?>
      <?php global $user;
     
       $user_roles = array_keys($user->roles);
        if (in_array(5, $user_roles)):?>
          <div class="fac-dash"><a href="/faculty-dashboard">My Dashboard</a></div>
         <?php endif;?> 
      <?php foreach ($tree_0 as $links): ?>
        <?php print $links; ?>
      <?php endforeach; ?>
    </div>
  <?php endif; ?>
 
  <?php if (!empty($tree_1)): ?>
    <div class='depth-1 clear-block <?php if ($collapsed) print 'collapsed' ?>'>
      <span class='close'><?php print t('Close') ?></span>
      <?php foreach ($tree_1 as $links): ?>
        <?php print $links; ?>
      <?php endforeach; ?>
    </div>
  <?php endif; ?>

  <div class='shadow'></div>
</div>
