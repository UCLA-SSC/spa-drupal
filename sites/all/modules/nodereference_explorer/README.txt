Nodereference Explorer README
---------------------------------------

Authors
-------
Gottfried Nindl (gnindl: http://drupal.org/user/421442), Web Developer, OSCE (Organisation for Security and Cooperation in Europe)
Velimir Alic (valic: http://drupal.org/user/406104), Web Architect, OSCE (Organisation for Security and Cooperation in Europe)
Benjamin J Doherty (bangpound: http://drupal.org/user/100456)

Release Notes
-------------
The overall purpose of the Nodereference Explorer module is to provide a comfortable and user-friendly way of referencing content. 
Content authors should be able to link internal arbitrary nodes, e. g. content types such as pages, stories, news, images etc., 
to each other. Typical use-cases are for instance:

    * Reuse of assets, such as images, audio and video clips for display on other pages
    * Providing a list of links to internal pages

While the nodereference module is great about creating internal references of your content's website its inbuilt widgets (autocomplete, 
select list or option boxes) for user input aren't very comfortable. The Nodereference Explorer is therefore supplied as an additional widget 
built on-top of the nodereference autocomplete widget. The autocomplete widget is enhanced by an explorer-like selection dialog which 
contains a custom or a default views. Single items, e. g. a row or a field, can be selected by a simple mouse click. Further exposed filter forms 
are rendered in order to enable a keyword, as well as taxonomy driven search.

This widget is highly configurable and it focusses stronlgy on usability. The most important features are:

* Selectable view items for all view display plugins, like grid, table, list and unformatted
* Exposed view filter forms for content search/exploration
* Plugin architecture for integration of third party CCK fields, Link is already built-in
* Highly configurable jQuery dialog including custom theming
	* Module theme
	* Global theme
	* jQuery UI themes
* Supported Dialog APIs
    * Built-in
    * Modal Frame
* Field preview (also for common autocomplete widget)
* Supported third party modules
    * Popups: Add & Reference support
    * Flexifield
* Leveraging indirectly the power of views:
	* Multilinguality
	* Lightbox or thickbox support
	* Image/Imagefield/Imagecache compatiblity
	* ...

Quick start Guide
-----------------
1. Enable the Nodererence Explorer module and the dependent modules (see the module info file for dependencies).
2. You should now be able to select the "Explorer" widget when attaching a nodereference or link field to a content type.
3. Field settings:
   a. Content: Choose the field preview display from the list the displays (depends on choosen View)
   b. Dialog: Specify dialog related settings here, e. g. title or window size. Choose a dialog theme from the 
	  available options. Note that you might specify a CSS scope for certain themes.
4. Global settings (two possibilities):
   a. Content types that can be referenced: A default view shipped with the module will be referenced with
   	  three displays: table, grid and list. Use this as a starting point for your custom view.
   b. View used to select the nodes: A customized view can be referenced here. Be aware to create a view with 
   	  displays having a table (selectable) or fields (selectable) plugin. Otherwise view items cannot be selected
   	  visually and logically. Note that the default display will not be rendered.
5. Finally, save the settings and optionally the view.
6. Now go to the edit page of your content type instance via 'Create Content' or 'Content Management->Content'. 
   You should now see one or more enhanced autocomplete input fields. Choose your content via the dialog box you open with 'Browse...' 
   and delete the reference with 'Remove'. Have fun using the exposed filters for exploring the content.
      

History
-------------
Version 1.0: Patchwork of different third party modules, like flexigrid for a browse table and aqCookie for the session. This first prototype
was developed in scope of a Proof of Concept: Can Drupal offer rich user experience? The answer: Yes, with some effort.
Version 1.1-beta1: Drupalized version solely relying on Drupal + CCK + Views + jQuery. Including of the bgiframe (IE 6 z-Index problem) plugin
and switch to jQuery UI version 1.6. Still a lot of issues to be solved.
Version 1.1-beta2: Flexibilisation of the JavaScript component (multiple field per node, quite working add more button). Theming concept and
built-in theme has been reengineered as well as critical issues have been resolved. The module now support an additional Dialog API called 
"Modal Frame". Code is quite solid now.
Version 1.1-beta3: "Add another item" button problem solved including experimental support for Link field type. This version is quite stable now.
Version 1.1-beta4: Plugin architecture, full support for Link field type.

TO-DOs:
-----------------
* Universial CCK browser: Deploy and use the explorer widget on any field type
* Extract and separate the foundation for a 'jQuery UI Widget' module (see http://drupal.org/node/454382)
* Use outside of CCK context
	* Integration with WYSIWIG editors
	* Taxonomy browser
* Port to Drupal 7 (and then into the Drupal core of course :))
