<?php
// $Id: template.php,v 1.15.2.1.2.15 2008/12/23 03:40:02 designerbrent Exp $

/**
 * Uncomment the following line during development to automatically
 * flush the theme cache when you load the page. That way it will
 * always look for new tpl files.
 */
// drupal_flush_all_caches();

/**
 * Intercept page template variables
 *
 * @param $vars
 *   A sequential array of variables passed to the theme function.
 */
function phptemplate_preprocess_page(&$vars) {
  global $user;
  $vars['path'] = base_path() . path_to_theme() .'/';
  $vars['user'] = $user;

  // Fixup the $head_title and $title vars to display better.
  $title = drupal_get_title();
  $headers = drupal_set_header();

  // wrap taxonomy listing pages in quotes and prefix with topic
  if (arg(0) == 'taxonomy' && arg(1) == 'term' && is_numeric(arg(2))) {
    $title = t('Topic') .' &#8220;'. $title .'&#8221;';
  }
  // if this is a 403 and they aren't logged in, tell them they need to log in
  else if (strpos($headers, 'HTTP/1.1 403 Forbidden') && !$user->uid) {
    $title = t('Please login to continue');
  }
  $vars['title'] = $title;

  if (!drupal_is_front_page()) {
    $vars['head_title'] = $title .' | '. $vars['site_name'];
    if ($vars['site_slogan'] != '') {
      $vars['head_title'] .= ' &ndash; '. $vars['site_slogan'];
    }
  }

  // determine layout
  // 3 columns
  if ($vars['layout'] == 'both') {
    $vars['left_classes'] = 'col-left span-6';
    $vars['right_classes'] = 'col-right span-6 last';
    $vars['center_classes'] = 'col-center span-12';
    $vars['body_classes'] .= ' col-3 ';
  }
  // 2 columns
  else if ($vars['layout'] != 'none') {
    // left column & center
    if ($vars['layout'] == 'left') {
      $vars['left_classes'] = 'col-left span-6';
      $vars['center_classes'] = 'col-center span-18 last';
    }
    // right column & center
    else if ($vars['layout'] == 'right') {
      $vars['right_classes'] = 'col-right span-6 last';
      $vars['center_classes'] = 'col-center span-18';
    }
    $vars['body_classes'] .= ' col-2 ';
  }
  // 1 column
  else {

    $vars['center_classes'] = 'col-center span-24';
    $vars['body_classes'] .= ' col-1 ';
  }

  $vars['meta'] = '';
  // SEO optimization, add in the node's teaser, or if on the homepage, the mission statement
  // as a description of the page that appears in search engines
  if ($vars['is_front'] && $vars['mission'] != '') {
    $vars['meta'] .= '<meta name="description" content="'. ucla_spa_trim_text($vars['mission']) .'" />'."\n";
  }
  else if (isset($vars['node']->teaser) && $vars['node']->teaser != '') {
    $vars['meta'] .= '<meta name="description" content="'. ucla_spa_trim_text($vars['node']->teaser) .'" />'."\n";
  }
  else if (isset($vars['node']->body) && $vars['node']->body != '') {
    $vars['meta'] .= '<meta name="description" content="'. ucla_spa_trim_text($vars['node']->body) .'" />'."\n";
  }
  // SEO optimization, if the node has tags, use these as keywords for the page
  if (isset($vars['node']->taxonomy)) {
    $keywords = array();
    foreach ($vars['node']->taxonomy as $term) {
      $keywords[] = $term->name;
    }
    $vars['meta'] .= '<meta name="keywords" content="'. implode(',', $keywords) .'" />'."\n";
  }

  // SEO optimization, avoid duplicate titles in search indexes for pager pages
  if (isset($_GET['page']) || isset($_GET['sort'])) {
    $vars['meta'] .= '<meta name="robots" content="noindex,follow" />'. "\n";
  }

  /* I like to embed the Google search in various places, uncomment to make use of this
  // setup search for custom placement
  $search = module_invoke('google_cse', 'block', 'view', '0');
  $vars['search'] = $search['content'];
  */
  
  /* to remove specific CSS files from modules use this trick
  // Remove stylesheets
  $css = $vars['css'];
  unset($css['all']['module']['sites/all/modules/contrib/plus1/plus1.css']);
  $vars['styles'] = drupal_get_css($css);   
  */
}

