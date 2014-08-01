<?php

/**
 * @file
 * Best contributors.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Most counters'),
  'single' => TRUE,
  'category' => t('PP'),
);

/**
 * Render the Queue status.
 */
function pp_frontpage_most_content_type_render($subtype, $conf, $panel_args, $context) {
  $block = new stdClass();
  $block->module = 'pp_frontpage';
  $block->title = t('Most active users');

  $cache = cache_get(__FUNCTION__);
  if (!empty($cache)) {
    $block->content = $cache->data;
    return $block;
  }

  $output = t('Statistics for last three months');

  $query = 'SELECT doid, SUM(items) as counter  FROM {ppgetstat} '
    . 'WHERE type = :type AND timestamp > :timestamp '
    . 'GROUP BY doid ORDER BY counter DESC LIMIT 0,5';

  $last_thee_months = REQUEST_TIME - PPGETSTAT_DEFAULT_TIME_PERIOD_FOR_SCANNING;

  $types = array(
    'Commits' => PPGETSTAT_TYPE_COMMITS,
    'Comments' => PPGETSTAT_TYPE_COMMENTS,
    'Core commits' => PPGETSTAT_TYPE_CORE_COMMITS,
    'Github.com activity' => PPGETSTAT_TYPE_GITHUB_ACTIVITY,
  );
  $table_header = array(t('user'), t('counter'));
  foreach ($types as $title => $data_type) {
    $query_results = db_query($query, array(
      ':type' => $data_type,
      ':timestamp' => $last_thee_months,
    ))->fetchAllKeyed();

    $table_data = array();
    foreach ($query_results as $doid => $counter) {
      $table_data[] = array(_ppgetstat_get_node_link_by_doid($doid), $counter);
    }

    $output .= '<h3>' . t($title) . '</h3>';
    $output .= theme('table', array('header' => $table_header, 'rows' => $table_data));
  }

  cache_set(__FUNCTION__, $output, 'cache', CACHE_TEMPORARY);

  $block->content = $output;
  return $block;
}
