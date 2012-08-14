<?php
// $Id: icalfield_ajaxy.utility.inc,v 1.1.2.1 2010/10/06 15:24:02 mfer Exp $

/**
 * @file
 * Utility functions for the icalfield_ajaxy widget.
 */

/**
 * The jQuery UI Timepicker replacements.
 */
function icalfield_ajaxy_timepicker_format_replacements() {
  return array('a', 'A', 'B', 'g', 'G', 'h', 'H', 'i', 's', 'u', ':');
}

/**
 * Substitutions between PHP and jQuery UI for placeholder/tokens for dates.
 *
 * jQuery UI Datepicker can build a time based on tokens. These tokens are different
 * than PHP. This list provides the replacements.
 *
 * @link http://docs.jquery.com/UI/Datepicker/formatDate
 */
function icalfield_ajaxy_datepicker_format_replacements() {
  return array(
   'd' => 'dd',
   'j' => 'd',
   'l' => 'DD',
   'D' => 'D',
   'm' => 'mm',
   'n' => 'm',
   'F' => 'MM',
   'M' => 'M',
   'Y' => 'yy',
   'y' => 'y',
   );
}

/**
 * Convert a date key into an ICAL BYDAY value.
 *
 * @param string $day
 *
 * @see icalfield_ajaxy_byday_reverse
 */
function icalfield_ajaxy_byday($day) {
  static $byday = array(
    'sun' => 'SU',
    'mon' => 'MO',
    'tue' => 'TU',
    'wed' => 'WE',
    'thu' => 'TH',
    'fri' => 'FR',
    'sat' => 'SA',
  );

  return isset($byday[$day]) ? $byday[$day] : FALSE;
}

/**
 * Convert an ICAL BYDAY value into a date key.
 *
 * @param string $day
 *
 * @todo Come up with a better name for this function. Or a better way to convert
 * back and forth. Or, remove it all together.  E.g. strtoupper(substr($day, 0, 2));
 *
 * @see icalfield_ajaxy_byday
 */
function icalfield_ajaxy_byday_reverse($day) {
  static $byday = array(
    'SU' => 'sun',
    'MO' => 'mon',
    'TU' => 'tue',
    'WE' => 'wed',
    'TH' => 'thu',
    'FR' => 'fri',
    'SA' => 'sat',
  );

  return $byday[$day];
}

/**
 * Convert a date key into a ICAL BYMONTH value.
 *
 * @param string $month
 */
function icalfield_ajaxy_bymonth($month) {
  $datetime = strtotime($month);
  return ($datetime) ? date('m', $datetime) : FALSE;
}

/**
 * Based on the number for a month return the key for the name of the month.
 *
 * @param int $month
 *   The month as a number.
 *
 * @return string
 *   The name of the month. This is a key and should not be translated.
 *
 * @see icalfield_ajaxy_bymonth
 */
function icalfield_ajaxy_bymonth_reverse($month) {
  $months =  array(
    1 => 'January',
    2 => 'February',
    3 => 'March',
    4 => 'April',
    5 => 'May',
    6 => 'June',
    7 => 'July',
    8 => 'August',
    9 => 'September',
    10 => 'October',
    11 => 'November',
    12 => 'December',
  );
  return $months[$month];
}

/**
 * Convert a name into the corresponding number. For example, convert 'first' into 1.
 *
 * @param string $name
 *
 * @return int
 *
 * @see icalfield_ajaxy_number_to_name
 */
function icalfield_ajaxy_name_to_number($name) {
  $conversion = array(
    'first' => 1,
    'second' => 2,
    'third' => 3,
    'fourth' => 4,
    'fifth' => 5,
  );

  return isset($conversion[$name]) ? $conversion[$name] : FALSE;
}

/**
 * Convert from a number to the corresponding name.
 *
 * @see icalfield_ajaxy_name_to_number
 */
function icalfield_ajaxy_number_to_name($name) {
  $conversion = array(
    1 => 'first',
    2 => 'second',
    3 => 'third',
    4 => 'fourth',
    5 => 'fifth',
  );

  return $conversion[$name];
}