/**
 * Intercept node template variables
 *
 * @param $vars
 *   A sequential array of variables passed to the theme function.
 */
function phptemplate_preprocess_node(&$vars) {
  $node = $vars['node']; // for easy reference
  // for easy variable adding for different node types
  switch ($node->type) {
    case 'page':
      break;
  }
}

/**
 * Intercept comment template variables
 *
 * @param $vars
 *   A sequential array of variables passed to the theme function.
 */
function phptemplate_preprocess_comment(&$vars) {
  static $comment_count = 1; // keep track the # of comments rendered
  
  // if the author of the node comments as well, highlight that comment
  $node = node_load($vars['comment']->nid);
  if ($vars['comment']->uid == $node->uid) {
    $vars['author_comment'] = TRUE;
  }
  // only show links for users that can administer links
  if (!user_access('administer comments')) {
    $vars['links'] = '';
  }
  // if subjects in comments are turned off, don't show the title then
  if (!variable_get('comment_subject_field', 1)) {
    $vars['title'] = '';
  }
  // if user has no picture, add in a filler
  if (empty($vars['comment']->picture)) {
    $vars['picture'] = '<div class="no-picture">&nbsp;</div>';
  }

  $vars['comment_count'] = $comment_count++;  
}

/**
 * Override or insert variables into the block templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("block" in this case.)
 */
function phptemplate_preprocess_block(&$vars, $hook) {
  $block = $vars['block'];

  // Special classes for blocks.
  $classes = array('block');
  $classes[] = 'block-' . $block->module;
  $classes[] = 'region-' . $vars['block_zebra'];
  $classes[] = $vars['zebra'];
  $classes[] = 'region-count-' . $vars['block_id'];
  $classes[] = 'count-' . $vars['id'];

  $vars['edit_links_array'] = array();
  $vars['edit_links'] = '';
  
  if (user_access('administer blocks')) {
    include_once './' . drupal_get_path('theme', 'blueprint') . '/template.block-editing.inc';
    phptemplate_preprocess_block_editing($vars, $hook);
    $classes[] = 'with-block-editing';
  }

  // Render block classes.
  $vars['classes'] = implode(' ', $classes);
}


/**
 * Intercept box template variables
 *
 * @param $vars
 *   A sequential array of variables passed to the theme function.
 */
function phptemplate_preprocess_box(&$vars) {
  // rename to more common text
  if (strpos($vars['title'], 'Post new comment') === 0) {
    $vars['title'] = 'Add your comment';
  }
}

/**
 * Override, remove "not verified", confusing
 *
 * Format a username.
 *
 * @param $object
 *   The user object to format, usually returned from user_load().
 * @return
 *   A string containing an HTML link to the user's page if the passed object
 *   suggests that this is a site user. Otherwise, only the username is returned.
 */
function ucla_spa_username($object) {
  if ($object->uid && $object->name) {
    // Shorten the name when it is too long or it will break many tables.
    if (drupal_strlen($object->name) > 20) {
      $name = drupal_substr($object->name, 0, 15) .'...';
    }
    else {
      $name = $object->name;
    }
    $user=user_load(array('uid' => $object->uid));
    $user_roles = array_keys($user->roles);
    if (in_array(4, $user_roles)) {
      $link=l('School of Public Affairs', 'home', array('attributes' => array('title' => t('View school profile.'))));
    }
     if (in_array(7, $user_roles)) {
      $link=l('Department of Public Policy', 'public-policy', array('attributes' => array('title' => t('View school profile.'))));
    }
     if (in_array(8, $user_roles)) {
      $link=l('Department of Social Welfare', 'social-welfare', array('attributes' => array('title' => t('View school profile.'))));
    }
     if (in_array(9, $user_roles)) {
      $link=l('Department of Urban Planning', 'urban-planning', array('attributes' => array('title' => t('View school profile.'))));
    }
    $profile=content_profile_load('faculty_profile', $object->uid);
    if (user_access('access user profiles')) {
      if ($link) {
        $output=$link;
        
      } elseif ($profile) {
        $output = l($profile->title, 'node/'. $profile->nid , array('attributes' => array('title' => t('View user profile.'))));
      } else {
        $output = l($name, 'user/'. $object->uid, array('attributes' => array('title' => t('View user profile.'))));
      }  
    }
    else {
      $output = check_plain($name);
    }
  }
  else if ($object->name) {
    // Sometimes modules display content composed by people who are
    // not registered members of the site (e.g. mailing list or news
    // aggregator modules). This clause enables modules to display
    // the true author of the content.
    if (!empty($object->homepage)) {
      $output = l($object->name, $object->homepage, array('attributes' => array('rel' => 'nofollow')));
    }
    else {
      $output = check_plain($object->name);
    }
  }
  else {
    $output = variable_get('anonymous', t('Anonymous'));
  }

  return $output;
}

