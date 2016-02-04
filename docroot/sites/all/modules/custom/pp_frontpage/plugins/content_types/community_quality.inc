<?php

$plugin = array(
'title' => t('Community quality'),
'single' => TRUE,
'category' => t('PP'),
);

/**
 * Render community list statistic
 */
function pp_frontpage_community_quality_content_type_render($subtype, $conf, $panel_args, $context) {
  $block = new stdClass();
  $block->module = 'pp_frontpage';
  $block->title = t('Community statistic');
  //@TODO add user list filtering for future.
  $all_user_count = db_select('node', 'n')
   ->fields('n', array('nid'))
   ->condition('n.type', 'user')
   ->countQuery()
   ->execute()
   ->fetchField();

  $active_by_commits_count = db_select('ppgetstat', 'pp')
   ->fields('pp', array('doid'))
   ->condition('pp.type', PPGETSTAT_TYPE_COMMITS)
   ->groupBy('pp.doid')
   ->countQuery()
   ->execute()
   ->fetchField();

  $active_by_comments_count = db_select('ppgetstat', 'pp')
    ->fields('pp', array('doid'))
    ->condition('pp.type', PPGETSTAT_TYPE_COMMENTS)
    ->groupBy('pp.doid')
    ->countQuery()
    ->execute()
    ->fetchField();

  $block->content = '<div class="bs-callout bs-callout-primary">
   <ul>
   <li>'.t('Total count').' : <strong>' . $all_user_count . '</strong></li>
   <li>'.t('Active committers').' : <strong>' . $active_by_commits_count . ' (' . number_format($active_by_commits_count/$all_user_count, 2)*100 . ')%</strong></li>
   <li>'.t('Active commenters').' : <strong>' . $active_by_comments_count . ' (' . number_format($active_by_comments_count/$all_user_count, 2)*100 . ')%</strong></li>
   </ul>
   </div>';

  return $block;
}
