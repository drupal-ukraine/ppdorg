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

  $db_data = db_query('SELECT timestamp, COUNT(items) FROM {ppgetstat} WHERE doid = :doid AND type = :type GROUP BY timestamp ORDER BY timestamp DESC',
    array(':doid' => $do_uid, ':type' => PPGETSTAT_TYPE_COMMITS)
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

/**
 * Form builder for reset stats form.
 */
function ppgetstat_reset_form(array $form, array &$form_state) {
  $titles = array();

  foreach (node_load_multiple(array(), array('type' => PPGETSTAT_USER_NODE_TYPE)) as $user_node) {
    $titles[] = $user_node->title;
  }

  $form['markup'] = array(
    '#type' => 'item',
    '#markup' => t('Submitting this form will remove all the statistics, queues content and last scan time so you can run scanning from scratch.'),
  );

  $form['stats_period'] = array(
    '#type' => 'textfield',
    '#title' => t('Scanning period (number of months)'),
    '#element_validate' => array('element_validate_integer_positive'),
    '#default_value' => PPGETSTAT_DEFAULT_TIME_PERIOD_FOR_SCANNING / PPGETSTAT_TIME_MONTH_GRANULARITY,
    '#description' => t('For debugging purposes we can shorten period of grabbing stats.'),
  );

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
function ppgetstat_reset_form_submit(array $form, array &$form_state) {
  if (!empty($form_state['values']['stats_period'])) {
    ppgetstat_stats_period($form_state['values']['stats_period']);

    variable_del('ppgetstat_last_statsjobs_timestamp');
    db_query('TRUNCATE TABLE {ppgetstat}');

    variable_del('ppgetstat_ppcc_last_commit_timestamp');
    variable_del('ppgetstat_ppcmnt_last_comment_timestamp');
    variable_del('ppgetstat_ppgetstat_last_commit_timestamp');
    variable_del('ppgetstat_last_statsjobs_timestamp');

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

/**
 * Form builder for reset stats form.
 */
function ppgetstat_sync_user_list($form, $form_state) {
  // Getting list of all available users list for analyzing.
  $user_lists_vocab = taxonomy_vocabulary_machine_name_load('users_list');
  $terms = taxonomy_get_tree($user_lists_vocab->vid);
  $options = array(
    0 => t('- None -'),
  );
  foreach ($terms as $term) {
    $options[$term->tid] = $term->name;
  }
  $form['users_list'] = array(
    '#type' => 'select',
    '#title' => t('Select users group scan to'),
    '#options' => $options,
  );

  $form['direct_user'] = array(
    '#type' => 'textfield',
    '#title' => t('Direct user rescan'),
    '#description' => t('If this field is not empty, only this user would be updated'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('test request')
  );

  return $form;
}

/**
 * Submit callback for sync user list form
 */
function ppgetstat_sync_user_list_submit($form, $form_state) {
  $direct_user = $form_state['values']['direct_user'];
  if (!empty($direct_user)) {
    $r = _ppgetstat_api_call(
      'https://www.drupal.org/',
      'api-d7/user.json',
      array(
        'uid' => $direct_user
      )
    );
    $users_data = array(
      $r->list[0]->uid => $r->list[0]
    );

    _ppgetstat_process_dorg_users($users_data, array($r->list[0]->uid));
    drupal_set_message('User has been updated');

  } else {
    $users_list_tid = $form_state['values']['users_list'];
    $term = taxonomy_term_load($users_list_tid);
    if (!empty($term)) {
      $field_query = field_get_items('taxonomy_term', $term, 'field_query');
      $variables = array('limit' => 7);
      foreach ($field_query as $item) {
        $variables[$item['first']] = $item['second'];
      }

      $r = _ppgetstat_api_call(
        'https://www.drupal.org/',
        'api-d7/user.json',
        $variables
      );

      if (!empty($r) && (!empty($r->last))) {
        $last_url = parse_url($r->last);
        parse_str($last_url['query'], $params);
        $page = $params['page'];
        for ($i = ($page - 1); $i >= 0; $i--) {
          $operations[] = array(
            '_ppgetstat_sync_user_list_step',
            array($variables, $i),
          );
        }
        $batch = array(
          'operations' => $operations,
          'finished' => '_ppgetstat_batch_finished',
          'title' => t('Processing import...'),
          'init_message' => t('Batch is starting.'),
        );
        batch_set($batch);
      }
      else {
        drupal_set_message('Error with getting initial data', 'error');
      }

    }
    else {
      drupal_set_message('Import process could not be started');
    }
  }
}


