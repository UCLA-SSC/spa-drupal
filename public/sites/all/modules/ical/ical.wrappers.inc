<?php
/**
 * @file
 * Extension classes for the iCalCreator library to make it more useful.
 */

class vevent_fixed extends vevent {

  /**
   * Old style constructor to follow iCalCreator style.
   *
   * We have to override the object name generation for the constructor since
   * our class is named differently.
   */
  function vevent_fixed() {
    parent::vevent();
    $this->objName = 'vevent';
  }

  /**
   * Bug fix for vevent::_date_time_string
   *
   * I don't fully comprehend what this method does, but there is some kind of
   * bug with its handling of untimed strings.  We're going to try and work
   * around it.
   */
  function _date_time_string( $datetime, $parno=FALSE ) {
    $output = parent::_date_time_string($datetime, $parno);

    // If there is no time component on the input, ensure there is no time
    // component on the output, either. The base version of this method is buggy
    // and tries to add on times when it shouldn't, resulting in midnight being
    // added instead of no-time.  See line 4361 of iCalcreator.class.php.
    if (strpos($datetime, 'T') === FALSE) {
      unset($output['hour'], $output['min'], $output['sec']);
    }

    return $output;
  }
}

/**
 * Wrapper class for the vevent class provided by iCalCreator.
 *
 * This class provides important additional functionality above and beyond
 * that provided by iCalCreator.
 */
class DrupalVEvent {

  /**
   * This is the date string format iCal uses.  Don't ask why.
   *
   * @var string
   */
  const ICAL_DATE_FORMAT = 'Ymd\THisO';
  //const ICAL_DATE_FORMAT = 'c';

  /**
   * The VEvent object we are wrapping.
   *
   * @var vevent
   */
  protected $vevent;

  /**
   * Creates a new instance of this class based on the provided iCal vevent fragment.
   *
   * @param string $vevent
   *   The string version of the VEVENT record. If it has start and end tags for
   *   the VEVENT itself they will be sanitized as necessary.
   * @return DrupalVEvent
   *   A new instance of this class.
   */
  public static function create($vevent) {
    // The beginning and ending of the vevent aren't handled by the vevent parser. It is
    // only looking for properties and not the surrounding to them. I believe this is due
    // to the parser trying to be used by a calendar and not for an individual property.
    $vevent = str_replace(array('BEGIN:VEVENT', 'END:VEVENT'), '', $vevent);

    $vevent = str_replace("\r\n", "\n", $vevent);

    // When lines are longer than 75 octets they are wrapped with a new line character followed
    // by a space. Before we can parse this we need to remove this wrapping. In our cases we are
    // normalizing to \n for the new line.
    // @see calendarComponent::_size75()
    $vevent = str_replace("\n ", "", $vevent);

    $vevent = trim($vevent);
    $lines = explode("\n", $vevent);

    // Now build the vevent object and wrap it in this class.
    $vevent = new vevent_fixed();
    $vevent->setConfig('nl', "\n");
    $vevent->parse($lines);

    return new self($vevent);
  }

  public function __construct(vevent $vevent) {
    $this->vevent = $vevent;
  }

  /**
   * Returns a new iterator object for this VEvent for a given range.
   *
   * @param DateTime $start
   *   The start time of the range to generate. If not specified, the start
   *   date of the event is used.
   * @param DateTime $end
   *   The end time of the range to generate. If not specified, a default value
   *   is used. The default is some fixed period after the start date as
   *   specified by the 'ical_iterator_range_default' variable, which itself
   *   defaults to 1 year.
   * @return DrupalVEventIterator
   */
  public function getIteratorRange(DateTime $start = NULL, DateTime $end = NULL) {
    if (empty($start)) {
      $start = $this->getStart();
    }

    if (empty($end)) {
      $end_range = variable_get('ical_iterator_range_default', '+1 year');
      $end = clone($start);
      $end->modify($end_range);
    }

    $range = new DrupalVEventIteratorRange($this, $end, $start);
    return $range;
  }

  /**
   * Returns a new iterator object for this VEvent for a given number of dates.
   *
   * @param int $count
   *   The maximum number of instances to generate. The total number may be less.
   * @param DateTime $start
   *   The start time of the range to generate. If not specified, the start
   *   date of the event is used.
   * @param DateTime $end
   *   The end time of the range to generate. If not specified, a default value
   *   is used. The default is some fixed period after the start date as
   *   specified by the 'ical_iterator_range_default' variable, which itself
   *   defaults to 1 year.
   * @return DrupalVEventIteratorCount
   */
  public function getIteratorCount($count, DateTime $start = NULL, DateTime $end = NULL) {
    $count = new DrupalVEventIteratorCount(new IteratorIterator($this->getIteratorRange($start, $end)), 0, $count);
    return $count;
  }