/**
 * Override, make sure Drupal doesn't return empty <P>
 *
 * Return a themed help message.
 *
 * @return a string containing the helptext for the current page.
 */
function ucla_spa_help() {
  $help = menu_get_active_help();
  // Drupal sometimes returns empty <p></p> so strip tags to check if empty
  if (strlen(strip_tags($help)) > 1) {
    return '<div class="help">'. $help .'</div>';
  }
}

/**
 * Override, use a better default breadcrumb separator.
 *
 * Return a themed breadcrumb trail.
 *
 * @param $breadcrumb
 *   An array containing the breadcrumb links.
 * @return a string containing the breadcrumb output.
 */
/*function ucla_spa_breadcrumb($breadcrumb) {
  if (count($breadcrumb) > 1) {
    $breadcrumb[] = drupal_get_title();
    return '<div class="breadcrumb">'. implode(' &rsaquo; ', $breadcrumb) .'</div>';
  }
}
*/
/**
 * Rewrite of theme_form_element() to suppress ":" if the title ends with a punctuation mark.
 */
function ucla_spa_form_element($element, $value) {
  $args = func_get_args();
  return preg_replace('@([.!?]):\s*(</label>)@i', '$1$2', call_user_func_array('theme_form_element', $args));
}

/**
 * Set status messages to use Blueprint CSS classes.
 */
function ucla_spa_status_messages($display = NULL) {
  $output = '';
  foreach (drupal_get_messages($display) as $type => $messages) {
    // blueprint can either call this success or notice
    if ($type == 'status') {
      $type = 'success';
    }
    $output .= "<div class=\"messages $type\">\n";
    if (count($messages) > 1) {
      $output .= " <ul>\n";
      foreach ($messages as $message) {
        $output .= '  <li>'. $message ."</li>\n";
      }
      $output .= " </ul>\n";
    }
    else {
      $output .= $messages[0];
    }
    $output .= "</div>\n";
  }
  return $output;
}

/**
 * Override comment wrapper to show you must login to comment.
 */
function ucla_spa_comment_wrapper($content, $node) {
  global $user;
  $output = '';

  if ($node = menu_get_object()) {
    if ($node->type != 'forum') {
      $count = $node->comment_count .' '. format_plural($node->comment_count, 'comment', 'comments');
      $count = ($count > 0) ? $count : 'No comments';
      $output .= '<h3 id="comment-number">'. $count .'</h3>';
    }
  }

  $output .= '<div id="comments">';
  $msg = '';
  if (!user_access('post comments')) {
    $dest = 'destination='. $_GET['q'] .'#comment-form';
    $msg = '<div id="messages"><div class="error-wrapper"><div class="messages error">'. t('Please <a href="!register">register</a> or <a href="!login">login</a> to post a comment.', array('!register' => url("user/register", array('query' => $dest)), '!login' => url('user', array('query' => $dest)))) .'</div></div></div>';
  }
  $output .= $content;
  $output .= $msg;

  return $output .'</div>';
}

/**
 * Override, use better icons, source: http://drupal.org/node/102743#comment-664157
 *
 * Format the icon for each individual topic.
 *
 * @ingroup themeable
 */
function ucla_spa_forum_icon($new_posts, $num_posts = 0, $comment_mode = 0, $sticky = 0) {
  // because we are using a theme() instead of copying the forum-icon.tpl.php into the theme
  // we need to add in the logic that is in preprocess_forum_icon() since this isn't available
  if ($num_posts > variable_get('forum_hot_topic', 15)) {
    $icon = $new_posts ? 'hot-new' : 'hot';
  }
  else {
    $icon = $new_posts ? 'new' : 'default';
  }

  if ($comment_mode == COMMENT_NODE_READ_ONLY || $comment_mode == COMMENT_NODE_DISABLED) {
    $icon = 'closed';
  }

  if ($sticky == 1) {
    $icon = 'sticky';
  }

  $output = theme('image', path_to_theme() . "/images/icons/forum-$icon.png");

  if ($new_posts) {
    $output = "<a name=\"new\">$output</a>";
  }

  return $output;
}

