<?php

/**
 * @file
 * Separate file with page callbacks.
 */

/**
 * Page callback to display commits statistics.
 */
function ppgetstat_commits_page($node) {
  drupal_set_title(t('Commits statistics'));

  if (!isset($node->field_user_id[LANGUAGE_NONE][0]['value'])) {
    return t('Please provide drupal uid of this user.');
  }

  $do_uid = $node->field_user_id[LANGUAGE_NONE][0]['value'];

  $db_data = db_query('SELECT timestamp, commits FROM {ppgetstat_commits} WHERE doid = :doid ORDER BY timestamp DESC',
    array(':doid' => $do_uid)
  )->fetchAllKeyed();

  $beginning = REQUEST_TIME - PPGETSTST_DEFAULT_TIME_PERIOD_FOR_SCANNING;
  $timestamp = $beginning - ($beginning % PPGETSTST_TIME_PERIOD_GRANULARITY);

  $table_data = array();
  while ($timestamp < REQUEST_TIME) {
    $commits = 0;
    if (isset($db_data[$timestamp])) {
      $commits = $db_data[$timestamp];
    }
    $table_data[] = array(
      format_date($timestamp, 'custom', 'j M y'),
      $commits,
    );

    $timestamp += PPGETSTST_TIME_PERIOD_GRANULARITY;
  }

  $variables = array('header' => array(t('Date'), t('Commits')), 'rows' => $table_data);
  $output = theme('table', $variables);

  return $output;
}
