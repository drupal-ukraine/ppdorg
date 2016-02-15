<?php

$plugin = array(
  'title' => t('Users registration stat by year'),
  'single' => TRUE,
  'category' => t('PP'),
  'required context' => new ctools_context_required('title','taxonomy_term'),
);

/**
 * Render users by year statistics.
 */
function pp_frontpage_users_by_year_content_type_render($subtype, $conf, $panel_args, $context) {

  // Localising of user group display to.
  $user_lists_object = $context->data;
  $user_lists_tid = $context->data->tid;

  $query = db_select('field_data_field_registered_at', 'f');
  $query->innerJoin('field_data_field_user_id', 'fd', 'f.entity_id = fd.entity_id');
  $query->innerJoin('field_data_field_user_lists', 'fu', 'fu.entity_id = fd.entity_id');
  $query->condition('fu.field_user_lists_tid', $user_lists_tid);
  $query->fields('f', array('entity_id','field_registered_at_value'));
  $query->fields('fd', array('field_user_id_value'));
  $query->groupBy('f.entity_id');
  $result = $query->execute()->fetchAll();
  $years_users  = array();

  $years_stat = array(
    t('Count')
  );
  $active_commiters_by_year = array(
    t('Active commiters')
  );
  $active_commenters_by_year = array(
    t('Active commenters')
  );

  foreach ($result as $item) {
    $year = date('Y', $item->field_registered_at_value);
    if (!isset($years_stat[$year])) {
      $years_stat[$year] = 1;
    } else {
      $years_stat[$year]++;
    }
    $years_users[$year][] = $item->field_user_id_value;
  }
  foreach ($years_users as $year => $users) {
    $active_by_commits_count = db_select('ppgetstat', 'pp')
      ->fields('pp', array('doid'))
      ->condition('pp.type', PPGETSTAT_TYPE_COMMITS)
      ->condition('pp.doid', $users, 'IN')
      ->groupBy('pp.doid')
      ->countQuery()
      ->execute()
      ->fetchField();

    $active_by_comments_count = db_select('ppgetstat', 'pp')
      ->fields('pp', array('doid'))
      ->condition('pp.type', PPGETSTAT_TYPE_COMMENTS)
      ->condition('pp.doid', $users, 'IN')
      ->groupBy('pp.doid')
      ->countQuery()
      ->execute()
      ->fetchField();
    $commit_p = ($active_by_commits_count/count($years_users[$year]))*100;
    $active_commiters_by_year[$year] = $active_by_commits_count . ' <span class="sm">'. $commit_p . ' %</span>';
    $comment_p = ($active_by_comments_count/count($years_users[$year]))*100;
    $active_commenters_by_year[$year] = $active_by_comments_count. ' <span class="sm">'. number_format($comment_p,2) . ' %</span>';
  }

  ksort($years_stat);

  $block = new stdClass();
  $block->module = 'pp_frontpage';
  $block->title = t('Users registered by year statistics');
  $header = array(t('Type of info \ Year'));
  $header += array_keys($years_stat);
  $block->content = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => array(
      array_values($years_stat),
      array_values($active_commiters_by_year),
      array_values($active_commenters_by_year)
    )
  );
  return $block;
}
