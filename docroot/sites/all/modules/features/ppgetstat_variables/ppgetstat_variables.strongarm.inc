<?php
/**
 * @file
 * ppgetstat_variables.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function ppgetstat_variables_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ppgetstat_pages_to_scan';
  $strongarm->value = '15';
  $export['ppgetstat_pages_to_scan'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ppgetstat_users';
  $strongarm->value = 'podarok,podarok_auto
ygerasimov';
  $export['ppgetstat_users'] = $strongarm;

  return $export;
}
