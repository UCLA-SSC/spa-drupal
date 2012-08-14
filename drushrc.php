<?php

$options['sites'] = array (
);
$options['profiles'] = array (
  0 => 'default',
);
$options['packages'] = array (
  'base' => 
  array (
    'modules' => 
    array (
      'tracker' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/modules/tracker/tracker.module',
        'basename' => 'tracker.module',
        'name' => 'tracker',
        'info' => 
        array (
          'name' => 'Tracker',
          'description' => 'Enables tracking of recent posts for users.',
          'dependencies' => 
          array (
            0 => 'comment',
          ),
          'package' => 'Core - optional',
          'version' => '6.13',
          'core' => '6.x',
          'project' => 'drupal',
          'datestamp' => '1246481719',
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'drupal',
        'version' => '6.13',
      ),
      'help' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/modules/help/help.module',
        'basename' => 'help.module',
        'name' => 'help',
        'info' => 
        array (
          'name' => 'Help',
          'description' => 'Manages the display of online help.',
          'package' => 'Core - optional',
          'version' => '6.13',
          'core' => '6.x',
          'project' => 'drupal',
          'datestamp' => '1246481719',
          'dependencies' => 
          array (
          ),
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'drupal',
        'version' => '6.13',
      ),
      'contact' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/modules/contact/contact.module',
        'basename' => 'contact.module',
        'name' => 'contact',
        'info' => 
        array (
          'name' => 'Contact',
          'description' => 'Enables the use of both personal and site-wide contact forms.',
          'package' => 'Core - optional',
          'version' => '6.13',
          'core' => '6.x',
          'project' => 'drupal',
          'datestamp' => '1246481719',
          'dependencies' => 
          array (
          ),
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'drupal',
        'version' => '6.13',
      ),
      'search' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/modules/search/search.module',
        'basename' => 'search.module',
        'name' => 'search',
        'info' => 
        array (
          'name' => 'Search',
          'description' => 'Enables site-wide keyword searching.',
          'package' => 'Core - optional',
          'version' => '6.13',
          'core' => '6.x',
          'project' => 'drupal',
          'datestamp' => '1246481719',
          'dependencies' => 
          array (
          ),
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'drupal',
        'version' => '6.13',
      ),
      'syslog' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/modules/syslog/syslog.module',
        'basename' => 'syslog.module',
        'name' => 'syslog',
        'info' => 
        array (
          'name' => 'Syslog',
          'description' => 'Logs and records system events to syslog.',
          'package' => 'Core - optional',
          'version' => '6.13',
          'core' => '6.x',
          'project' => 'drupal',
          'datestamp' => '1246481719',
          'dependencies' => 
          array (
          ),
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'drupal',
        'version' => '6.13',
      ),
      'dblog' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/modules/dblog/dblog.module',
        'basename' => 'dblog.module',
        'name' => 'dblog',
        'info' => 
        array (
          'name' => 'Database logging',
          'description' => 'Logs and records system events to the database.',
          'package' => 'Core - optional',
          'version' => '6.13',
          'core' => '6.x',
          'project' => 'drupal',
          'datestamp' => '1246481719',
          'dependencies' => 
          array (
          ),
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'drupal',
        'version' => '6.13',
      ),
      'block' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/modules/block/block.module',
        'basename' => 'block.module',
        'name' => 'block',
        'info' => 
        array (
          'name' => 'Block',
          'description' => 'Controls the boxes that are displayed around the main content.',
          'package' => 'Core - required',
          'version' => '6.13',
          'core' => '6.x',
          'project' => 'drupal',
          'datestamp' => '1246481719',
          'dependencies' => 
          array (
          ),
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'drupal',
        'version' => '6.13',
      ),
      'aggregator' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/modules/aggregator/aggregator.module',
        'basename' => 'aggregator.module',
        'name' => 'aggregator',
        'info' => 
        array (
          'name' => 'Aggregator',
          'description' => 'Aggregates syndicated content (RSS, RDF, and Atom feeds).',
          'package' => 'Core - optional',
          'version' => '6.13',
          'core' => '6.x',
          'project' => 'drupal',
          'datestamp' => '1246481719',
          'dependencies' => 
          array (
          ),
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'drupal',
        'version' => '6.13',
      ),
      'node' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/modules/node/node.module',
        'basename' => 'node.module',
        'name' => 'node',
        'info' => 
        array (
          'name' => 'Node',
          'description' => 'Allows content to be submitted to the site and displayed on pages.',
          'package' => 'Core - required',
          'version' => '6.13',
          'core' => '6.x',
          'project' => 'drupal',
          'datestamp' => '1246481719',
          'dependencies' => 
          array (
          ),
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'drupal',
        'version' => '6.13',
      ),
      'comment' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/modules/comment/comment.module',
        'basename' => 'comment.module',
        'name' => 'comment',
        'info' => 
        array (
          'name' => 'Comment',
          'description' => 'Allows users to comment on and discuss published content.',
          'package' => 'Core - optional',
          'version' => '6.13',
          'core' => '6.x',
          'project' => 'drupal',
          'datestamp' => '1246481719',
          'dependencies' => 
          array (
          ),
          'php' => '4.3.5',
        ),
        'schema_version' => 6004,
        'project' => 'drupal',
        'version' => '6.13',
      ),
      'locale' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/modules/locale/locale.module',
        'basename' => 'locale.module',
        'name' => 'locale',
        'info' => 
        array (
          'name' => 'Locale',
          'description' => 'Adds language handling functionality and enables the translation of the user interface to languages other than English.',
          'package' => 'Core - optional',
          'version' => '6.13',
          'core' => '6.x',
          'project' => 'drupal',
          'datestamp' => '1246481719',
          'dependencies' => 
          array (
          ),
          'php' => '4.3.5',
        ),
        'schema_version' => 6006,
        'project' => 'drupal',
        'version' => '6.13',
      ),
      'trigger' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/modules/trigger/trigger.module',
        'basename' => 'trigger.module',
        'name' => 'trigger',
        'info' => 
        array (
          'name' => 'Trigger',
          'description' => 'Enables actions to be fired on certain system events, such as when new content is created.',
          'package' => 'Core - optional',
          'version' => '6.13',
          'core' => '6.x',
          'project' => 'drupal',
          'datestamp' => '1246481719',
          'dependencies' => 
          array (
          ),
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'drupal',
        'version' => '6.13',
      ),
      'book' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/modules/book/book.module',
        'basename' => 'book.module',
        'name' => 'book',
        'info' => 
        array (
          'name' => 'Book',
          'description' => 'Allows users to structure site pages in a hierarchy or outline.',
          'package' => 'Core - optional',
          'version' => '6.13',
          'core' => '6.x',
          'project' => 'drupal',
          'datestamp' => '1246481719',
          'dependencies' => 
          array (
          ),
          'php' => '4.3.5',
        ),
        'schema_version' => 6001,
        'project' => 'drupal',
        'version' => '6.13',
      ),
      'update' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/modules/update/update.module',
        'basename' => 'update.module',
        'name' => 'update',
        'info' => 
        array (
          'name' => 'Update status',
          'description' => 'Checks the status of available updates for Drupal and your installed modules and themes.',
          'version' => '6.13',
          'package' => 'Core - optional',
          'core' => '6.x',
          'project' => 'drupal',
          'datestamp' => '1246481719',
          'dependencies' => 
          array (
          ),
          'php' => '4.3.5',
        ),
        'schema_version' => 6001,
        'project' => 'drupal',
        'version' => '6.13',
      ),
      'ping' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/modules/ping/ping.module',
        'basename' => 'ping.module',
        'name' => 'ping',
        'info' => 
        array (
          'name' => 'Ping',
          'description' => 'Alerts other sites when your site has been updated.',
          'package' => 'Core - optional',
          'version' => '6.13',
          'core' => '6.x',
          'project' => 'drupal',
          'datestamp' => '1246481719',
          'dependencies' => 
          array (
          ),
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'drupal',
        'version' => '6.13',
      ),
      'blogapi' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/modules/blogapi/blogapi.module',
        'basename' => 'blogapi.module',
        'name' => 'blogapi',
        'info' => 
        array (
          'name' => 'Blog API',
          'description' => 'Allows users to post content using applications that support XML-RPC blog APIs.',
          'package' => 'Core - optional',
          'version' => '6.13',
          'core' => '6.x',
          'project' => 'drupal',
          'datestamp' => '1246481719',
          'dependencies' => 
          array (
          ),
          'php' => '4.3.5',
        ),
        'schema_version' => 6002,
        'project' => 'drupal',
        'version' => '6.13',
      ),
      'user' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/modules/user/user.module',
        'basename' => 'user.module',
        'name' => 'user',
        'info' => 
        array (
          'name' => 'User',
          'description' => 'Manages the user registration and login system.',
          'package' => 'Core - required',
          'version' => '6.13',
          'core' => '6.x',
          'project' => 'drupal',
          'datestamp' => '1246481719',
          'dependencies' => 
          array (
          ),
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'drupal',
        'version' => '6.13',
      ),
      'throttle' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/modules/throttle/throttle.module',
        'basename' => 'throttle.module',
        'name' => 'throttle',
        'info' => 
        array (
          'name' => 'Throttle',
          'description' => 'Handles the auto-throttling mechanism, to control site congestion.',
          'package' => 'Core - optional',
          'version' => '6.13',
          'core' => '6.x',
          'project' => 'drupal',
          'datestamp' => '1246481719',
          'dependencies' => 
          array (
          ),
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'drupal',
        'version' => '6.13',
      ),
      'color' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/modules/color/color.module',
        'basename' => 'color.module',
        'name' => 'color',
        'info' => 
        array (
          'name' => 'Color',
          'description' => 'Allows the user to change the color scheme of certain themes.',
          'package' => 'Core - optional',
          'version' => '6.13',
          'core' => '6.x',
          'project' => 'drupal',
          'datestamp' => '1246481719',
          'dependencies' => 
          array (
          ),
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'drupal',
        'version' => '6.13',
      ),
      'php' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/modules/php/php.module',
        'basename' => 'php.module',
        'name' => 'php',
        'info' => 
        array (
          'name' => 'PHP filter',
          'description' => 'Allows embedded PHP code/snippets to be evaluated.',
          'package' => 'Core - optional',
          'version' => '6.13',
          'core' => '6.x',
          'project' => 'drupal',
          'datestamp' => '1246481719',
          'dependencies' => 
          array (
          ),
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'drupal',
        'version' => '6.13',
      ),
      'system' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/modules/system/system.module',
        'basename' => 'system.module',
        'name' => 'system',
        'info' => 
        array (
          'name' => 'System',
          'description' => 'Handles general site configuration for administrators.',
          'package' => 'Core - required',
          'version' => '6.13',
          'core' => '6.x',
          'project' => 'drupal',
          'datestamp' => '1246481719',
          'dependencies' => 
          array (
          ),
          'php' => '4.3.5',
        ),
        'schema_version' => 6052,
        'project' => 'drupal',
        'version' => '6.13',
      ),
      'openid' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/modules/openid/openid.module',
        'basename' => 'openid.module',
        'name' => 'openid',
        'info' => 
        array (
          'name' => 'OpenID',
          'description' => 'Allows users to log into your site using OpenID.',
          'version' => '6.13',
          'package' => 'Core - optional',
          'core' => '6.x',
          'project' => 'drupal',
          'datestamp' => '1246481719',
          'dependencies' => 
          array (
          ),
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'drupal',
        'version' => '6.13',
      ),
      'statistics' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/modules/statistics/statistics.module',
        'basename' => 'statistics.module',
        'name' => 'statistics',
        'info' => 
        array (
          'name' => 'Statistics',
          'description' => 'Logs access statistics for your site.',
          'package' => 'Core - optional',
          'version' => '6.13',
          'core' => '6.x',
          'project' => 'drupal',
          'datestamp' => '1246481719',
          'dependencies' => 
          array (
          ),
          'php' => '4.3.5',
        ),
        'schema_version' => 1001,
        'project' => 'drupal',
        'version' => '6.13',
      ),
      'translation' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/modules/translation/translation.module',
        'basename' => 'translation.module',
        'name' => 'translation',
        'info' => 
        array (
          'name' => 'Content translation',
          'description' => 'Allows content to be translated into different languages.',
          'dependencies' => 
          array (
            0 => 'locale',
          ),
          'package' => 'Core - optional',
          'version' => '6.13',
          'core' => '6.x',
          'project' => 'drupal',
          'datestamp' => '1246481719',
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'drupal',
        'version' => '6.13',
      ),
      'blog' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/modules/blog/blog.module',
        'basename' => 'blog.module',
        'name' => 'blog',
        'info' => 
        array (
          'name' => 'Blog',
          'description' => 'Enables keeping easily and regularly updated user web pages or blogs.',
          'package' => 'Core - optional',
          'version' => '6.13',
          'core' => '6.x',
          'project' => 'drupal',
          'datestamp' => '1246481719',
          'dependencies' => 
          array (
          ),
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'drupal',
        'version' => '6.13',
      ),
      'profile' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/modules/profile/profile.module',
        'basename' => 'profile.module',
        'name' => 'profile',
        'info' => 
        array (
          'name' => 'Profile',
          'description' => 'Supports configurable user profiles.',
          'package' => 'Core - optional',
          'version' => '6.13',
          'core' => '6.x',
          'project' => 'drupal',
          'datestamp' => '1246481719',
          'dependencies' => 
          array (
          ),
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'drupal',
        'version' => '6.13',
      ),
      'path' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/modules/path/path.module',
        'basename' => 'path.module',
        'name' => 'path',
        'info' => 
        array (
          'name' => 'Path',
          'description' => 'Allows users to rename URLs.',
          'package' => 'Core - optional',
          'version' => '6.13',
          'core' => '6.x',
          'project' => 'drupal',
          'datestamp' => '1246481719',
          'dependencies' => 
          array (
          ),
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'drupal',
        'version' => '6.13',
      ),
      'poll' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/modules/poll/poll.module',
        'basename' => 'poll.module',
        'name' => 'poll',
        'info' => 
        array (
          'name' => 'Poll',
          'description' => 'Allows your site to capture votes on different topics in the form of multiple choice questions.',
          'package' => 'Core - optional',
          'version' => '6.13',
          'core' => '6.x',
          'project' => 'drupal',
          'datestamp' => '1246481719',
          'dependencies' => 
          array (
          ),
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'drupal',
        'version' => '6.13',
      ),
      'forum' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/modules/forum/forum.module',
        'basename' => 'forum.module',
        'name' => 'forum',
        'info' => 
        array (
          'name' => 'Forum',
          'description' => 'Enables threaded discussions about general topics.',
          'dependencies' => 
          array (
            0 => 'taxonomy',
            1 => 'comment',
          ),
          'package' => 'Core - optional',
          'version' => '6.13',
          'core' => '6.x',
          'project' => 'drupal',
          'datestamp' => '1246481719',
          'php' => '4.3.5',
        ),
        'schema_version' => 6001,
        'project' => 'drupal',
        'version' => '6.13',
      ),
      'taxonomy' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/modules/taxonomy/taxonomy.module',
        'basename' => 'taxonomy.module',
        'name' => 'taxonomy',
        'info' => 
        array (
          'name' => 'Taxonomy',
          'description' => 'Enables the categorization of content.',
          'package' => 'Core - optional',
          'version' => '6.13',
          'core' => '6.x',
          'project' => 'drupal',
          'datestamp' => '1246481719',
          'dependencies' => 
          array (
          ),
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'drupal',
        'version' => '6.13',
      ),
      'menu' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/modules/menu/menu.module',
        'basename' => 'menu.module',
        'name' => 'menu',
        'info' => 
        array (
          'name' => 'Menu',
          'description' => 'Allows administrators to customize the site navigation menu.',
          'package' => 'Core - optional',
          'version' => '6.13',
          'core' => '6.x',
          'project' => 'drupal',
          'datestamp' => '1246481719',
          'dependencies' => 
          array (
          ),
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'drupal',
        'version' => '6.13',
      ),
      'upload' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/modules/upload/upload.module',
        'basename' => 'upload.module',
        'name' => 'upload',
        'info' => 
        array (
          'name' => 'Upload',
          'description' => 'Allows users to upload and attach files to content.',
          'package' => 'Core - optional',
          'version' => '6.13',
          'core' => '6.x',
          'project' => 'drupal',
          'datestamp' => '1246481719',
          'dependencies' => 
          array (
          ),
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'drupal',
        'version' => '6.13',
      ),
      'filter' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/modules/filter/filter.module',
        'basename' => 'filter.module',
        'name' => 'filter',
        'info' => 
        array (
          'name' => 'Filter',
          'description' => 'Handles the filtering of content in preparation for display.',
          'package' => 'Core - required',
          'version' => '6.13',
          'core' => '6.x',
          'project' => 'drupal',
          'datestamp' => '1246481719',
          'dependencies' => 
          array (
          ),
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'drupal',
        'version' => '6.13',
      ),
      'menu_block' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/menu_block/menu_block.module',
        'basename' => 'menu_block.module',
        'name' => 'menu_block',
        'info' => 
        array (
          'name' => 'Menu Block',
          'description' => 'Provides configurable blocks of menu items.',
          'core' => '6.x',
          'dependencies' => 
          array (
            0 => 'menu',
          ),
          'version' => '6.x-2.2',
          'project' => 'menu_block',
          'datestamp' => '1229377222',
          'php' => '4.3.5',
        ),
        'schema_version' => 6202,
        'project' => 'menu_block',
        'version' => '6.x-2.2',
      ),
      'link' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/link/link.module',
        'basename' => 'link.module',
        'name' => 'link',
        'info' => 
        array (
          'name' => 'Link',
          'description' => 'Defines simple link field types.',
          'dependencies' => 
          array (
            0 => 'content',
          ),
          'package' => 'CCK',
          'core' => '6.x',
          'version' => '6.x-2.6',
          'project' => 'link',
          'datestamp' => '1246976471',
          'php' => '4.3.5',
        ),
        'schema_version' => 6002,
        'project' => 'link',
        'version' => '6.x-2.6',
      ),
      'imagefield' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/imagefield/imagefield.module',
        'basename' => 'imagefield.module',
        'name' => 'imagefield',
        'info' => 
        array (
          'name' => 'ImageField',
          'description' => 'Defines an image field type.',
          'core' => '6.x',
          'dependencies' => 
          array (
            0 => 'content',
            1 => 'filefield',
          ),
          'package' => 'CCK',
          'version' => '6.x-3.1',
          'project' => 'imagefield',
          'datestamp' => '1246665655',
          'php' => '4.3.5',
        ),
        'schema_version' => 6006,
        'project' => 'imagefield',
        'version' => '6.x-3.1',
      ),
      'performance' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/devel/performance/performance.module',
        'basename' => 'performance.module',
        'name' => 'performance',
        'info' => 
        array (
          'name' => 'Performance Logging',
          'description' => 'Logs detailed and/or summary page generation time and memory consumption for page requests.',
          'package' => 'Development',
          'core' => '6.x',
          'version' => '6.x-1.17',
          'project' => 'devel',
          'datestamp' => '1250713853',
          'dependencies' => 
          array (
          ),
          'php' => '4.3.5',
        ),
        'schema_version' => 3,
        'project' => 'devel',
        'version' => '6.x-1.17',
      ),
      'devel' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/devel/devel.module',
        'basename' => 'devel.module',
        'name' => 'devel',
        'info' => 
        array (
          'name' => 'Devel',
          'description' => 'Various blocks, pages, and functions for developers.',
          'package' => 'Development',
          'dependencies' => 
          array (
            0 => 'menu',
          ),
          'core' => '6.x',
          'version' => '6.x-1.17',
          'project' => 'devel',
          'datestamp' => '1250713853',
          'php' => '4.3.5',
        ),
        'schema_version' => 6003,
        'project' => 'devel',
        'version' => '6.x-1.17',
      ),
      'devel_node_access' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/devel/devel_node_access.module',
        'basename' => 'devel_node_access.module',
        'name' => 'devel_node_access',
        'info' => 
        array (
          'name' => 'Devel node access',
          'description' => 'Developer block and page illustrating relevant node_access records.',
          'package' => 'Development',
          'core' => '6.x',
          'version' => '6.x-1.17',
          'project' => 'devel',
          'datestamp' => '1250713853',
          'dependencies' => 
          array (
          ),
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'devel',
        'version' => '6.x-1.17',
      ),
      'devel_generate' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/devel/devel_generate.module',
        'basename' => 'devel_generate.module',
        'name' => 'devel_generate',
        'info' => 
        array (
          'name' => 'Devel generate',
          'description' => 'Generate dummy users, nodes, and taxonomy terms.',
          'package' => 'Development',
          'core' => '6.x',
          'version' => '6.x-1.17',
          'project' => 'devel',
          'datestamp' => '1250713853',
          'dependencies' => 
          array (
          ),
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'devel',
        'version' => '6.x-1.17',
      ),
      'devel_themer' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/devel/devel_themer.module',
        'basename' => 'devel_themer.module',
        'name' => 'devel_themer',
        'info' => 
        array (
          'name' => 'Theme developer',
          'description' => 'Essential theme API information for theme developers',
          'package' => 'Development',
          'dependencies' => 
          array (
            0 => 'devel',
          ),
          'core' => '6.x',
          'version' => '6.x-1.17',
          'project' => 'devel',
          'datestamp' => '1250713853',
          'php' => '4.3.5',
        ),
        'schema_version' => 6002,
        'project' => 'devel',
        'version' => '6.x-1.17',
      ),
      'active_tags_popular' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/active_tags/active_tags_popular.module',
        'basename' => 'active_tags_popular.module',
        'name' => 'active_tags_popular',
        'info' => 
        array (
          'name' => 'Active Tags Popular',
          'description' => 'Further enhance Active Tags by making it easy for users to select from some popular tags',
          'dependencies' => 
          array (
            0 => 'active_tags',
          ),
          'package' => 'User interface',
          'core' => '6.x',
          'version' => '6.x-1.5',
          'project' => 'active_tags',
          'datestamp' => '1249850403',
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'active_tags',
        'version' => '6.x-1.5',
      ),
      'active_tags' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/active_tags/active_tags.module',
        'basename' => 'active_tags.module',
        'name' => 'active_tags',
        'info' => 
        array (
          'name' => 'Active Tags',
          'description' => 'Enhanced tag widget for free tagging vocabularies',
          'dependencies' => 
          array (
            0 => 'taxonomy',
          ),
          'package' => 'User interface',
          'core' => '6.x',
          'version' => '6.x-1.5',
          'project' => 'active_tags',
          'datestamp' => '1249850403',
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'active_tags',
        'version' => '6.x-1.5',
      ),
      'tabs' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/tabs/tabs.module',
        'basename' => 'tabs.module',
        'name' => 'tabs',
        'info' => 
        array (
          'name' => 'Tabs',
          'description' => 'A helper module for creating tabbed pages.',
          'core' => '6.x',
          'version' => '6.x-1.2',
          'project' => 'tabs',
          'datestamp' => '1251300403',
          'dependencies' => 
          array (
          ),
          'php' => '4.3.5',
        ),
        'schema_version' => 2,
        'project' => 'tabs',
        'version' => '6.x-1.2',
      ),
      'imagecache_ui' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/imagecache/imagecache_ui.module',
        'basename' => 'imagecache_ui.module',
        'name' => 'imagecache_ui',
        'info' => 
        array (
          'name' => 'ImageCache UI',
          'description' => 'ImageCache User Interface.',
          'dependencies' => 
          array (
            0 => 'imagecache',
            1 => 'imageapi',
          ),
          'package' => 'ImageCache',
          'core' => '6.x',
          'version' => '6.x-2.0-beta10',
          'project' => 'imagecache',
          'datestamp' => '1250716281',
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'imagecache',
        'version' => '6.x-2.0-beta10',
      ),
      'imagecache' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/imagecache/imagecache.module',
        'basename' => 'imagecache.module',
        'name' => 'imagecache',
        'info' => 
        array (
          'name' => 'ImageCache',
          'description' => 'Dynamic image manipulator and cache.',
          'package' => 'ImageCache',
          'dependencies' => 
          array (
            0 => 'imageapi',
          ),
          'core' => '6.x',
          'version' => '6.x-2.0-beta10',
          'project' => 'imagecache',
          'datestamp' => '1250716281',
          'php' => '4.3.5',
        ),
        'schema_version' => 6002,
        'project' => 'imagecache',
        'version' => '6.x-2.0-beta10',
      ),
      'date_popup' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/date/date_popup/date_popup.module',
        'basename' => 'date_popup.module',
        'name' => 'date_popup',
        'info' => 
        array (
          'name' => 'Date Popup',
          'description' => 'Enables jquery popup calendars and time entry widgets for selecting dates and times.',
          'dependencies' => 
          array (
            0 => 'date_api',
            1 => 'date_timezone',
          ),
          'package' => 'Date/Time',
          'core' => '6.x',
          'version' => '6.x-2.3',
          'project' => 'date',
          'datestamp' => '1248867055',
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'date',
        'version' => '6.x-2.3',
      ),
      'date_timezone' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/date/date_timezone/date_timezone.module',
        'basename' => 'date_timezone.module',
        'name' => 'date_timezone',
        'info' => 
        array (
          'name' => 'Date Timezone',
          'description' => 'Needed when using Date API. Overrides site and user timezone handling to set timezone names instead of offsets.',
          'package' => 'Date/Time',
          'dependencies' => 
          array (
            0 => 'date_api',
          ),
          'core' => '6.x',
          'version' => '6.x-2.3',
          'project' => 'date',
          'datestamp' => '1248867055',
          'php' => '4.3.5',
        ),
        'schema_version' => 5201,
        'project' => 'date',
        'version' => '6.x-2.3',
      ),
      'date_locale' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/date/date_locale/date_locale.module',
        'basename' => 'date_locale.module',
        'name' => 'date_locale',
        'info' => 
        array (
          'name' => 'Date Locale',
          'description' => 'Allows the site admin to configure multiple formats for date/time display to tailor dates for a specific locale or audience.',
          'package' => 'Date/Time',
          'dependencies' => 
          array (
            0 => 'date_api',
            1 => 'locale',
          ),
          'core' => '6.x',
          'version' => '6.x-2.3',
          'project' => 'date',
          'datestamp' => '1248867055',
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'date',
        'version' => '6.x-2.3',
      ),
      'date' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/date/date/date.module',
        'basename' => 'date.module',
        'name' => 'date',
        'info' => 
        array (
          'name' => 'Date',
          'description' => 'Defines CCK date/time fields and widgets.',
          'dependencies' => 
          array (
            0 => 'content',
            1 => 'date_api',
            2 => 'date_timezone',
          ),
          'package' => 'Date/Time',
          'core' => '6.x',
          'version' => '6.x-2.3',
          'project' => 'date',
          'datestamp' => '1248867055',
          'php' => '4.3.5',
        ),
        'schema_version' => 6006,
        'project' => 'date',
        'version' => '6.x-2.3',
      ),
      'date_tools' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/date/date_tools/date_tools.module',
        'basename' => 'date_tools.module',
        'name' => 'date_tools',
        'info' => 
        array (
          'name' => 'Date Tools',
          'description' => 'Tools to import and auto-create dates and calendars.',
          'dependencies' => 
          array (
            0 => 'content',
            1 => 'date',
          ),
          'package' => 'Date/Time',
          'core' => '6.x',
          'version' => '6.x-2.3',
          'project' => 'date',
          'datestamp' => '1248867055',
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'date',
        'version' => '6.x-2.3',
      ),
      'date_repeat' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/date/date_repeat/date_repeat.module',
        'basename' => 'date_repeat.module',
        'name' => 'date_repeat',
        'info' => 
        array (
          'name' => 'Date Repeat API',
          'description' => 'A Date Repeat API to calculate repeating dates and times from iCal rules.',
          'dependencies' => 
          array (
            0 => 'date_api',
          ),
          'package' => 'Date/Time',
          'core' => '6.x',
          'version' => '6.x-2.3',
          'project' => 'date',
          'datestamp' => '1248867055',
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'date',
        'version' => '6.x-2.3',
      ),
      'date_php4' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/date/date_php4/date_php4.module',
        'basename' => 'date_php4.module',
        'name' => 'date_php4',
        'info' => 
        array (
          'name' => 'Date PHP4',
          'description' => 'Emulate PHP 5.2 date functions in PHP 4.x, PHP 5.0, and PHP 5.1. Required when using the Date API with PHP versions less than PHP 5.2.',
          'package' => 'Date/Time',
          'dependencies' => 
          array (
            0 => 'date_api',
          ),
          'core' => '6.x',
          'version' => '6.x-2.3',
          'project' => 'date',
          'datestamp' => '1248867055',
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'date',
        'version' => '6.x-2.3',
      ),
      'date_api' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/date/date_api.module',
        'basename' => 'date_api.module',
        'name' => 'date_api',
        'info' => 
        array (
          'name' => 'Date API',
          'description' => 'A Date API that can be used by other modules.',
          'package' => 'Date/Time',
          'core' => '6.x',
          'version' => '6.x-2.3',
          'project' => 'date',
          'datestamp' => '1248867055',
          'dependencies' => 
          array (
          ),
          'php' => '4.3.5',
        ),
        'schema_version' => 6006,
        'project' => 'date',
        'version' => '6.x-2.3',
      ),
      'lightbox2' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/lightbox2/lightbox2.module',
        'basename' => 'lightbox2.module',
        'name' => 'lightbox2',
        'info' => 
        array (
          'name' => 'Lightbox2',
          'description' => 'Enables Lightbox2 for Drupal',
          'core' => '6.x',
          'version' => '6.x-1.9',
          'project' => 'lightbox2',
          'datestamp' => '1231421439',
          'dependencies' => 
          array (
          ),
          'php' => '4.3.5',
        ),
        'schema_version' => 4,
        'project' => 'lightbox2',
        'version' => '6.x-1.9',
      ),
      'panels_mini' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/panels/panels_mini/panels_mini.module',
        'basename' => 'panels_mini.module',
        'name' => 'panels_mini',
        'info' => 
        array (
          'name' => 'Mini panels',
          'description' => 'Create mini panels that can be used as blocks by Drupal and panes by other panel modules.',
          'package' => 'Panels',
          'dependencies' => 
          array (
            0 => 'panels',
          ),
          'core' => '6.x',
          'version' => '6.x-3.0',
          'project' => 'panels',
          'datestamp' => '1250724669',
          'php' => '4.3.5',
        ),
        'schema_version' => 6301,
        'project' => 'panels',
        'version' => '6.x-3.0',
      ),
      'panels_export' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/panels/panels_export/panels_export.module',
        'basename' => 'panels_export.module',
        'name' => 'panels_export',
        'info' => 
        array (
          'dependencies' => 
          array (
          ),
          'description' => '',
          'version' => NULL,
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => '',
        'version' => NULL,
      ),
      'panels_node' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/panels/panels_node/panels_node.module',
        'basename' => 'panels_node.module',
        'name' => 'panels_node',
        'info' => 
        array (
          'name' => 'Panel nodes',
          'description' => 'Create nodes that are divided into areas with selectable content.',
          'package' => 'Panels',
          'dependencies' => 
          array (
            0 => 'panels',
          ),
          'core' => '6.x',
          'version' => '6.x-3.0',
          'project' => 'panels',
          'datestamp' => '1250724669',
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'panels',
        'version' => '6.x-3.0',
      ),
      'panels' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/panels/panels.module',
        'basename' => 'panels.module',
        'name' => 'panels',
        'info' => 
        array (
          'name' => 'Panels',
          'description' => 'Core Panels display functions; provides no external UI, at least one other Panels module should be enabled.',
          'core' => '6.x',
          'package' => 'Panels',
          'dependencies' => 
          array (
            0 => 'ctools',
          ),
          'version' => '6.x-3.0',
          'project' => 'panels',
          'datestamp' => '1250724669',
          'php' => '4.3.5',
        ),
        'schema_version' => 6305,
        'project' => 'panels',
        'version' => '6.x-3.0',
      ),
      'page_manager' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/ctools/page_manager/page_manager.module',
        'basename' => 'page_manager.module',
        'name' => 'page_manager',
        'info' => 
        array (
          'name' => 'Page manager',
          'description' => 'Provides a UI and API to manage pages within the site.',
          'core' => '6.x',
          'dependencies' => 
          array (
            0 => 'ctools',
          ),
          'package' => 'Chaos tool suite',
          'version' => '6.x-1.0',
          'project' => 'ctools',
          'datestamp' => '1250724631',
          'php' => '4.3.5',
        ),
        'schema_version' => 6102,
        'project' => 'ctools',
        'version' => '6.x-1.0',
      ),
      'bulk_export' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/ctools/bulk_export/bulk_export.module',
        'basename' => 'bulk_export.module',
        'name' => 'bulk_export',
        'info' => 
        array (
          'name' => 'Bulk Export',
          'description' => 'Performs bulk exporting of data objects known about by Chaos tools.',
          'core' => '6.x',
          'dependencies' => 
          array (
            0 => 'ctools',
          ),
          'package' => 'Chaos tool suite',
          'version' => '6.x-1.0',
          'project' => 'ctools',
          'datestamp' => '1250724631',
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'ctools',
        'version' => '6.x-1.0',
      ),
      'ctools_plugin_example' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/ctools/ctools_plugin_example/ctools_plugin_example.module',
        'basename' => 'ctools_plugin_example.module',
        'name' => 'ctools_plugin_example',
        'info' => 
        array (
          'name' => 'Chaos Tools (CTools) Plugin Example',
          'description' => 'Shows how an external module can provide ctools plugins (for Panels, etc.).',
          'package' => 'Chaos tool suite',
          'dependencies' => 
          array (
            0 => 'ctools',
            1 => 'panels',
            2 => 'page_manager',
            3 => 'advanced_help',
          ),
          'core' => '6.x',
          'version' => '6.x-1.0',
          'project' => 'ctools',
          'datestamp' => '1250724631',
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'ctools',
        'version' => '6.x-1.0',
      ),
      'delegator' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/ctools/delegator/delegator.module',
        'basename' => 'delegator.module',
        'name' => 'delegator',
        'info' => 
        array (
          'dependencies' => 
          array (
          ),
          'description' => '',
          'version' => NULL,
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => '',
        'version' => NULL,
      ),
      'views_content' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/ctools/views_content/views_content.module',
        'basename' => 'views_content.module',
        'name' => 'views_content',
        'info' => 
        array (
          'name' => 'Views content panes',
          'description' => 'Allows Views content to be used in Panels, Dashboard and other modules which use the CTools Content API.',
          'package' => 'Chaos tool suite',
          'dependencies' => 
          array (
            0 => 'ctools',
            1 => 'views',
          ),
          'core' => '6.x',
          'version' => '6.x-1.0',
          'project' => 'ctools',
          'datestamp' => '1250724631',
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'ctools',
        'version' => '6.x-1.0',
      ),
      'ctools' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/ctools/ctools.module',
        'basename' => 'ctools.module',
        'name' => 'ctools',
        'info' => 
        array (
          'name' => 'Chaos tools',
          'description' => 'A library of helpful tools by Merlin of Chaos.',
          'core' => '6.x',
          'package' => 'Chaos tool suite',
          'version' => '6.x-1.0',
          'project' => 'ctools',
          'datestamp' => '1250724631',
          'dependencies' => 
          array (
          ),
          'php' => '4.3.5',
        ),
        'schema_version' => 6005,
        'project' => 'ctools',
        'version' => '6.x-1.0',
      ),
      'userreference' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/cck/modules/userreference/userreference.module',
        'basename' => 'userreference.module',
        'name' => 'userreference',
        'info' => 
        array (
          'name' => 'User Reference',
          'description' => 'Defines a field type for referencing a user from a node.',
          'dependencies' => 
          array (
            0 => 'content',
            1 => 'text',
            2 => 'optionwidgets',
          ),
          'package' => 'CCK',
          'core' => '6.x',
          'version' => '6.x-2.5',
          'project' => 'cck',
          'datestamp' => '1249334428',
          'php' => '4.3.5',
        ),
        'schema_version' => 6002,
        'project' => 'cck',
        'version' => '6.x-2.5',
      ),
      'content_copy' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/cck/modules/content_copy/content_copy.module',
        'basename' => 'content_copy.module',
        'name' => 'content_copy',
        'info' => 
        array (
          'name' => 'Content Copy',
          'description' => 'Enables ability to import/export field definitions.',
          'dependencies' => 
          array (
            0 => 'content',
          ),
          'package' => 'CCK',
          'core' => '6.x',
          'version' => '6.x-2.5',
          'project' => 'cck',
          'datestamp' => '1249334428',
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'cck',
        'version' => '6.x-2.5',
      ),
      'number' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/cck/modules/number/number.module',
        'basename' => 'number.module',
        'name' => 'number',
        'info' => 
        array (
          'name' => 'Number',
          'description' => 'Defines numeric field types.',
          'dependencies' => 
          array (
            0 => 'content',
          ),
          'package' => 'CCK',
          'core' => '6.x',
          'version' => '6.x-2.5',
          'project' => 'cck',
          'datestamp' => '1249334428',
          'php' => '4.3.5',
        ),
        'schema_version' => 6001,
        'project' => 'cck',
        'version' => '6.x-2.5',
      ),
      'fieldgroup' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/cck/modules/fieldgroup/fieldgroup.module',
        'basename' => 'fieldgroup.module',
        'name' => 'fieldgroup',
        'info' => 
        array (
          'name' => 'Fieldgroup',
          'description' => 'Create display groups for CCK fields.',
          'dependencies' => 
          array (
            0 => 'content',
          ),
          'package' => 'CCK',
          'core' => '6.x',
          'version' => '6.x-2.5',
          'project' => 'cck',
          'datestamp' => '1249334428',
          'php' => '4.3.5',
        ),
        'schema_version' => 6008,
        'project' => 'cck',
        'version' => '6.x-2.5',
      ),
      'nodereference' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/cck/modules/nodereference/nodereference.module',
        'basename' => 'nodereference.module',
        'name' => 'nodereference',
        'info' => 
        array (
          'name' => 'Node Reference',
          'description' => 'Defines a field type for referencing one node from another.',
          'dependencies' => 
          array (
            0 => 'content',
            1 => 'text',
            2 => 'optionwidgets',
          ),
          'package' => 'CCK',
          'core' => '6.x',
          'version' => '6.x-2.5',
          'project' => 'cck',
          'datestamp' => '1249334428',
          'php' => '4.3.5',
        ),
        'schema_version' => 6002,
        'project' => 'cck',
        'version' => '6.x-2.5',
      ),
      'content_permissions' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/cck/modules/content_permissions/content_permissions.module',
        'basename' => 'content_permissions.module',
        'name' => 'content_permissions',
        'info' => 
        array (
          'name' => 'Content Permissions',
          'description' => 'Set field-level permissions for CCK fields.',
          'package' => 'CCK',
          'core' => '6.x',
          'dependencies' => 
          array (
            0 => 'content',
          ),
          'version' => '6.x-2.5',
          'project' => 'cck',
          'datestamp' => '1249334428',
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'cck',
        'version' => '6.x-2.5',
      ),
      'optionwidgets' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/cck/modules/optionwidgets/optionwidgets.module',
        'basename' => 'optionwidgets.module',
        'name' => 'optionwidgets',
        'info' => 
        array (
          'name' => 'Option Widgets',
          'description' => 'Defines selection, check box and radio button widgets for text and numeric fields.',
          'dependencies' => 
          array (
            0 => 'content',
          ),
          'package' => 'CCK',
          'core' => '6.x',
          'version' => '6.x-2.5',
          'project' => 'cck',
          'datestamp' => '1249334428',
          'php' => '4.3.5',
        ),
        'schema_version' => 6002,
        'project' => 'cck',
        'version' => '6.x-2.5',
      ),
      'text' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/cck/modules/text/text.module',
        'basename' => 'text.module',
        'name' => 'text',
        'info' => 
        array (
          'name' => 'Text',
          'description' => 'Defines simple text field types.',
          'dependencies' => 
          array (
            0 => 'content',
          ),
          'package' => 'CCK',
          'core' => '6.x',
          'version' => '6.x-2.5',
          'project' => 'cck',
          'datestamp' => '1249334428',
          'php' => '4.3.5',
        ),
        'schema_version' => 6004,
        'project' => 'cck',
        'version' => '6.x-2.5',
      ),
      'content' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/cck/content.module',
        'basename' => 'content.module',
        'name' => 'content',
        'info' => 
        array (
          'name' => 'Content',
          'description' => 'Allows administrators to define new content types.',
          'package' => 'CCK',
          'core' => '6.x',
          'version' => '6.x-2.5',
          'project' => 'cck',
          'datestamp' => '1249334428',
          'dependencies' => 
          array (
          ),
          'php' => '4.3.5',
        ),
        'schema_version' => 6011,
        'project' => 'cck',
        'version' => '6.x-2.5',
      ),
      'imagecache_canvasactions' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/imagecache_actions/imagecache_canvasactions.module',
        'basename' => 'imagecache_canvasactions.module',
        'name' => 'imagecache_canvasactions',
        'info' => 
        array (
          'name' => 'Imagecache Canvas Actions',
          'description' => 'Actions for manipulating image canvases layers, including watermark and background effect. Also an aspect switcher (portrait/landscape)',
          'dependencies' => 
          array (
            0 => 'imagecache',
            1 => 'imageapi',
          ),
          'package' => 'ImageCache',
          'core' => '6.x',
          'version' => '6.x-1.6',
          'project' => 'imagecache_actions',
          'datestamp' => '1244598933',
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'imagecache_actions',
        'version' => '6.x-1.6',
      ),
      'imagecache_coloractions' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/imagecache_actions/imagecache_coloractions.module',
        'basename' => 'imagecache_coloractions.module',
        'name' => 'imagecache_coloractions',
        'info' => 
        array (
          'name' => 'Imagecache Color Actions',
          'description' => 'Additional ImageCache actions, providing color-shifting, brightness and alpha transparency effects.',
          'dependencies' => 
          array (
            0 => 'imagecache',
            1 => 'imageapi',
          ),
          'package' => 'ImageCache',
          'core' => '6.x',
          'version' => '6.x-1.6',
          'project' => 'imagecache_actions',
          'datestamp' => '1244598933',
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'imagecache_actions',
        'version' => '6.x-1.6',
      ),
      'imagecache_customactions' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/imagecache_actions/imagecache_customactions.module',
        'basename' => 'imagecache_customactions.module',
        'name' => 'imagecache_customactions',
        'info' => 
        array (
          'name' => 'Imagecache Custom Actions',
          'description' => 'Allow direct PHP code manipulation of imagecache images.',
          'dependencies' => 
          array (
            0 => 'imagecache',
          ),
          'package' => 'ImageCache',
          'core' => '6.x',
          'version' => '6.x-1.6',
          'project' => 'imagecache_actions',
          'datestamp' => '1244598933',
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'imagecache_actions',
        'version' => '6.x-1.6',
      ),
      'imagecache_textactions' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/imagecache_actions/imagecache_textactions.module',
        'basename' => 'imagecache_textactions.module',
        'name' => 'imagecache_textactions',
        'info' => 
        array (
          'name' => 'Imagecache Text Actions',
          'description' => 'Display simple or dynamic captions on images.',
          'dependencies' => 
          array (
            0 => 'imagecache',
          ),
          'package' => 'ImageCache',
          'core' => '6.x',
          'version' => '6.x-1.6',
          'project' => 'imagecache_actions',
          'datestamp' => '1244598933',
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'imagecache_actions',
        'version' => '6.x-1.6',
      ),
      'views_tabs' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/views_tabs/views_tabs.module',
        'basename' => 'views_tabs.module',
        'name' => 'views_tabs',
        'info' => 
        array (
          'name' => 'Views Tabs',
          'description' => 'Enables the presentation of views through tabs.',
          'package' => 'Views',
          'dependencies' => 
          array (
            0 => 'views',
            1 => 'tabs',
          ),
          'core' => '6.x',
          'version' => '6.x-1.x-dev',
          'project' => 'views_tabs',
          'datestamp' => '1229386634',
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'views_tabs',
        'version' => '6.x-1.x-dev',
      ),
      'admin_menu' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/admin_menu/admin_menu.module',
        'basename' => 'admin_menu.module',
        'name' => 'admin_menu',
        'info' => 
        array (
          'name' => 'Administration menu',
          'description' => 'Provides a dropdown menu to most administrative tasks and other common destinations (to users with the proper permissions).',
          'package' => 'Administration',
          'core' => '6.x',
          'version' => '6.x-1.5',
          'project' => 'admin_menu',
          'datestamp' => '1246537502',
          'dependencies' => 
          array (
          ),
          'php' => '4.3.5',
        ),
        'schema_version' => 6002,
        'project' => 'admin_menu',
        'version' => '6.x-1.5',
      ),
      'email' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/email/email.module',
        'basename' => 'email.module',
        'name' => 'email',
        'info' => 
        array (
          'name' => 'Email',
          'description' => 'Defines an email field type for cck',
          'dependencies' => 
          array (
            0 => 'content',
          ),
          'package' => 'CCK',
          'core' => '6.x',
          'version' => '6.x-1.2',
          'project' => 'email',
          'datestamp' => '1244921128',
          'php' => '4.3.5',
        ),
        'schema_version' => 6002,
        'project' => 'email',
        'version' => '6.x-1.2',
      ),
      'jcalendar' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/calendar/jcalendar/jcalendar.module',
        'basename' => 'jcalendar.module',
        'name' => 'jcalendar',
        'info' => 
        array (
          'name' => 'Calendar Popup',
          'description' => 'Replaces the links to calendar items with a javascript popup that gracefully regresses if javascript is not enabled',
          'dependencies' => 
          array (
            0 => 'calendar',
            1 => 'views',
          ),
          'core' => '6.x',
          'package' => 'Date/Time',
          'version' => '6.x-2.2',
          'project' => 'calendar',
          'datestamp' => '1248867038',
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'calendar',
        'version' => '6.x-2.2',
      ),
      'calendar_ical' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/calendar/calendar_ical/calendar_ical.module',
        'basename' => 'calendar_ical.module',
        'name' => 'calendar_ical',
        'info' => 
        array (
          'name' => 'Calendar iCal',
          'description' => 'Adds ical functionality to Calendar views.',
          'dependencies' => 
          array (
            0 => 'views',
            1 => 'date_api',
            2 => 'calendar',
          ),
          'package' => 'Date/Time',
          'core' => '6.x',
          'version' => '6.x-2.2',
          'project' => 'calendar',
          'datestamp' => '1248867038',
          'php' => '4.3.5',
        ),
        'schema_version' => 6001,
        'project' => 'calendar',
        'version' => '6.x-2.2',
      ),
      'calendar' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/calendar/calendar.module',
        'basename' => 'calendar.module',
        'name' => 'calendar',
        'info' => 
        array (
          'name' => 'Calendar',
          'description' => 'Views plugin to display views containing dates as Calendars.',
          'dependencies' => 
          array (
            0 => 'views',
            1 => 'date_api',
            2 => 'date_timezone',
          ),
          'package' => 'Date/Time',
          'core' => '6.x',
          'version' => '6.x-2.2',
          'project' => 'calendar',
          'datestamp' => '1248867038',
          'php' => '4.3.5',
        ),
        'schema_version' => 6003,
        'project' => 'calendar',
        'version' => '6.x-2.2',
      ),
      'skinr' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/skinr/skinr.module',
        'basename' => 'skinr.module',
        'name' => 'skinr',
        'info' => 
        array (
          'name' => 'Skinr',
          'description' => 'Provides a way to define and/or skin bits of Drupal output from the UI.',
          'core' => '6.x',
          'version' => '6.x-1.0-beta1',
          'project' => 'skinr',
          'datestamp' => '1244726191',
          'dependencies' => 
          array (
          ),
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'skinr',
        'version' => '6.x-1.0-beta1',
      ),
      'views_export' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/views/views_export/views_export.module',
        'basename' => 'views_export.module',
        'name' => 'views_export',
        'info' => 
        array (
          'name' => 'Views exporter',
          'description' => 'Allows exporting multiple views at once.',
          'package' => 'Views',
          'dependencies' => 
          array (
            0 => 'views',
          ),
          'core' => '6.x',
          'version' => '6.x-2.6',
          'project' => 'views',
          'datestamp' => '1244671915',
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'views',
        'version' => '6.x-2.6',
      ),
      'views' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/views/views.module',
        'basename' => 'views.module',
        'name' => 'views',
        'info' => 
        array (
          'name' => 'Views',
          'description' => 'Create customized lists and queries from your database.',
          'package' => 'Views',
          'core' => '6.x',
          'version' => '6.x-2.6',
          'project' => 'views',
          'datestamp' => '1244671915',
          'dependencies' => 
          array (
          ),
          'php' => '4.3.5',
        ),
        'schema_version' => 6007,
        'project' => 'views',
        'version' => '6.x-2.6',
      ),
      'views_ui' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/views/views_ui.module',
        'basename' => 'views_ui.module',
        'name' => 'views_ui',
        'info' => 
        array (
          'name' => 'Views UI',
          'description' => 'Administrative interface to views. Without this module, you cannot create or edit your views.',
          'package' => 'Views',
          'core' => '6.x',
          'dependencies' => 
          array (
            0 => 'views',
          ),
          'version' => '6.x-2.6',
          'project' => 'views',
          'datestamp' => '1244671915',
          'php' => '4.3.5',
        ),
        'schema_version' => 1,
        'project' => 'views',
        'version' => '6.x-2.6',
      ),
      'wysiwyg' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/wysiwyg/wysiwyg.module',
        'basename' => 'wysiwyg.module',
        'name' => 'wysiwyg',
        'info' => 
        array (
          'name' => 'Wysiwyg',
          'description' => 'Allows users to edit contents with client-side editors.',
          'package' => 'User interface',
          'core' => '6.x',
          'version' => '6.x-2.0',
          'project' => 'wysiwyg',
          'datestamp' => '1244598972',
          'dependencies' => 
          array (
          ),
          'php' => '4.3.5',
        ),
        'schema_version' => 6002,
        'project' => 'wysiwyg',
        'version' => '6.x-2.0',
      ),
      'imagepicker' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/modules/imagepicker/imagepicker.module',
        'basename' => 'imagepicker.module',
        'name' => 'imagepicker',
        'info' => 
        array (
          'name' => 'Imagepicker',
          'description' => 'Upload, browse and add images to your nodes.',
          'package' => 'Other',
          'core' => '6.x',
          'version' => '6.x-2.2',
          'project' => 'imagepicker',
          'datestamp' => '1248986495',
          'dependencies' => 
          array (
          ),
          'php' => '4.3.5',
        ),
        'schema_version' => 6202,
        'project' => 'imagepicker',
        'version' => '6.x-2.2',
      ),
    ),
    'themes' => 
    array (
      'pushbutton' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/themes/pushbutton/pushbutton.info',
        'basename' => 'pushbutton.info',
        'name' => 'Pushbutton',
        'info' => 
        array (
          'name' => 'Pushbutton',
          'description' => 'Tabled, multi-column theme in blue and orange tones.',
          'version' => '6.13',
          'core' => '6.x',
          'engine' => 'phptemplate',
          'project' => 'drupal',
          'datestamp' => '1246481719',
        ),
        'owner' => '/var/aegir/UCLA/drupal-6.13/themes/engines/phptemplate/phptemplate.engine',
        'prefix' => 'phptemplate',
        'template' => true,
        'project' => 'drupal',
        'version' => '6.13',
      ),
      'minnelli' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/themes/garland/minnelli/minnelli.info',
        'basename' => 'minnelli.info',
        'name' => 'Minnelli',
        'info' => 
        array (
          'name' => 'Minnelli',
          'description' => 'Tableless, recolorable, multi-column, fixed width theme.',
          'version' => '6.13',
          'core' => '6.x',
          'base theme' => 'garland',
          'stylesheets' => 
          array (
            'all' => 
            array (
              0 => 'minnelli.css',
            ),
          ),
          'project' => 'drupal',
          'datestamp' => '1246481719',
        ),
        'project' => 'drupal',
        'version' => '6.13',
      ),
      'garland' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/themes/garland/garland.info',
        'basename' => 'garland.info',
        'name' => 'Garland',
        'info' => 
        array (
          'name' => 'Garland',
          'description' => 'Tableless, recolorable, multi-column, fluid width theme (default).',
          'version' => '6.13',
          'core' => '6.x',
          'engine' => 'phptemplate',
          'stylesheets' => 
          array (
            'all' => 
            array (
              0 => 'style.css',
            ),
            'print' => 
            array (
              0 => 'print.css',
            ),
          ),
          'project' => 'drupal',
          'datestamp' => '1246481719',
        ),
        'owner' => '/var/aegir/UCLA/drupal-6.13/themes/engines/phptemplate/phptemplate.engine',
        'prefix' => 'phptemplate',
        'template' => true,
        'project' => 'drupal',
        'version' => '6.13',
      ),
      'marvin' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/themes/chameleon/marvin/marvin.info',
        'basename' => 'marvin.info',
        'name' => 'Marvin',
        'info' => 
        array (
          'name' => 'Marvin',
          'description' => 'Boxy tabled theme in all grays.',
          'regions' => 
          array (
            'left' => 'Left sidebar',
            'right' => 'Right sidebar',
          ),
          'version' => '6.13',
          'core' => '6.x',
          'base theme' => 'chameleon',
          'project' => 'drupal',
          'datestamp' => '1246481719',
        ),
        'project' => 'drupal',
        'version' => '6.13',
      ),
      'chameleon' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/themes/chameleon/chameleon.info',
        'basename' => 'chameleon.info',
        'name' => 'Chameleon',
        'info' => 
        array (
          'name' => 'Chameleon',
          'description' => 'Minimalist tabled theme with light colors.',
          'regions' => 
          array (
            'left' => 'Left sidebar',
            'right' => 'Right sidebar',
          ),
          'features' => 
          array (
            0 => 'logo',
            1 => 'favicon',
            2 => 'name',
            3 => 'slogan',
          ),
          'stylesheets' => 
          array (
            'all' => 
            array (
              0 => 'style.css',
              1 => 'common.css',
            ),
          ),
          'version' => '6.13',
          'core' => '6.x',
          'project' => 'drupal',
          'datestamp' => '1246481719',
        ),
        'project' => 'drupal',
        'version' => '6.13',
      ),
      'bluemarine' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/themes/bluemarine/bluemarine.info',
        'basename' => 'bluemarine.info',
        'name' => 'Bluemarine',
        'info' => 
        array (
          'name' => 'Bluemarine',
          'description' => 'Table-based multi-column theme with a marine and ash color scheme.',
          'version' => '6.13',
          'core' => '6.x',
          'engine' => 'phptemplate',
          'project' => 'drupal',
          'datestamp' => '1246481719',
        ),
        'owner' => '/var/aegir/UCLA/drupal-6.13/themes/engines/phptemplate/phptemplate.engine',
        'prefix' => 'phptemplate',
        'template' => true,
        'project' => 'drupal',
        'version' => '6.13',
      ),
      'blueprint' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/themes/blueprint/blueprint.info',
        'basename' => 'blueprint.info',
        'name' => 'Blueprint',
        'info' => 
        array (
          'name' => 'Blueprint',
          'description' => 'Drupal base theme built with Blueprint CSS framework.',
          'version' => '6.x-1.2',
          'core' => '6.x',
          'engine' => 'phptemplate',
          'stylesheets' => 
          array (
            'screen,projection' => 
            array (
              0 => 'blueprint/blueprint/screen.css',
              1 => 'css/blueprint.css',
              2 => 'css/style.css',
            ),
            'print' => 
            array (
              0 => 'blueprint/blueprint/print.css',
            ),
          ),
          'scripts' => 
          array (
            0 => 'scripts/general.js',
          ),
          'regions' => 
          array (
            'left' => 'Left sidebar',
            'right' => 'Right sidebar',
            'content' => 'Content',
            'header' => 'Header',
            'footer' => 'Footer',
          ),
          'project' => 'blueprint',
          'datestamp' => '1230042309',
        ),
        'owner' => '/var/aegir/UCLA/drupal-6.13/themes/engines/phptemplate/phptemplate.engine',
        'prefix' => 'phptemplate',
        'template' => true,
        'project' => 'blueprint',
        'version' => '6.x-1.2',
      ),
      'ninesixty' => 
      array (
        'filename' => '/var/aegir/UCLA/drupal-6.13/sites/all/themes/ninesixty/ninesixty.info',
        'basename' => 'ninesixty.info',
        'name' => '960 Grid System',
        'info' => 
        array (
          'name' => '960 Grid System',
          'description' => 'A blank base theme using the 960 Grid System. See the "README.txt" file within the theme folder for more information.',
          'screenshot' => 'images/screenshot.png',
          'core' => '6.x',
          'engine' => 'phptemplate',
          'stylesheets' => 
          array (
            'all' => 
            array (
              0 => 'styles/framework/reset.css',
              1 => 'styles/framework/text.css',
              2 => 'styles/framework/960.css',
              3 => 'styles/framework/debug.css',
              4 => 'styles/styles.css',
            ),
          ),
          'version' => '6.x-1.0',
          'project' => 'ninesixty',
          'datestamp' => '1249223138',
        ),
        'owner' => '/var/aegir/UCLA/drupal-6.13/themes/engines/phptemplate/phptemplate.engine',
        'prefix' => 'phptemplate',
        'template' => true,
        'project' => 'ninesixty',
        'version' => '6.x-1.0',
      ),
    ),
    'platforms' => 
    array (
      'drupal' => 
      array (
        'short_name' => 'drupal',
        'version' => '6.13',
        'description' => 'This platform is running Drupal 6.13',
      ),
    ),
    'profiles' => 
    array (
      'default' => 
      array (
        'name' => 'default',
        'filename' => '/var/aegir/UCLA/drupal-6.13/profiles/default/default.profile',
        'project' => '',
        'info' => 
        array (
          'name' => 'Drupal',
          'description' => 'Select this profile to enable some basic Drupal functionality and the default theme.',
          'languages' => 
          array (
            0 => 'en',
          ),
          'version' => NULL,
        ),
        'version' => '6.13',
      ),
    ),
  ),
  'profiles' => 
  array (
    'default' => 
    array (
      'modules' => 
      array (
      ),
      'themes' => 
      array (
      ),
    ),
  ),
);
$options['master_url'] = 'http://ryanaegirlaptop.com';
$options['publish_path'] = '/var/aegir/UCLA/drupal-6.13';
$options['web_host'] = 'ryanaegirlaptop.com';
$options['web_ip'] = '';
$options['web_group'] = 'www-data';
$options['script_user'] = 'aegir';
$options['restart_cmd'] = 'sudo /usr/sbin/apache2ctl graceful';
$options['backup_path'] = '/var/aegir/backups';
$options['config_path'] = '/var/aegir/config';
$options['master_db'] = 'mysqli://aegir:this1@localhost';