/**
 * Override, remove previous/next links for forum topics
 *
 * Makes forums look better and is great for performance
 * More: http://www.sysarchitects.com/node/70
 */
function ucla_spa_forum_topic_navigation($node) {
  return '';
}

/**
 * Trim a post to a certain number of characters, removing all HTML.
 */
function ucla_spa_trim_text($text, $length = 150) {
  // remove any HTML or line breaks so these don't appear in the text
  $text = trim(str_replace(array("\n", "\r"), ' ', strip_tags($text)));
  $text = trim(substr($text, 0, $length));
  $lastchar = substr($text, -1, 1);
  // check to see if the last character in the title is a non-alphanumeric character, except for ? or !
  // if it is strip it off so you don't get strange looking titles
  if (preg_match('/[^0-9A-Za-z\!\?]/', $lastchar)) {
    $text = substr($text, 0, -1);
  }
  // ? and ! are ok to end a title with since they make sense
  if ($lastchar != '!' && $lastchar != '?') {
    $text .= '...';
  }
  return $text;
}








/**
 * Theme primary links as nice menus
 *
 * @param $direction
 *   Optional. The direction the menu expands. Default is 'down'.
 * @param $menu
 *   Optional. A custom menu array to use for theming --
 *   it should have the same structure as that returned
 *   by menu_tree_all_data(). Default is the standard menu tree.
 * @return
 *   An HTML string of nice menu primary links.
 */
function ucla_spa_nice_menu_primary_links($direction = 'down', $menu = NULL) {
  $menu_name = variable_get('menu_primary_links_source', 'primary-links');
 
  $output = theme('nice_menu', 0, $menu_name, 0, $direction, $menu);
  return $output['content'];
}


/**
 * Builds the final nice menu.
 *
 * @param $menu_name
 *   The top-level menu name that contains the menu to use (e.g. navigation
 *   or primary-links) for Drupal menus. For custom $menus this is just the
 *   name for menu display.
 * @param $mlid
 *   The menu ID from which to start building the items, i.e. the parent
 *   of the displayed menu.
 * @param $menu
 *   Optional. A custom menu array to use for theming -- it should have
 *   the same structure as that returned by menu_tree_all_data().
 * @return
 *   An HTML string of properly nested nice menu lists.
 */
function ucla_spa_nice_menu_tree($menu_name, $mlid = NULL, $menu = NULL) {
  // Load the full menu array.
  $menu = isset($menu) ? $menu : menu_tree_all_data($menu_name);

  // For custom $menus and menus built all the way from the top-level we
  // don't need to "create" the specific sub-menu and we need to get the title
  // from the $menu_name since there is no "parent item" array.

  // Create the specific menu if we have a mlid.
  if (!empty($mlid)) {
    // Load the parent menu item.
    $item = menu_link_load($mlid);
   
    $title = check_plain($item['title']);
    // Narrow down the full menu to the specific sub-tree we need.
    for ($p = 1; $p < 10; $p++) {
      if ($sub_mlid = $item["p$p"]) {
        $subitem = menu_link_load($sub_mlid);
        // Menu sets these ghetto-ass keys in _menu_tree_check_access().
        $menu = $menu[(50000 + $subitem['weight']) .' '. $subitem['title'] .' '. $subitem['mlid']]['below'];
      }
    }
  }
  // Otherwise just set a title and move on.
  else {
    // Get the title from the DB since we don't have it in the $menu.
    $result = db_result(db_query("SELECT title FROM {menu_custom} WHERE menu_name = '%s'", $menu_name));
    $title = check_plain($result);
  }

  $output['content'] = '';
  $output['subject'] = $title;

  if ($menu) {
    $output['content'] .= theme('nice_menu_build', $menu);
  }

  return $output;
}

/**
 * Helper function that builds the nested lists of a nice menu.
 *
 * @param $menu
 *   Menu array from which to build the nested lists.
 */