  /**
   * Sets the start of the vevent.
   *
   * @param DateTime $start
   *   The DateTime object which should be the start of the vevent.
   * @return DrupalVEvent
   *   The called object.
   */
  public function setStart(DateTime $start) {
    //$this->vevent->setDtstamp($start->format('Y'), $start->format('m'), $start->format('d'), $start->format('H'), $start->format('i'), $start->format('s'), $start->getTimezone()->getName());
    $this->vevent->setDtstart($start->format('Y'), $start->format('m'), $start->format('d'), $start->format('H'), $start->format('i'), $start->format('s'), $start->getTimezone()->getName());

    return $this;
  }

  /**
   * Sets the end of the vevent.
   *
   * @param DateTime $end
   *   The DateTime object which should be the start of the vevent.
   * @return DrupalVEvent
   *   The called object.
   */
  public function setEnd(DateTime $end) {
    $this->vevent->setDtend($end->format('Y'), $end->format('m'), $end->format('d'), $end->format('H'), $end->format('i'), $end->format('s'), $end->getTimezone()->getName());

    return $this;
  }

  /**
   * Returns the start of this event.
   *
   * @return DateTime
   */
  public function getStart() {
    // The timezone may be stored either in the dtstart value or in the
    // properties array, because iCal is inconsistent and iCalCreator doesn't
    // hide that inconsistency. Fail.
    $data = $this->vevent->getProperty('dtstart', FALSE, TRUE);

    $start = empty($data['params']['TZID']) ? $this->veventDateToDateTime($data['value']) : $this->veventDateToDateTime($data['value'], new DateTimeZone($data['params']['TZID']));

    return $start;
  }

  /**
   * Returns the end of this event.
   *
   * @return DateTime
   *   If there is no end specified for this event, FALSE is returned instead.
   *   Remember to check the data type to ensure you don't try calling methods
   *   on a non-existent object!
   */
  public function getEnd() {
    // The timezone may be stored either in the dtstart value or in the
    // properties array, because iCal is inconsistent and iCalCreator doesn't
    // hide that inconsistency. Fail.
    $data = $this->vevent->getProperty('dtend', FALSE, TRUE);

    // If there is no end date return FALSE otherwise the code below would return
    // a garbage date.
    if (!$data) {
      return FALSE;
    }

    $end = empty($data['params']['TZID']) ? $this->veventDateToDateTime($data['value']) : $this->veventDateToDateTime($data['value'], new DateTimeZone($data['params']['TZID']));

    return $end;
  }

  /**
   * Returns an array of all rdates defined on this event.
   *
   * @return array
   *   An array of DateTime objects.
   */
  public function getRdates() {
    return $this->getREXDate('RDATE');
  }

  /**
   * Returns an array of all exdates defined on this event.
   *
   * @return array
   *   An array of DateTime objects.
   */
  public function getExdates() {
    return $this->getREXDate('EXDATE');
  }

  /**
   * Returns an array of all dates defined on this event with the specified field.
   *
   * @param string $field
   *   The field to check for dates.  Only RDATE and EXDATE are supported.
   *
   * @return array
   *   An array of DateTime objects.
   */
  protected function getREXDate($field) {
    $vevent = clone($this->vevent);
    $dates = array();

    // RDATEs and EXDATEs in iCal can be multi-value, but have only a single
    // shared timezone. iCalCreator does not hide this fact, but its default
    // retrival method ignores it.  Instead, we pull data out using the fancier
    // format that always return an array of dates and separate timezone.
    while ($data = $vevent->getProperty($field, FALSE, TRUE)) {
      $timezone = new DateTimeZone($data['params']['TZID']);
      foreach ($data['value'] as $date) {
        // Create the date in its native timezone.
        $dates[] = $this->vEventDateToDateTime($date, $timezone);
      }
    }

    return $dates;
  }

