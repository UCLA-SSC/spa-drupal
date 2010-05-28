// $Id: README.txt,v 1.3 2009/02/01 01:18:00 sun Exp $

1. Installation
---------------

Copy the path_image.tar.gz file to your modules/ directory and unpack.
This will create modules/path_image folder.

Go to "Administer >> Site building >> Modules" and enable the "Path
image" module.

Go to "Administer >> User management >> Permissions" and setup the
roles to whom you wish to allow administration of the module.

Go to "Administer >> Site configuration >> Path image" to setup the
module (see below).

Go to "Administer >> Site building >> Blocks" to setup the block on
your page.

2. Configuration
----------------

  Block title: [default 'Path image']
    This sets the block title, blank out for no title.

  Image repository: [default 'images']
    The name of the folder in which the images are stored. Folder
    is relative the drupal 'files' folder as specified on the File
    system configuration page. No leading or trailing slashes.

  Allowable file types in repository: [default 'gif, jpg, png']
    A list of comma separated file extensions that will be scanned
    for in the file repsoitory. 

  Numeric parse halt: [default on]
    When searching paths the default is not to "look beyond" a numeric
    so for example node/243 would only search up to node/
    However, you may have a path like taxonomy/term/24/53 and you want
    at search all componets of the path inclusing the trailing numerics.
    If that's the case, switch this setting off.

  Default image for the homepage:
    This allows you to select an image that is displayed on the / root
    homepage of your site. A checkbox is provided to switch this feature
    on or off.

  Default image when no match made:
    This allows you to select an image that is displayed when no path 
    match is made. A checkbox is provided to switch this feature
    on or off.	

  Add a new path: Select an image:
    Enter here a new path to associate with an image (no leading/trailing
    slashes).

  Alter a path:  Select an image:
    An existing path can be amended here.

3. Paths
--------

A path match is done is reverse length order. Let's look at an example:

  taxonomy/term/3
  taxonomy

Searching is currently terminating at the first full numeric component of
the path, in this example, 3. The search would match any path that begins
'taxonomy/term' . If no match was found then the next search would be for
'taxonomy', etc.

Switching "Numeric parse halt" off would scan for all components of the path
including the numeric 3 in the example.