function ucla_spa_nice_menu_build($menu) {
  $output = '';
  
   if (arg(0)== 'node' && is_numeric(arg(1))) {
     $node=node_load(arg(1));
     }
     
      
  foreach ($menu as $menu_item) {
    $mlid = $menu_item['link']['mlid'];
    // Check to see if it is a visible menu item.
    if ($menu_item['link']['hidden'] == 0) {
    
    if ($node) {
          $href='node/'. $node->nid;
	  //if (db_result(db_query("SELECT count(mlid) FROM {menu_links} WHERE link_path = '%s' AND module = 'menu'", $menu_item['link']['href'])) == 0) {
	  $type_trails = variable_get('menutrails_node_types', array());
          
	  if ($type_trails[$node->type] ) {
            $href = $type_trails[$node->type] ? $type_trails[$node->type] : FALSE;
          }
	  $term_trails = variable_get('menutrails_terms', array());
	  if ($term_trails[$term->tid]) {
            foreach ($node->taxonomy as $term) {
		if ($term_trails[$term->tid]) {
		  $href = $term_trails[$term->tid];
		}
	    }
          }
          
          
          
    } else {
      //its not a node but just a page..
      $href=trim($_SERVER['REQUEST_URI'], '/');
      
      
    }  
  
    
    if ($menu_item['below']) {
      foreach ($menu_item['below'] as $below) {
       if ($href == $below['link']['href']) {
       
          $menu_item['link']['in_active_trail']=TRUE;
          $menu_item['link']['active']='active-trail';
         $active=t('active-trail');
       // print 'SUCCESS';
        //krumo($menu_item);
         
        } else {
         $active=null;
        }
      }
    }  
    
    
      // Build class name based on menu path
      // e.g. to give each menu item individual style.
      // Strip funny symbols.
      $clean_path = str_replace(array('http://', '<', '>', '&', '=', '?', ':'), '', $menu_item['link']['href']);
      // Convert slashes to dashes.
      $clean_path = str_replace('/', '-', $clean_path);
      $path_class = 'menu-path-'. $clean_path;
      // If it has children build a nice little tree under it.
      if ((!empty($menu_item['link']['has_children'])) && (!empty($menu_item['below']))) {
        // Keep passing children into the function 'til we get them all.
        $children = theme('nice_menu_build', $menu_item['below']);
        // Set the class to parent only of children are displayed.
        $parent_class = $children ? 'menuparent ' : '';
        $output .= '<li id="menu-'. $mlid .'" class="'. $menu_item['link']['active'] .' '. $parent_class . $path_class .'">'. theme('menu_item_link', $menu_item['link']);
        // Build the child UL only if children are displayed for the user.
        if ($children) {
          $output .= '<ul>';
          $output .= $children;
          $output .= "<div class='clear-me'></div></ul>\n";
        }
        $output .= "<div class='clear-me'></div></li>\n";
      }
      else {
        $output .= '<li id="menu-'. $mlid .'" class="'. $active . $path_class .'">'. theme('menu_item_link', $menu_item['link']) .'</li>'."\n";
      }
    }
  }
  return $output;
}

/***
* SUBSTITUTE THEME FUNCTION
*/

/***
* This is a substitute function for theme_links()
*
* The important difference is that we use the in_active_trail bit here to set
* an "active" CSS class, which is what most themes (e.g. garland) use to 
* denote an active/open menu item. You should alter/override this as your 
* design needs dictate.
*
*/


function ucla_spa_links($links, $attributes = array('class' => 'links')) {
  $output = '';
  

  if (count($links) > 0) {
    $output = '<ul'. drupal_attributes($attributes) .'>';

    $num_links = count($links);
    $i = 1;

    foreach ($links as $key => $link) {
      $class = $key;

      // Add first, last and active classes to the list of links to help out themers.
      if ($i == 1) {
        $class .= ' first';
      }
      if ($i == $num_links) {
        $class .= ' last';
      }
      if (isset($link['href']) && ($link['href'] == $_GET['q'] || ($link['href'] == '<front>' && drupal_is_front_page()))) {
        $class .= ' active';
      }
      
      if (isset($link['href'])) {
        // add active class for containing <li> and <a> if active-trail is set on the link itself
        if (strpos($link['attributes']['class'], 'active-trail') !== FALSE && strpos($class, 'active') === FALSE) {
           $class .= ' active';
           $link['attributes']['class'] .= ' active';
         }
        // Pass in $link as $options, they share the same keys.
        $link = l($link['title'], $link['href'], $link);
      }
      else if (!empty($link['title'])) {
        // Some links are actually not links, but we wrap these in <span> for adding title and class attributes
        if (empty($link['html'])) {
          $link['title'] = check_plain($link['title']);
        }
        $span_attributes = '';
        if (isset($link['attributes'])) {
          $span_attributes = drupal_attributes($link['attributes']);
        }
        $link = '<span'. $span_attributes .'>'. $link['title'] .'</span>';
      }

      $i++;
      $output .= '<li'. drupal_attributes(array('class' => $class)) .'>';
      $output .= $link;
      $output .= "</li>\n";
    }

    $output .= '</ul>';
  }

// set original value
//menu_set_item('faculty-index/public-policy', 3593);

  return $output;
}




