// $Id: explorer.tabs.js,v 1.1 2009/07/17 15:38:28 gnindl Exp $

/**
 * @file explorer.tabs.js
 * Create tabs which contains the displays of the referenced view.
 * The default display is hidden. When there's only one display to
 * be shown the tabs are hidden as well.
 */

/**
 * Tab related events, e. g. show/hide filter a form or
 * render the initial display
 * @param
 *   DOM context
 */
Drupal.behaviors.NodereferenceExplorerTabs = function(context) {
  //create tabs
  //TODO: set options from settings, e. g. { fx: { opacity: "toggle" }} );
  var tabs = $('div.nodereference-explorer-tabset ul.nodereference-explorer-tabs', context);
  $(tabs).tabs();
  
  //disable and hide default display because it's causing inconvinient id problems with the underlying CCK form, 
  //e. g. exposed widget form fields disappear
  $(tabs).tabs('select', 1);
  $(tabs).tabs('disable', 0);
  $('li.nodereference-explorer-tab:first', tabs).hide();
  
  //show no tab if only one display (default is hidden, therefore <= 2)
  if ($(tabs).children().length <= 2)
    $('li.nodereference-explorer-tab', tabs).hide();
	  
  //display filter for selected tab
  $(tabs).bind('tabsshow', function(event, ui) {
	//FIX for browser hiding of multiple identical ids of filter forms
	$('.views-widget .form-item', context).show();
	
    // TODO the selected node needs to be highlighted. to reproduce, open
    // dialog, choose a tab, navigate to the page where the selected node is
    // highlighted and find a node ID on the same page, switch to another tab,
    // select the node with the ID you chose from the previous tab, and switch
    // back to that tab.
    $('.nodereference-explorer-views-filters', tabs).hide();
    var display = $(ui.panel).attr('id');
    $('#filter-'+display).show();
  });
}