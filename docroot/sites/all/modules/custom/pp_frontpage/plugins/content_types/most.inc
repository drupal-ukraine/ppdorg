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

  $query = 'SELECT doid, SUM(items) as counter  FROM {ppgetstat} '
    . 'WHERE type = :type AND timestamp > :timestamp '
    . 'GROUP BY doid ORDER BY counter DESC LIMIT 0,100';

  $last_thee_months = REQUEST_TIME - PPGETSTAT_DEFAULT_TIME_PERIOD_FOR_SCANNING;

  $types = array(
    'Commits' => PPGETSTAT_TYPE_COMMITS,
    'Comments' => PPGETSTAT_TYPE_COMMENTS,
 //   'Core commits' => PPGETSTAT_TYPE_CORE_COMMITS,
    'Github.com activity' => PPGETSTAT_TYPE_GITHUB_ACTIVITY,
  );
  $table_header = array(t('user'), t('company') , t('counter'), t('D.org link'));
  foreach ($types as $title => $data_type) {
    $query_results = db_query($query, array(
      ':type' => $data_type,
      ':timestamp' => $last_thee_months,
    ))->fetchAllKeyed();
    $doids = array_keys($query_results);

    $user_nodes = _ppgetstat_get_users_by_doids($doids);
    $table_data = array();
    foreach ($query_results as $doid => $counter) {
      $user_node = $user_nodes[$doid];
      $company_column = '';
      if (!empty($user_node->field_organisations)) {
        $company_titles = array();
        $companies = field_get_items('node', $user_node, 'field_organisations');
        foreach ($companies as $company) {
          $company_titles[] = taxonomy_term_load($company['tid'])->name;
        }
        $company_column = implode(',', $company_titles);
      }
      $table_data[] = array(
        _ppgetstat_get_node_link_by_doid($doid),
        $company_column ,
        $counter,
        l('http://drupal.org/user/' . $doid ,'http://drupal.org/user/' . $doid, array('external' => true))
        );
    }

    $output = '<h3>' . t($title) . '</h3>';
    $output .= theme('table', array('header' => $table_header, 'rows' => $table_data));
  }

  cache_set(__FUNCTION__, $output, 'cache', CACHE_TEMPORARY);

  $block->content = $output;
  return $block;
}