function ucla_spa_tweetsimple_getblock($tweet_uri, $num) {
  // simplepie_get((string) tweeter search uri, (string) simplepie cache loc, (int) cache refresh in seconds)
  if ($simplepie = simplepie_get($tweet_uri, FALSE, 600)) {
    foreach ($simplepie->get_items() as $item) {
      if ($author = $item->get_author()) {
        $twauthor = check_plain($author->get_name());
        $twauthor_link = filter_xss($author->get_link());
      } 
      else {
        $twauthor = '';
      }
      
      $t_src = $item->get_item_tags('http://api.twitter.com/', 'source');
      $twfrom = filter_xss(trim($t_src[0]['data'], '"'));
      
      $tweets[] = array(
        'twicon' => filter_xss($item->get_link(0, $rel = 'image')),
        'twauthor' => $twauthor,
        'twauthor_link' => $twauthor_link,   
        'twitem' => filter_xss($item->get_description(), $allowed_tags = array('a', 'em', 'strong', 'cite', 'code', 'ul', 'b')),
        'twdate' => $item->get_date('j F Y | g:i a '),
        'twfrom' => $twfrom,
      );
    }
  } 
  else {
    drupal_set_message(t("Simplepie error."), 'error');
  }
  $count=0;
  while ( $count < $num ) {
    $tweets_final[$count]=$tweets[$count];
    $count++;
  }
 
  // echo '<pre>'; print_r($tweets); echo '</pre>';
  if (count($tweets_final)) {
 	 $output.=theme('tweetsimple_tweets', $tweets_final);
 	 $link=str_replace('/statuses/user_timeline', '', $tweet_uri);
 	 $link=str_replace('.atom','',$link);
 	 if (arg(0) == 'node' && is_numeric(arg(1))) {
 	   $node=node_load(arg(1));
 	   }
 	 if ($node->type == 'faculty_profile') {
 	   $output.= "<div class='tweetlink'><a href='". $link ."'>Visit the Professor's Twitter Page</a></div>";
 	 } else {
 	    $output.= "<div class='tweetlink'><a href='". $link ."'>Visit our Twitter Page</a></div>";
 	 } 
 	 return $output;
  } else {
  	return '<p class="err" style="margin: 8px; padding: 12px;">Sorry, this feed is currently empty, check later or visit <a href="http://www.twitter.com">twitter.com</a> to post your own tweets!</p>';
  }
}

function ucla_spa_tweetsimple_tweets($tweets) {
  $output = '<div id="twblock">';
  foreach ($tweets as $tweet) {
    $output .= theme('tweetsimple_tweet', $tweet['twicon'],  $tweet['twauthor'], $tweet['twauthor_link'], $tweet['twitem'],  $tweet['twdate'], $tweet['twfrom']);
  }
  $output .= '</div>';
  return $output;
}



function ucla_spa_preprocess_views_exposed_form(&$vars, $hook) {
  //krumo($vars);
  // only alter the jobs search exposed filter form
  if ($vars['form']['#id'] == 'views-exposed-form-glossary-panel-pane-1') {
   
    $vars['widgets']['filter-field_last_name_value']=NULL;
    
    
	$alphabet = array ("A","B","C","D","E","F","G","H","I","J","K","L","M",
	"N","O","P","Q","R","S","T","U","V","W","X","Y","Z");
	foreach ($alphabet as $letter) {
	 
	 $output.='<li>';
	 $output.=l($letter, 'faculty-index',  array('query' => 'keys=&field_areas_of_interest_nid=All&field_last_name_value='. $letter));
	 $output.='</li>';
	}
	$output.= '<div class="alpha-ind-head">You can also search alphabetically by Last Name</div><div class="alpha-ind-wrapper"><ul class="alpha-index">'. $output .'</ul></div>';
    //$vars['form']['#description']= $output;
   
    // Change the text on the submit button
    //$vars['form']['submit']['#value'] = t('Search');

    // Rebuild the rendered version (submit button, rest remains unchanged)
    //unset($vars['form']['submit']['#printed']);
    //$vars['button'] = drupal_render($vars['form']['submit']);
  }
}