  /**
   * Add an Exclude date to the vevent.
   *
   * @param DateTime $exdate
   *   The date to exclude.
   * @return DrupalVEvent
   *   The called object.
   */
  public function addExdate(DateTime $exdate) {
    // vevent wants not a single date, but always an array of exdates.  It then
    // stores them as an array of exdates for each call. No, it doesn't make
    // the slightest bit of sense.  The timezone also needs to be specified only
    // in the $params, not in the exdate definition itself, because it gets
    // stripped out otherwise.  There is no explanation for that other than the
    // original author didn't believe in consistency.
    $exdates[] = array(
      'year' => $exdate->format('Y'),
      'month' => $exdate->format('m'),
      'day' => $exdate->format('d'),
      'hour' => $exdate->format('H'),
      'min' => $exdate->format('i'),
      'sec' => $exdate->format('s'),
      //'tz' => $exdate->format('Z'),
    );

    // TZID wants the timezone as a name, not as an offset.  I think.
    $params['TZID'] = $exdate->format('e');
    $this->vevent->setExdate($exdates, $params);

    return $this;
  }

  /**
   * Remove an exdate from the list of exdates.
   *
   * @param DateTime $exdate
   *   The date to remove from the exclusions list.
   * @return DrupalVEvent
   *   The object called.
   */
  public function removeExdate(DateTime $exdate) {
    // The vevent object does not provide any methods for removing elements
    // so we need to manipulate the internal variables and attempt to not
    // screw up anything. Yikes!
    if (!empty($this->vevent->exdate)) {
      // Tracking if one is found to do cleanup afterwards.
      $found = FALSE;

      foreach ($this->vevent->exdate as $key => $value) {
        $timezone = new DateTimeZone($value['params']['TZID']);

        // Tracking if one is found here to do cleanup if there are still values
        // and one is found. This if for internal pointers.
        $exfound = FALSE;

        foreach ($value['value'] as $k => $v) {
          $existing = $this->vEventDateToDateTime($v, $timezone);
          if ($exdate == $existing) {
            // We found an exisitng one so we remove it.
            unset($this->vevent->exdate[$key]['value'][$k]);

            // There is an internal countner we need to reset. If getProperty
            // is being used the counter for it will be reset. There is no other
            // way to ensute this is somewhat safe.
            if (isset($this->vevent->propix['EXDATE'])) {
              unset($this->vevent->propix['EXDATE']);
              $found = TRUE;
              $exfound = TRUE;
            }

            // We need to check if there are any other values on that exdate.
            if (count($this->vevent->exdate[$key]['value'][$k]) == 0) {
              // There are no more values on this exdate so we can remove it.
              unset($this->vevent->exdate[$key]);
              continue; // There is nothing left to loop over so we can leave.
            }
          }
        }
        if ($exfound && !empty($this->vevent->exdate[$key])) {
          // One was found and there are still dates on the exdate.
          $this->vevent->exdate[$key]['values'] = array_values($this->vevent->exdate[$key]['values']);
        }
      }

      if ($found) {
        // We found at least one and need to reset the keys to make sure it is
        // nicely ordered for the getProperty/propix ordering used elsewhere.
        $this->vevent->exdate = array_values($this->vevent->exdate);
      }
    }

    return $this;
  }

  /**
   * Add an RDATE recurrence date to the vevent.
   *
   * @param DateTime $rdate
   * @return DrupalVEvent
   *   The called object.
   */
  public function addRdate(DateTime $rdate) {
    // vevent wants not a single date, but always an array of rdates.  It then
    // stores them as an array of rdates for each call. No, it doesn't make
    // the slightest bit of sense.  The timezone also needs to be specified only
    // in the $params, not in the exdate definition itself, because it gets
    // stripped out otherwise.  There is no explanation for that other than the
    // original author didn't believe in consistency.
    $rdates[] = array(
      'year' => $rdate->format('Y'),
      'month' => $rdate->format('m'),
      'day' => $rdate->format('d'),
      'hour' => $rdate->format('H'),
      'min' => $rdate->format('i'),
      'sec' => $rdate->format('s'),
      //'tz' => $exdate->format('Z'),
    );

    // TZID wants the timezone as a name, not as an offset.  I think.
    $params['TZID'] = $rdate->format('e');
    $this->vevent->setRdate($rdates, $params);

    return $this;
  }

