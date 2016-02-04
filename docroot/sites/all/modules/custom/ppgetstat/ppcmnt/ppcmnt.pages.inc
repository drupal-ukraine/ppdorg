<?php

/**
 * @file
 * Separate file with page callbacks.
 */

/**
 * Page callback to display commits statistics.
 */
function ppcmnt_comments_page($node) {
  drupal_set_title(t('Comments statistics'));

  $table_data = _pppcmnt_load_the_data($node);

  if (is_string($table_data)) {
    return $table_data;
  }

  $variables = array(
    'header' => array(t('Date'), t('Comments')),
    'rows' => $table_data,
  );
  $output = theme('table', $variables);

  return $output;
}

/**
 * Print stats for visualization.
 */
function ppcmnt_comments_tsv_source($node) {
  drupal_add_http_header('Cache-Control', 'private, max-age=' . 60 * 60 * 24);
  drupal_add_http_header('Content-Type', 'application/octet-stream');
  drupal_add_http_header('Content-Transfer-Encoding', 'binary');

  $data = _pppcmnt_load_the_data($node);

  print t('date	close') . "\n";

  if (is_string($data)) {
    return;
  }

  foreach ($data as $value) {
    list($date, $number) = $value;
    print str_replace(' ', '-', $date) . '	' . $number . "\n";
  }

  drupal_exit();
}

/**
 * Load the stats data for this user.
 */
function _pppcmnt_load_the_data($node) {
  if (!isset($node->field_user_id[LANGUAGE_NONE][0]['value'])) {
    return t('Please provide drupal uid of this user.');
  }

  $do_uid = $node->field_user_id[LANGUAGE_NONE][0]['value'];

  $db_data = db_query('SELECT timestamp, COUNT(items) FROM {ppgetstat} WHERE doid = :doid AND type = :type GROUP BY timestamp ORDER BY timestamp DESC',
    array(':doid' => $do_uid, ':type' => PPGETSTAT_TYPE_COMMENTS)
  )->fetchAllKeyed();

  $beginning = REQUEST_TIME - PPGETSTAT_DEFAULT_TIME_PERIOD_FOR_SCANNING;
  $timestamp = $beginning - ($beginning % PPGETSTAT_TIME_WEEK_GRANULARITY);

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

    $timestamp += PPGETSTAT_TIME_WEEK_GRANULARITY;
  }

  return $table_data;
}
