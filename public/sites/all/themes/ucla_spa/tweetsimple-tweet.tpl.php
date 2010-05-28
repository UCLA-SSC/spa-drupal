<?php
// $Id: tweetsimple-tweet.tpl.php,v 1.1 2009/08/03 17:49:58 rfpldev Exp $

/**
 * @file tweetsimple-tweet.tpl.php
 * Default theme implementation to present tweets
 *
 * This template is used to loop through and render each tweet
 *
 * Available variables:
 * - $twicon: Twitter user icon.
 * - $twauthor: Twitter author name.
 * 				To suppress author (real name), use:
 *              substr($twauthor, 0, strpos($twauthor, '('))
 * - $twauthor_link: Twitter author link.
 * - $twitem: Twitter text item
 * - $twdate: Twitter post date
 * - $twfrom: Twitter source
 *
 */
?>
<div class="tweetblock">

  <div class="twitem"><?php echo _filter_url($twitem, 1); ?><br />
   

    <small><?php echo $twdate; ?></small>
  </div>
  
</div>