  /**
   * Converts from the date array format of the vevent object to a PHP DateTime.
   *
   * @param $date
   *   The array format used by the vevent object for storing dates.
   * @param DateTimeZone $timezone
   *   The timezone to use for the resulting date object.  Either this value
   *   must be specified or the $date array must include a 'tz' key with a
   *   timezone value. If neither one is specified, the user's default timezone
   *   is used.
   * @return DateTime
   *   The DateTime object corresponding to $date, give or take the timezone
   *   handling above.
   */
  public function vEventDateToDateTime($date, DateTimeZone $timezone = NULL) {
    // Default to the specified timezone, then to the timezone embedded in the
    // date information if any, and finally to whatever the user's timezone is
    // set to.
    if (!$timezone) {
      if (!empty($date['tz'])) {
        if ($date['tz'] == 'Z') {
          $date['tz'] = 'UTC';
        }
        $timezone = new DateTimeZone($date['tz']);
      }
      else {
        // When we have no timezone we use the default timezone for the user.
        $timezone = new DateTimeZone(date_default_timezone_name(TRUE));
      }
    }

    $datetime = new DateTime('now', $timezone);
    $datetime->setDate($date['year'], $date['month'], $date['day']);

    // All day events have an hour/minute/second not set.
    if (!isset($date['hour'], $date['min'], $date['sec'])) {
      $datetime->setTime(0, 0, 0);
    }
    else {
      $datetime->setTime($date['hour'], $date['min'], $date['sec']);
    }

    return $datetime;
  }

  /**
   * Determines if this vevent will have recurrences after the specified date.
   *
   * Note: This method is not guaranteed to be perfectly accurate. We err on
   * the side of saying there will be future instances if we're not sure.
   *
   * @param DateTime $date
   *   The date after which we want to check for additional instances. Default
   *   is right now.
   * @param int $num_existing
   *   The number of instances we know already exist.  This is needed to check
   *   against the COUNT restriction on an RRULE.
   *
   * @return boolean
   */
  public function hasFutureInstances(DateTime $date = NULL, $num_existing = 0) {

    if (empty($date)) {
      $date = new DateTime();
    }

    // Check if the start itself is in the future.
    if ($this->getStart() > $date) {
      return TRUE;
    }

    // Check if there are any rdates in the future.
    foreach ($this->getRdates() as $rdate) {
      if ($rdate > $date) {
        return TRUE;
      }
    }

    // Check for the UNTIL and COUNT properties of any RRULES.
    $vevent = clone($this->vevent);
    while ($rrule = $vevent->getProperty('RRULE')) {
      if (!empty($rrule['UNTIL'])) {

        // There are cases where the UNTIL value can be an array of information.
        // So, we need to convert it to a string.
        if (is_array($rrule['UNTIL'])) {
          $rrule['UNTIL'] = $this->vevent->_format_date_time($rrule['UNTIL']);
        }

        // UNTIL is specified in the iCal spec to always be UTC.
        $until = new DateTime($rrule['UNTIL'], new DateTimeZone('UTC'));
        if ($until > $date) {
          return TRUE;
        }
      }
      elseif (!empty($rrule['COUNT'])) {
        if ($num_existing) {
          return ($num_existing < $rrule['COUNT']);
        }
        else {
          // If we didn't say how many instances have already been created,
          // assume that there will be more so that we don't miss instances.
          return TRUE;
        }
      }
      else {
        // The RRULE has no COUNT or UNTIL, so it continues forever.
        return TRUE;
      }
    }

    // There's nothing to indicate there may be future events.
    return FALSE;
  }

  /**
   * Check if a vevent has canceled set.
   *
   * @return bool
   *   TRUE: The vevent is canceled.
   *   FALSE: The vevent is NOT canceled.
   */
  public function isCanceled() {
    $status = FALSE;
    if ($this->vevent->getProperty('status') == 'CANCELLED') {
      $status = TRUE;
    }
    return $status;
  }

  /**
   * Is the vevent an all day event (no time associated with the date).
   *
   * return bool
   *  TRUE if the vevent is all day.
   *  FALSE if the event has times associsted with it.
   */
  public function isAllDay() {
    $ret = FALSE;
    $start = $this->vevent->getProperty('dtstart', FALSE, TRUE);

    // If there is no time associated with an event the hour, minute, and second
    // should be set to false.
    if (!isset($start['value']['hour']) && !isset($start['value']['min']) && !isset($start['value']['sec'])) {
      $ret = TRUE;
    }

    // @todo Should we have some end checking as well?

    return $ret;
  }

  /**
   * Passes through any undefined calls to the underlying vevent object.
   */
  public function __call($method, $args) {
    return call_user_func_array(array($this->vevent, $method), $args);
  }

  public function __clone() {
    $this->vevent = clone($this->vevent);
  }

  public function __get($property) {
    return $this->vevent->{$property};
  }
}