/**
* Implementation of hook_views_pre_view()
*
* This will be invoked on every view request before the view is themed, even if the view is cached
*/
function ucla_spa_views_pre_view(&$view, &$items) {
  if ($view->display_handler->display->display_plugin=='page') {
   // drupal_set_breadcrumb(menutrails_get_breadcrumbs());
  }
}
/*
function menutrails_views_pre_view(&$view, &$items) {
if ($view->display_handler->display->display_plugin=='page') {
$trail = menutrails_get_breadcrumbs();
$deadweight = array_pop($trail);
drupal_set_breadcrumb($trail);
}
}*/



function phptemplate_node_submitted($node) {
  return t('Submitted by !username Posted on @datetime',
    array(
      '!username' => theme('username', $node),
      '@datetime' => date('F d, Y', $node->created),
    ));
}


function ucla_spa_preprocess_search_results(&$variables) {
  $variables['search_results'] = '';
  foreach ($variables['results'] as $result) {
    $variables['search_results'] .= theme('search_result', $result, $variables['type']);
  }
 
  $variables['search_count']=count($variables['results']);
  $variables['pager'] = theme('pager', NULL, 10, 0);
  // Provide alternate search results template.
  $variables['template_files'][] = 'search-results-'. $variables['type'];
}

/**
 * Theme override of theme_search_theme_form().
 */
function phptemplate_search_theme_form($form) {
  // Replace the normal label with a hint.
  unset($form['search_theme_form']['#title']);
  $form['search_theme_form']['#hint'] = t('search...');
  $form['submit']['#value'] = t('>');
  $form['submit']['#type'] = 'image_button';
  $form['submit']['#src'] = path_to_theme() . '/images/search-button.gif';
  return drupal_render($form);
}
/**
 * Theme override of theme_search_theme_form().
 */
function phptemplate_search_form($form) {
  // Replace the normal label with a hint.
  unset($form['search_form']['#title']);
  $form['search_form']['#hint'] = t('Search');
  return drupal_render($form);
}


/**
 * Views Slideshow: "previous" control.
 *
 * @ingroup themeable
 */
function phptemplate_views_slideshow_singleframe_control_previous($vss_id, $view, $options) {
  $img = '<img src="/'. path_to_theme() .'/images/slide-prev.png"/>';
  return l($img, '#', array(
    'attributes' => array(
      'class' => 'views_slideshow_singleframe_previous views_slideshow_previous',
      'id' => "views_slideshow_singleframe_prev_" . $vss_id,
    ),
    'html' => 'true',
    'fragment' => ' ',
    'external' => TRUE,
  ));
}

/**
 * Views Slideshow: "pause" control.
 *
 * @ingroup themeable
 */
function phptemplate_views_slideshow_singleframe_control_pause($vss_id, $view, $options) {
  return l(t(''), '', array(
    'attributes' => array(
      'class' => 'views_slideshow_singleframe_pause views_slideshow_pause',
      'id' => "views_slideshow_singleframe_playpause_" . $vss_id,
    ),
    'fragment' => ' ',
    'external' => TRUE,
  ));
}

/**
 * Views Slideshow: "next" control.
 *
 * @ingroup themeable
 */
function phptemplate_views_slideshow_singleframe_control_next($vss_id, $view, $options) {
  $img = '<img src="/'. path_to_theme() .'/images/slide-next.png"/>';
  return l($img, '#', array(
    'attributes' => array(
      'class' => 'views_slideshow_singleframe_next views_slideshow_next',
      'id' => "views_slideshow_singleframe_next_" . $vss_id,
    ),
    'html' => 'true',
    'fragment' => ' ',
    'external' => TRUE,
  ));
}