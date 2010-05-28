<?php // $Id: page.tpl.php,v 1.15.4.7 2008/12/23 03:40:02 designerbrent Exp $ ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
	"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>">
<head>
	<title><?php print $head_title ?></title>
	<meta http-equiv="content-language" content="<?php print $language->language ?>" />
	<?php print $meta; ?>
  <?php print $head; ?>
  <?php print $styles; ?>
  <!--[if lte IE 7]>
    <link rel="stylesheet" href="<?php print $path; ?>blueprint/blueprint/ie.css" type="text/css" media="screen, projection">
  	<link href="<?php print $path; ?>css/ie.css" rel="stylesheet"  type="text/css"  media="screen, projection" />
  <![endif]-->  
  <!--[if lte IE 6]>
  	<link href="<?php print $path; ?>css/ie6.css" rel="stylesheet"  type="text/css"  media="screen, projection" />
  <![endif]--> 
  <?php if (!empty($admin)):?>
   <link href="/sites/all/themes/ucla_spa/css/admin.css?2" media="all" rel="stylesheet" type="text/css">
 
  <?php endif; ?>
</head>

<body class="<?php print $body_classes; print arg(1) ?>">
  <?php if (!empty($admin)):?>
   <?php print $admin; ?>
  <?php endif; ?>
<div class="container">
<?php print $skinr; ?>
  <div id="header">
  
    <div id="logo">
      <a href="<?php print url();?>"><img src="<?php print $logo?>"/></a>
    </div>
      <div id="top-right">
      <?php $school=node_load(422);?>
      <a href="<?php print $school->field_linkedin[0]['url']?>"><img src="/<?php print path_to_theme()?>/images/linkedin-big.png"/></a>
      <a href="<?php print $school->field_facebook[0]['url']?>"><img src="/<?php print path_to_theme()?>/images/facebook-big.png"/></a>
      <a href="http://twitter.com/<?php print $school->field_twitter[0]['value']?>"><img src="/<?php print path_to_theme()?>/images/twitter-big.png"/></a>
      <a href="<?php print $school->field_youtube_page[0]['url']?>"><img src="/<?php print path_to_theme()?>/images/youtube-big.png"/></a>
    </div>
    <?php print $header; ?>
    <?php if (isset($primary_links)) : ?>
      <?php print theme('nice_menu_primary_links'); ?>
    <?php endif; ?>
    <?php if (isset($secondary_links)) : ?>
      <?php print theme('links', $secondary_links, array('id' => 'subnav', 'class' => 'links')) ?>
    <?php endif; ?> 
    <?php if ($top): ?>
      <div class="top clear span-24"><?php print $top; ?></div>
    <?php endif ?>
  </div>
  <div class="clear"></div>
  <?php if ($left): ?>
    <div class="<?php print $left_classes; ?>"><?php print $left; ?></div>
  <?php endif ?>
  
  <div class="<?php print $center_classes; ?>">
    <?php
    

      if ($tabs != '') {
        print '<div class="tabs">'. $tabs .'</div>';
      }

      if ($messages != '') {
        print '<div id="messages">'. $messages .'</div>';
      }
      
      if ($title != '') {
        print '<h2 class="main-title">'. $title .'</h2>';
      }      

      print $help; // Drupal already wraps this one in a class      

      print $content;
      print $feed_icons;
    ?>

    <?php if ($footer_message | $footer | $footer_1): ?>
      <div id="footer" class="clear">
        <?php if ($footer): ?>
          <?php print $footer; ?>
        <?php endif; ?>
        <div class="span-6 first">
          <?php print $footer_1; ?>
        </div>
        <div class="span-6">
          <?php print $footer_2; ?>
        </div>
        <div class="span-6">
          <?php print $footer_3; ?>
        </div>
        <div class="span-6 last">
          <?php print $footer_4; ?>
        </div>
        <div class="clear"></div>
        <?php if ($footer_message): ?>
          <div id="footer-message"><?php print $footer_message; ?>
			  <div id="footer-right-links">
				  <a href="<?php print $school->field_linkedin[0]['url']?>"><img src="/<?php print path_to_theme()?>/images/linkedin-small.png"/></a>
				  <a href="<?php print $school->field_facebook[0]['url']?>"><img src="/<?php print path_to_theme()?>/images/facebook-small.png"/></a>
				  <a href="http://twitter.com/<?php print $school->field_twitter[0]['value']?>"><img src="/<?php print path_to_theme()?>/images/twitter-small.png"/></a>
				  <a href="<?php print $school->field_youtube_page[0]['url']?>"><img src="/<?php print path_to_theme()?>/images/youtube-small.png"/></a>
      		 </div>
         </div>
        <?php endif; ?>
      </div>
    <?php endif; ?>
  </div>

  <?php if ($right): ?>
    <div class="<?php print $right_classes; ?>"><?php print $right; ?></div>
  <?php endif ?>
  

  <?php print $scripts ?>
  <?php print $closure; ?>

</div>

</body>
</html>