/**
 * Iterator for a range of event instances.
 */
class DrupalVEventIteratorRange implements IteratorAggregate, Countable {

  /**
   * The VEvent object we are wrapping
   *
   * @var DrupalVEvent
   */
  public $vevent;

  /**
   * The first date in the range we want.
   *
   * If this date is not a value that would be generated from this vevent, the
   * first instance will be the first instance after this date.
   *
   * @var DateTime
   */
  protected $start;

  /**
   * The last date in the range we want.
   *
   * If this date is not a value that would be generated from this vevent, the
   * last instance will be the last instance before this date.
   *
   * @var DateTime
   */
  protected $end;

  /**
   * The number of instances to generate at maximum.
   *
   * 0 means no limit.  In that case, $end must be specified in order to avoid
   * creating an infinite loop.
   *
   * @var int
   */
  protected $count = 0;

  /**
   * An array of instances that have been generated.
   *
   * @var array
   */
  protected $dates = array();

  /**
   * The internal position pointer of the iterator.
   *
   * @var int
   */
  protected $position = 0;

  /**
   * Constructs a new event range iterator.
   *
   * Either $end or $count must be specified.  Failure to do so would result in
   * an infinite loop, and therefore an exception is thrown instead.  Specifying
   * both is allowed.
   *
   * @param DrupalVEvent $vevent
   *   The vevent we are iterating.
   * @param DateTime $end
   *   The end date for this range.
   * @param DateTime $start
   *   The start time for this range.  If not specified it will be derived from
   *   the vevent.
   */
  public function __construct(DrupalVEvent $vevent, DateTime $end, DateTime $start = NULL) {

    // We are going to be pulling data out of the $vevent object, which increments
    // various internal counters.  Those counters do not have a reset mechanism.
    // That is moronically stupid of vevent, but it is what it is.  The easiest
    // way around that is to simply clone it before we start using it.  That way
    // subsequent iterator objects will not have to worry about the object being
    // "already used".
    // See calendarComponent::getProperty() for the (previously undefined)
    // $this->propix variable to see where this happens.
    $this->vevent = clone($vevent);

    $this->start = $start;
    $this->end = $end;

    if (empty($this->start)) {
      $this->deriveStart();
    }

    $this->dates = $this->calculateDates($this->start, $this->end);
  }

  /**
   * Derives the first instance of the event.
   *
   * The first instance of this range will be either the starting date in the
   * vevent itself or the specified start date, whichever is later.
   *
   * Later logic relies on the first instance already being known.  Fortunately
   * that is easy to determine.
   *
   * @todo This is buggy, as it assumes that the specified start date will be an
   *       instance in the range.  If it's not, the first instance may be wrong.
   *       We'll deal with that later.
   */
  /*
  protected function deriveFirstDate() {
    // If there is no first date yet, then we assume it to be either the specified
    // start date or the date of the vevent itself, whichever is later.
    if (empty($this->dates[0])) {
      $dtstart = $this->vevent->vEventDateToDateTime($this->vevent->getProperty('dtstart'));

      // We need to normalize the datetimes to the same timezone for comparison.
      $dtstart_compare = clone($dtstart);
      $start_compare = clone($this->start);
      $zone = new DateTimezone('UTC');
      $dtstart_compare->setTimezone($zone);
      $start_compare->setTimezone($zone);

      // If the two dates are identical then it doesn't really matter which one
      // we use.  Actually it would matter if the timezones were different, but
      // we're going to conveniently ignore that for now unless it becomes a
      // problem.
      $this->dates[0] = $dtstart_compare->format('c') > $start_compare->format('c') ? $dtstart : $this->start;
    }
  }
  */

  /**
   * Derives the start instance of this range from the vevent object.
   */
  protected function deriveStart() {

    if ($dtstart = $this->vevent->getProperty('dtstart')) {
      $start = $this->vevent->vEventDateToDateTime($dtstart);

      $this->start = $start;
    }
    else {
      // How else would we get it?
    }
  }

