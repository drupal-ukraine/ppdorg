<?php

$plugin = array(
  'title' => t('Community quality'),
  'single' => TRUE,
  'category' => t('PP'),
  'required context' => new ctools_context_required('title','taxonomy_term'),
);

/**
 * Render community list statistic
 */
function pp_frontpage_community_quality_content_type_render($subtype, $conf, $panel_args, $context) {

  // Localising of user group display to.
  $user_lists_object = $context->data;
  $user_lists_tid = $context->data->tid;
  dpm($user_lists_object);
  //@TODO make country fetch configurable.
  $field_query = field_get_items('taxonomy_term', $user_lists_object, 'field_query');
  $country = $field_query[0]['second'];

  $block = new stdClass();
  $block->module = 'pp_frontpage';
  $block->title = t('Community statistic');

  // Getting info for showing count of individual members at this country.
  $members_stat = _ppgetstat_get_individual_members();

  //@TODO add user list filtering for future.
  $all_user_query = db_select('node', 'n');
  $all_user_query->innerJoin('field_data_field_user_lists', 'fu', 'fu.entity_id = n.nid');
  $all_user_query->condition('n.type', 'user');
  $all_user_query->condition('fu.field_user_lists_tid', $user_lists_tid);
  $all_user_query->addExpression('COUNT(n.nid)');
  $all_user_result = $all_user_query->execute();
  $all_user_count= $all_user_result->fetchField();

  $active_by_commits_query = db_select('ppgetstat', 'pp');
  $active_by_commits_query->innerJoin('field_data_field_user_id', 'fn', 'fn.field_user_id_value = pp.doid');
  $active_by_commits_query->innerJoin('field_data_field_user_lists', 'fu', 'fu.entity_id = fn.entity_id');
  $active_by_commits_query->condition('pp.type', PPGETSTAT_TYPE_COMMITS);
  $active_by_commits_query->condition('fu.field_user_lists_tid', $user_lists_tid);
  $active_by_commits_query->groupBy('pp.doid');
  $active_by_commits_query->addExpression('COUNT(pp.doid)');
  $active_by_commits_count= $active_by_commits_query->execute()->fetchField();

  $active_by_comments_query = db_select('ppgetstat', 'pp');
  $active_by_comments_query->innerJoin('field_data_field_user_id', 'fn', 'fn.field_user_id_value = pp.doid');
  $active_by_comments_query->innerJoin('field_data_field_user_lists', 'fu', 'fu.entity_id = fn.entity_id');
  $active_by_comments_query->condition('pp.type', PPGETSTAT_TYPE_COMMITS);
  $active_by_comments_query->condition('fu.field_user_lists_tid', $user_lists_tid);
  $active_by_comments_query->groupBy('pp.doid');
  $active_by_comments_query->addExpression('COUNT(pp.doid)');
  $active_by_comments_count= $active_by_comments_query->execute()->fetchField();

  $individual_members_count = $members_stat['members_country_count'][$country];
  if ($all_user_count > 0) {
    $block->content = '<div class="bs-callout bs-callout-primary">
   <ul>
   <li>' . t('Total count') . ' : <strong>' . $all_user_count . '</strong></li>
   <li>' . t('Active committers') . ' : <strong>' . $active_by_commits_count . ' (' . number_format($active_by_commits_count / $all_user_count, 2) * 100 . ')%</strong></li>
   <li>' . t('Active commenters') . ' : <strong>' . $active_by_comments_count . ' (' . number_format($active_by_comments_count / $all_user_count, 2) * 100 . ')%</strong></li>
   <li>' . t('Individual Assoc members') . ' : <strong>' . $individual_members_count . ' (' . number_format($individual_members_count / $all_user_count, 2) * 100 . ')%</strong></li>
   </ul>
   </div>';
  }
  return $block;
}
