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
  'required context' => new ctools_context_required('title','taxonomy_term'),
);

/**
 * Render the Queue status.
 */
function pp_frontpage_most_content_type_render($subtype, $conf, $panel_args, $context) {

  if (isset($context) && isset($context->data) && isset($context->data->tid)) {
    // Localising of user group display to.
    $user_lists_object = $context->data;
    $user_lists_tid = $context->data->tid;

    $block = new stdClass();
    $block->module = 'pp_frontpage';
    $block->title = t('Most active users');
    $output = '';

    $cache = cache_get(__FUNCTION__);
    if (!empty($cache)) {
      $block->content = $cache->data;
      return $block;
    }

    $output = '<div class="bs-callout bs-callout-info">' . t(
        'Statistics for the last 12 years'
      ) . '</div>';
    $query = 'SELECT pp.doid, SUM(pp.items) as counter  FROM {ppgetstat} pp '
      . 'INNER JOIN field_data_field_user_id fn ON fn.field_user_id_value = pp.doid '
      . 'INNER JOIN field_data_field_user_lists fu ON fu.entity_id = fn.entity_id '
      . 'WHERE pp.type = :type AND pp.timestamp > :timestamp AND fu.field_user_lists_tid = :user_list_tid '
      . 'GROUP BY pp.doid ORDER BY counter DESC LIMIT 0,200';

    $last_thee_months = REQUEST_TIME - PPGETSTAT_DEFAULT_TIME_PERIOD_FOR_SCANNING;

    $types = array(
      'Commits' => PPGETSTAT_TYPE_COMMITS,
      'Comments' => PPGETSTAT_TYPE_COMMENTS,
      //   'Core commits' => PPGETSTAT_TYPE_CORE_COMMITS,
      //  'Github.com activity' => PPGETSTAT_TYPE_GITHUB_ACTIVITY,
    );
    $table_header = array(
      t('Position'),
      t('user'),
      t('companies worked for'),
      t('counter'),
      t('D.org link')
    );
    foreach ($types as $title => $data_type) {
      $query_results = db_query(
        $query,
        array(
          ':type' => $data_type,
          ':timestamp' => $last_thee_months,
          ':user_list_tid' => $user_lists_tid,
        )
      )->fetchAllKeyed();
      $doids = array_keys($query_results);

      $user_nodes = _ppgetstat_get_users_by_doids($doids);
      $table_data = array();
      $pos = 1;
      foreach ($query_results as $doid => $counter) {
        $user_node = $user_nodes[$doid];
        $company_column = '';
        if (!empty($user_node->field_organisations)) {
          $company_titles = array();
          $companies = field_get_items(
            'node',
            $user_node,
            'field_organisations'
          );
          foreach ($companies as $company) {
            $company_titles[] = taxonomy_term_load($company['tid'])->name;
          }
          $company_column = implode(',', $company_titles);
        }
        $table_data[] = array(
          $pos,
          _ppgetstat_get_node_link_by_doid($doid),
          $company_column,
          $counter,
          l(
            'http://drupal.org/user/' . $doid,
            'http://drupal.org/user/' . $doid,
            array('external' => TRUE)
          )
        );
        $pos++;
      }
      $output .= '<div class="panel panel-primary">';
      $output .= '<div class="panel-heading">' . t($title) . '</div>';
      $output .= '<div class="panel-body">' . theme(
          'table',
          array('header' => $table_header, 'rows' => $table_data)
        ) . '</div>';
      $output .= '</div>';
    }

    cache_set(
      __FUNCTION__ . ':' . $user_lists_tid,
      $output,
      'cache',
      CACHE_TEMPORARY
    );

    $block->content = $output;
    return $block;
  }
}