  /**
   * Calculates the dates that this event will occur.
   *
   * Note that it is assumed that $start is a valid instance of the recurrence.
   * If it's not, then the results may be unpredictable.  Times may be off,
   * for instance.
   *
   * @param DateTime $start
   *   The datetime of the first instance.
   * @param DateTime $end
   *   The datetime past which we do not want any instances.
   * @return array
   *   An array of DateTime objects.
   */
  protected function calculateDates(DateTime $start, DateTime $end) {

    $return = array();
    $start_zone = $start->getTimezone();
    $exclude_dates = array();
    $dates = array();

    // The RRULE should regenerate this value, but in case there are no RRULES
    // and this vevent has only DTSTART and RDATE record we need to ensure
    // that it's here.  Duplicate checking later on will strip it out again if
    // necessary.
    $dates[] = $start->format('Y-m-d H:i:s');

    // Most properties of vevent are technically a multi-value array, and getProperty()
    // increments a counter.  We therefore need to clone it so that this method
    // can be called multiple times without breaking, since there is no reset
    // on vevent. Ick.
    $vevent = clone($this->vevent);

    // First calculate the EXRULEs, which are dates that should NOT have an instance.
    $rules = $vevent->createExrule();
    if (is_string($rules)) {
      $rules = array($rules);
    }
    if ($rules) {
      $exclude_dates = $this->calculateRules($rules, $start, $end);
    }

    // Now add in any EXDATE entries, normalizing them to the same timezone.
    foreach ($vevent->getExdates() as $date) {
      $date->setTimezone($start_zone);
      $exclude_dates[] = $date->format('Y-m-d H:i:s');
    }

    // Now calculate the RRULEs.  We don't try to exclude dates here as that
    // would still not catch RDATEs.
    $rules = $vevent->createRrule();
    if (is_string($rules)) {
      $rules = array($rules);
    }
    if ($rules) {
      $dates = $this->calculateRules($rules, $start, $end);
    }

    // Now add in any RDATE entries, normalizing them to the same timezone.
    foreach ($vevent->getRdates() as $date) {
      $date->setTimezone($start_zone);
      $dates[] = $date->format('Y-m-d H:i:s');
    }

    // Absent any reason to do otherwise, we'll normalize the instances to use
    // the timezone of the start.
    $excludes = array();
    foreach ($exclude_dates as $date) {
      $date = new DateTime($date, $start_zone);
      $excludes[] = $date;
    }

    // In case we ended up with multiple duplicate dates (specified in an RRULE
    // and an RDATE for example), drop the duplicates because the iCal spec says
    // to do so.
    $dates = array_unique($dates);

    // The format we're storing dates in is lexically sortable, since we know
    // everything is in the same timezone.
    sort($dates);

    foreach ($dates as $date) {
      $date = new DateTime($date, $start_zone);
      if (!in_array($date, $excludes)) {
        $return[] = $date;
      }
    }

    return $return;
  }

  /**
   * Calculate the dates matching an RRULE or EXRULE
   *
   * Fortunately RRULES and EXRULES use the same syntax, so we can process them
   * with the same logic.
   *
   * @param array $rules
   *   An array of RRULES or EXRULES to process.
   * @param DateTime $start
   *   The first date of the recurrence.  This will be the first instance, so
   *   it must be a date that would match the rule.
   * @param DateTime $end
   *   The last date of the rule.  This may or may not be an instance within
   *   this set, and if the rule has an UNTIL or LIMIT in it then we may
   *   never erach this value.  It provides an end-case past which we won't go
   *   as a last resort.
   * @return array
   *   An array of timestamp strings that match the specified rule, normalized
   *   to UTC timezone.
   */
  protected function calculateRules(array $rules, DateTime $start, DateTime $end, array $excludes = array()) {
    $dates = array();

    foreach ($rules as $rule) {
      // Unfold the rule if it has been wrapped to multiple lines, then remove
      // extraneous newlines.
      $rule = trim(str_replace("\n ", '', $rule));

      $dates += date_repeat_calc($rule, $start->format('c'), $end->format('c'), $excludes, $start->getTimezone()->getName());
    }

    return $dates;
  }

  public function getIterator() {
    return new ArrayIterator($this->dates);
  }

  public function count() {
    return count($this->dates);
  }
}

/**
 * Limit iterator for DrupalVEvent instances.
 *
 */
class DrupalVEventIteratorCount extends LimitIterator implements Countable {

  /**
   * The number of elements that we are iterating over.
   *
   * This value may be smaller than the specified count if the original iterator
   * started with fewer than that number of elements.
   *
   * @var int
   */
  protected $numElements;

  public function __construct(Iterator $iterator, $offset = 0, $count = -1) {
    parent::__construct($iterator, $offset, $count);

    $num_outer = count($iterator) - $offset;
    $this->numElements = ($count == -1) ? $num_outer : min($count, $num_outer);
  }

  public function count() {
    return $this->numElements;
  }
}

