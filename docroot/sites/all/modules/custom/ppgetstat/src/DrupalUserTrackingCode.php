<?php

/**
 * @file
 * Class for manipulating with drupal.org/user/ID/track/code list.
 */
class DrupalUserTrackingCode {

  /**
   * Timestamp until dig into the past.
   *
   * @var
   */
  protected $date;

  /**
   * User profile url. http://drupal.org/user/12345 only. No aliases.
   * @var
   */
  protected $profile;

  /**
   * Array of checked page ids.
   * @var
   */
  protected $ids;
  /**
   * Init object with date limit.
   *
   * DrupalUserTrackingCode constructor.
   * @param string $date
   */
  public function __construct($profile = '', $date = '') {
    $this->date = $date;
    $this->profile = $profile;
    $this->ids = new ArrayIterator();
  }

  /**
   * Get page counter.
   *
   * @return int
   */
  public function getCount($id = 0) {
    if ($id == 0){
      return 1;
    }
    $page = _ppgetstat_fetch_page($this->profile . '/track/code?page=' . $id);
    preg_match_all("/<li class=\"pager-next last\"><a href=\".*\">››<\/a><\/li>/miU", $page, $match);
    if (isset($match[0][0])) {
      $this->ids->offsetSet($id, $id);
      $this->ids->next();
      $this->getCount($this->nextUpId($id));
    }
    else {
      $this->ids->offsetSet($id, FALSE);
      $this->getCount($this->nextDownId($id));
    }
  }

  private function nextUpId($current = 0) {
    $this->ids->seek($current);

    switch ($current) {
      case 0:
        return 100;
      default:
        return $current + $current;
    }
  }

  private function nextDownId($current = 0) {
    switch ($current) {
      case 0:
        return 0;
      default:
        return round(0.75 * $current);
    }
  }


}