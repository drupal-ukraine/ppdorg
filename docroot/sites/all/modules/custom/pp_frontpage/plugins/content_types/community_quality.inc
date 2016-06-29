<?php

$plugin = array(
  'title' => t('Community quality'),
  'single' => TRUE,
  'category' => t('PP'),
  'required context' => new ctools_context_required('title', 'taxonomy_term'),
);

/**
 * Render community list statistic
 */
function pp_frontpage_community_quality_content_type_render(
  $subtype,
  $conf,
  $panel_args,
  $context
) {

  if (isset($context) && isset($context->data) && isset($context->data->tid)) {
    // Localising of user group display to.
    $user_lists_object = $context->data;
    $user_lists_tid = $context->data->tid;
    //@TODO make country fetch configurable.
    $field_query = field_get_items(
      'taxonomy_term',
      $user_lists_object,
      'field_query'
    );
    $country = $field_query[0]['second'];

    $block = new stdClass();
    $block->module = 'pp_frontpage';
    $block->title = t('Community statistic');

    // Getting info for showing count of individual members at this country.
    $members_stat = _ppgetstat_get_individual_members();

// Query for global count uids, attached to the country.
    $query = db_select('node', 'n');
    $query->innerJoin(
      'field_data_field_user_lists',
      'fu',
      'fu.entity_id = n.nid'
    );
    $query->fields('fu', array('field_user_lists_tid'));
    $query->addExpression('COUNT(fu.field_user_lists_tid)', 'gc');
    $query->condition('fu.field_user_lists_tid', $user_lists_tid);
    $query->orderBy('gc', 'DESC');
    $all_user_count = $query->countQuery()->execute()->fetchField();

    // Getting active by commits statistics
    $active_by_commits_query = db_select('ppgetstat', 'pp');
    $active_by_commits_query->condition('pp.type', PPGETSTAT_TYPE_COMMITS);
    $active_by_commits_query->condition('pp.user_group_tid', $user_lists_tid);
    $active_by_commits_query->fields('pp',array('user_group_tid'));
    $active_by_commits_query->addExpression('COUNT(pp.doid)', 'ac');

    $active_by_commits_query->groupBy('pp.user_group_tid');
    $active_by_commits_query->groupBy('pp.doid');
    $active_by_commits_count = $active_by_commits_query->countQuery()->execute()->fetchField();

    $active_by_comments_count = 0;

    $individual_members_count = $members_stat['members_country_count'][$country];
    if ($all_user_count > 0) {
      $block->content = '<div class="bs-callout bs-callout-primary">
   <ul>
   <li>' . t('Total count') . ' : <strong>' . $all_user_count . '</strong></li>
   <li>' . t(
          'Active committers'
        ) . ' : <strong>' . $active_by_commits_count . ' (' . number_format(
          $active_by_commits_count / $all_user_count,
          2
        ) * 100 . ')%</strong></li>
   <li>' . t(
          'Active commenters'
        ) . ' : <strong>' . $active_by_comments_count . ' (' . number_format(
          $active_by_comments_count / $all_user_count,
          2
        ) * 100 . ')%</strong></li>
   <li>' . t(
          'Individual Assoc members'
        ) . ' : <strong>' . $individual_members_count . ' (' . number_format(
          $individual_members_count / $all_user_count,
          2
        ) * 100 . ')%</strong></li>
   </ul>
   </div>';
    }
    return $block;
  }
}
