// $Id: icalfield_ajaxy.js,v 1.1.2.1 2010/09/21 22:25:11 crell Exp $
/**
 * @file
 * Provides the JavaScript side of the icalfield ajaxy widget beyond what ctools ajax
 * and other core JavaScript files provide.
 */

(function($) {
Drupal.behaviors.icalAjaxy = function(context) {
  // Add the JS to each of the fields.
  for (i in Drupal.settings.icalAjaxy) {
    $('#' + Drupal.settings.icalAjaxy[i].id + '-items-wrapper', context).drupalIcalAjaxy(Drupal.settings.icalAjaxy[i]);
  }

  // Setup the tabs.
  $('.ical-ajaxy-tabs', context).tabs().addClass('ui-tabs-vertical ui-helper-clearfix');
  $('.ical-ajaxy-tabs li', context).removeClass('ui-corner-top').addClass('ui-corner-left');
  $('.icalfield-ajaxy-selected-tab-value', context).click();
  
  // We go over all the tables and set the height where appropriate on a per table basis.
  $('table.icalfield-ajaxy-rule-table tbody', context).each(function() {
    var $this = $(this);
    if ($this.height() > 140) {
      $this.height(140);
    }
  });

  // Messages sent back when updated by ajax should only display for 30 seconds.
  $('.icalfield-ajaxy-message', context).each(function() {
    var $this = $(this);
    setTimeout(function() {
      $this.fadeOut('slow');
    } , 10000);
  });

  // Some ie6 specific css hacks cannot be done in css. Since JS is required we
  // make changes here.
  if ($.browser.msie && parseInt($.browser.version) == 6) {
    // .parents('.form-item:first') or calling first() did now work in ie6.
    // going for the parent items directory worked.
    // @todo There must be a better way to set the druation clearing.
    $('.ical-ajaxy-duration', context).parent().parent().addClass('clear-block');
  }
}

/**
 * The ical Ajaxy JavaScript is instanced and attached to each of the fields in the DOM.
 * This way the internal functions do not need to know the context of which field they
 * are attached to.
 */
$.fn.drupalIcalAjaxy = function(options) {
  this.each(function() {
    $.drupalIcalAjaxy(this, options);
  });
  return this;
}

$.drupalIcalAjaxy = function(container, options) {
  var container = $(container)[0];
  return container.drupalIcalAjaxy || (container.drupalIcalAjaxy = new Drupal.icalAjaxy(container, options));
}

/**
 * icalAjaxy is the object that handles in browser functionality apart from
 * ctools. This is instanciated for each instance.
 */
Drupal.icalAjaxy = function(container, options) {
  var APP = this;
  var $container = $(container);

  APP.init = function() {
    // Remove the JS not enabled message since JS is enabled to even get here.
    $container.find('#' + options.id + '-no-js').remove();
    $('#' + options.id + '-occurrence').show(); /* @todo: what is this? should it be removed? */

    // Firefox 3+ renders pages differently than sarafi, chrome, or IE. The DOM
    // is slightly different for form elements on labels. Firefox does not put
    // other form elements inside a label. Instead, it closes the label and the
    // next form element is at the same level. Other browsers put the form
    // element within the label. I could not find anything in the specs
    // specifying proper behavior here. Is there something?
    if ($.browser.mozilla) {
      // Add a class saying this is firefox. There must be a better way to do this.
      $container.addClass('icalfield-ajaxy-mozilla');
    }

    // We need a way to identify ie6 and ie7 for specific css fixes. When can
    // we stop supporting ie6?
    if ($.browser.msie) {
      $container.addClass('icalfield-ajaxy-msie-' + parseInt($.browser.version));
    }

    // Attach the full datetime calendar. We use .live (regularly) because parts of the
    // form are removed and added back.
    $container.find('.icalfield-ajaxy-cal-icon').live('click', function() {
      // If we have an all day event we do not include the time picer. Only the
      // date is required.
      var $this = $(this).parent().find('.ical-ajaxy-datetime');
      var checked = $this.parents('.icalfield-ajaxy-form-wrapper:first').find('.icalfield-ajaxy-all-day:checked').length;

      $this.datepicker('destroy');

      if (checked) {
        $this.datepicker({
          duration: '',
          constrainInput: false,
          changeMonth: true,
          changeYear: true,
          dateFormat: options.input_format,
          showOn: ''
        }).datepicker('show');
      }
      else {
        $this.datepicker({
          duration: '',
          showTime: true,
          stepMinutes: 5,
          constrainInput: false,
          changeMonth: true,
          changeYear: true,
          dateFormat: options.input_format,
          showOn: ''
        }).datepicker('show');
      }
    });
    
    // Attach the date picker.
    $container.find('.icalfield-ajaxy-cal-icon-date').live('click', function() {
      $this = $(this).parent().find('.ical-ajaxy-datepicker');
      $this.datepicker('destroy');
      $this.datepicker({
        duration: '',
        constrainInput: false,
        changeMonth: true,
        changeYear: true,
        dateFormat: options.input_format,
        showOn: ''
      }).datepicker('show');
    });
    
    // After the start date/time is entered and if there is no end time we set a default
    // end date time for one hour later.
    // @todo: It would be good for this amount of time to be configurable.
    $container.find('.ical-ajaxy-start-date').live('change', function() {
      // Find the end date.
      var $start = $(this);
      
      // The time inserted by the timepicker is not available as soon as the
      // field changes. It is inserted as a second step and is not picked up by
      // the code. So, we stick a slight delay in so we can pick up the time.
      setTimeout(function() {
        var $end = $start.parents('#' + options.id + '-single-wrapper, #' + options.id + '-multiple-wrapper').find('.ical-ajaxy-end-date');
        // If no end date is filled in generate one for an hour from the start.
        
        var checked = $start.parents('.icalfield-ajaxy-form-wrapper:first').find('.icalfield-ajaxy-all-day:checked').length;
        if ($end.val() == '' && !checked) {
          // Get the start date
          var date = new Date($start.val());
          
          // Validate the Date object was able to parse the date/time before
          // using it.
          if (!isNaN(date.getHours())) {
           
            // Add an hour
            var time = date.getTime();

            date.setTime(time + (60 * 60 * 1000)); /* 60 seconds in a minute, 60 minutes in an hour, 1000 milliseconds per second. */

            // Convert to the specified format and set as end date. Since datepicker is a dependency we can use
            // its utility functions.
            $end.val($.datepicker.formatDate('mm/dd/yy', date) + ' ' + APP.misc.getTime(date));
          }
        }
      }, 250);
    });
  
    // On the multiple form attach the single update handler.
    $container.find('.ical-ajaxy-single-update').live('click', function() {
      var id = $(this).attr('ical_id');
      var formData = $('#' + options.id + '-data-single input[value="' + id + '"]').parent().children().serialize();
      APP.misc.ajax(this, formData);
      return false;
    });
  
    $container.find('.ical-ajaxy-single-cancel').live('click', function() {
      var $this = $(this);
      var id = $this.attr('ical_id');
      var formData = $('#' + options.id + '-data-single input[value="' + id + '"]').parent().children().serialize();

      APP.misc.ajax(this, formData);
    
      var text = Drupal.t('Canceled');
      if ($this.text() == Drupal.t('Canceled')) {
        text = Drupal.t('Cancel');
      }
    
      $this.text(text)
        // Adding line-through on a tr does not affect the supporting content. Instead adding it to the td.
        .parents('tr:first').children('td').toggleClass('ical-ajaxy-canceled');
      return false;
    });
    
    /* Remove single dates from the list. */
    $container.find('.ical-ajaxy-single-remove').live('click', function() {
      var id = $(this).attr('ical_id');
      var formData = $('#' + options.id + '-data-single input[value="' + id + '"]').parent().remove();
    
      $(this).parents('tr:first').remove();
      
      // @todo if this is the last one we need to remove the wrapper.
      return false;
    });
    
    /* The rule links */
    $container.find('.ical-ajaxy-rule-update').live('click', function() {
      var id = $(this).attr('ical_id');
      var formData = $('#' + options.id + '-data-rules input[value="' + id + '"]').parent().children().serialize();
      APP.misc.ajax(this, formData);
      return false;
    });
    
    /* Remove Rules from the editor. */
    $container.find('.ical-ajaxy-rule-remove').live('click', function() {
      // Get the ical_id we are removing.
      var id = $(this).attr('ical_id');
      
      // Remove it from the data store on the element.
      $('#' + options.id + '-data-rules input[value="' + id + '"]').parent().remove();
    
      // Remove it from the editor.
      $(this).parents('#' + options.id + '-editor-items-rule-' + id).remove();
      
      // @todo if this is the last one we need to remove the wrapper.
      return false;
    });

    // Cancel a rule.
    $container.find('.ical-ajaxy-rule-cancel').live('click', function() {
      var $this = $(this);
      var id = $this.attr('ical_id');
      // Get the existing data for the rule.
      var formData = $('#' + options.id + '-data-rules input[value="' + id + '"]').parent().children().serialize();
      // Send that data back to Drupal.
      APP.misc.ajax(this, formData);

      // Flip the name of the link.
      var text = Drupal.t('Canceled');
      if ($this.text() == text) {
        text = Drupal.t('Cancel');
      }
    
      // Set the links new text.
      $this.text(text)
      
      // Add toggle the parent class for visually displaying canceled
      .parents('.ical-ajaxy-editor-title:first').toggleClass('ical-ajaxy-canceled');
      
      // Stop the link from being followed.
      return false;
    });
    
    // Exclude a rule date.
    $container.find('.ical-ajaxy-rule-exclude').live('click', function() {
      var $this = $(this);
      var id = $this.attr('ical_id');
      // Get the existing data for the rule.
      var formData = $('#' + options.id + '-data-rules input[value="' + id + '"]').parent().children().serialize();
      // Send that data back to Drupal.
      APP.misc.ajax(this, formData);

      // Flip the name of the link.
      var text = Drupal.t('Included');
      if ($this.text() == text) {
        text = Drupal.t('Excluded');
      }
    
      // Set the links new text.
      $this.text(text).toggleClass('ui-icon-circle-minus')
      
      // Adding an excluded class when an item is excluded so you can visually
      // see it.
      .parents('tr:first').children('td').toggleClass('ical-ajaxy-excluded');
      
      // Stop the link from being followed.
      return false;
    });

    /* The recurrence rule tabs */
    $container.find('.ical-ajaxy-tab-container').live('mouseup', function(e) {
      $e = $(e.target);
      if ($e.hasClass('ical-ajaxy-tab')) {
        var name = $e.text();
        $('#' + options.id + '-items-multiple-tabs-selection').val(name);
      }
    });
    // When the page loads set the active tab.
    $container.find('.icalfield-ajaxy-selected-tab-value').live('click', function(e) {
      // Get the value.
      var value = $(this).val();
      
      // Find the correct tab and click it. This will highlight it.
      $container.find("div.ical-ajaxy-tabs ul.ical-ajaxy-tab-container a:contains('" + value + "')").click();
    });

    // Make the tables in the editor collapsible.
    $container.find('.icalfield-ajaxy-collapse-button').live('click', function() {
      $(this).toggleClass('ui-icon-triangle-1-s').parents('.icalfield-ajaxy-header:first').find('.icalfield-ajaxy-table').slideToggle('slow');
    });
  };

  /**
   * This is an extension of the ctools ajax. In this case the data sent back is the
   * form data which is passed in.
   */
  APP.misc = {
    ajax: function(obj, formData) {
      if ($(object).hasClass('ctools-ajaxing')) {
        return false;
      }
  
      var url = $(obj).attr('href');
      var object = $(obj);
      $(obj).addClass('ctools-ajaxing');
      try {
        url = url.replace(/nojs/g, 'ajax');
        $.ajax({
          type: "POST",
          url: url,
          data: formData, /* The form data which is passed in. */
          global: true,
          success: Drupal.CTools.AJAX.respond,
          error: function(xhr) {
            Drupal.CTools.AJAX.handleErrors(xhr, url);
          },
          complete: function() {
            object.removeClass('ctools-ajaxing');
          },
          dataType: 'json'
        });
      }
      catch (err) {
        alert("An error occurred while attempting to process " + url);
        $(obj).removeClass('ctools-ajaxing');
        return false;
      }
    },
    /**
     * Return a time in the form of hh:mm (am|pm).
     */
    getTime: function(dateTime) {
      var hours = dateTime.getHours();
      var minutes = dateTime.getMinutes();
      var part = null;
      
      // am or pm?
      if (hours < 12) {
        part = 'am';
      }
      else {
        hours -= 12;
        part = 'pm';
      }
      
      // 0 is really 12 am.
      if (hours == 0) {
        hours = 12;
      }
      
      // If hours and minutes are only one character long, make them two.
      if (hours < 10) {
        hours = "0" + hours;
      }
      if (minutes < 10) {
        minutes = "0" + minutes;
      }
      return hours + ':' + minutes + ' ' + part;
    }
  };
  
  APP.init();
};
})(jQuery);
