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

  $table_data = _ppgetstat_get_commits_data($node);

  if (is_string($table_data)) {
    return $table_data;
  }

  $variables = array(
    'header' => array(t('Date'), t('Commits')),
    'rows' => $table_data,
  );
  $output = theme('table', $variables);

  return $output;
}

/**
 * Generate tsv file for chart.
 */
function ppgetstat_commits_tsv_source($node) {
  drupal_add_http_header('Cache-Control', 'private, max-age=' . 60 * 60 * 24);
  drupal_add_http_header('Content-Type', 'application/octet-stream');
  drupal_add_http_header('Content-Transfer-Encoding', 'binary');

  $data = _ppgetstat_get_commits_data($node);

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
 * Helper function to load the Commits stats.
 */
function _ppgetstat_get_commits_data($node) {
  if (!isset($node->field_user_id[LANGUAGE_NONE][0]['value'])) {
    return t('Please provide drupal uid of this user.');
  }

  $do_uid = $node->field_user_id[LANGUAGE_NONE][0]['value'];

  $db_data = db_query('SELECT timestamp, items FROM {ppgetstat} WHERE doid = :doid AND type = :type ORDER BY timestamp DESC',
    array(':doid' => $do_uid, ':type' => PPGETSTAT_TYPE_COMMITS)
  )->fetchAllKeyed();

  $beginning = REQUEST_TIME - PPGETSTAT_DEFAULT_TIME_PERIOD_FOR_SCANNING;
  $timestamp = $beginning - ($beginning % PPGETSTAT_TIME_PERIOD_GRANULARITY);

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

    $timestamp += PPGETSTAT_TIME_PERIOD_GRANULARITY;
  }

  return $table_data;
}

/**
 * Form builder for reset stats form.
 */
function ppgetstat_reset_form($form, $form_state) {

  $form['markup'] = array(
    '#type' => 'item',
    '#markup' => t('Submitting this form will remove all the statistics, queues content and last scan time so you can run scanning from scratch.'),
  );

  $form['stats_period'] = array(
    '#type' => 'textfield',
    '#title' => t('Scanning period'),
    '#element_validate' => array('element_validate_integer_positive'),
    '#default_value' => variable_get('ppgetstat_stats_period', 6 * 30 * 24 * 60 * 60),
    '#description' => t('For debugging purposes we can shorten period of grabbing stats. By default it is half a year 15552000, but we can set it to one month 2592000.')
  );

  $titles = array();
  foreach (node_load_multiple(array(), array('type' => PPGETSTAT_USER_NODE_TYPE)) as $user_node) {
    $titles[] = $user_node->title;
  }

  $form['users'] = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#title' => t('Clear stats by user'),
    '#options' => $titles,
    '#default_value' => '',
    '#prefix' => t('or')
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
  );

  return $form;
}

/**
 * Submit handler for reset form.
 */
function ppgetstat_reset_form_submit($form, $form_state) {
  if (!empty($form_state['values']['stats_period'])) {
    variable_set('ppgetstat_stats_period', $form_state['values']['stats_period']);

    variable_del('ppgetstat_last_statsjobs_timestamp');
    db_query('TRUNCATE TABLE {ppgetstat}');

    drupal_set_message(t('Statistics resetted.'));
  }
  elseif (!empty($form_state['values']['users'])) {
    $users = node_load_multiple($form_state['values']['users']);

    $doids = array();
    foreach ($users as $user) {
      if (!empty($user->field_user_id[LANGUAGE_NONE][0]['value'])) {
        $doids[] = $user->field_user_id[LANGUAGE_NONE][0]['value'];
      }
    }

    db_delete('ppgetstat')
      ->condition('doid', $doids, 'IN')
      ->execute();

    drupal_set_message(t('Statistics for given users was cleared.'));
  }

  $defined_queues = array();

  if (function_exists('queue_ui_defined_queues')) {
    $defined_queues = queue_ui_defined_queues();
  }

  foreach (array('stats_jobs', 'dorg_scrapping') as $queue_name) {
    $queue = DrupalQueue::get($queue_name);

    if (isset($defined_queues[$queue_name]['delete'])) {
      $function = $defined_queues[$queue_name]['delete'];
      $function($queue);
    }

    $queue->deleteQueue();
  }
}
