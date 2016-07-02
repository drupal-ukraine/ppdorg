<?php

$plugin = array(
  'title' => t('Community quality'),
  'single' => TRUE,
  'category' => t('PP'),
  'required context' => new ctools_context_required('title', 'taxonomy_term'),
  'hook theme' => 'pp_front_page_community_quality_content_type_theme'
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
    $block->content = '';
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
      $block->title = t('Community statistic');
      $block->content = array(
        '#theme' => 'pp_frontpage_community_quality',
        '#total' => $all_user_count,
        '#active_committers' => $active_by_commits_count,
        '#active_commenters' => $active_by_comments_count,
        '#assoc_individual_members' => $individual_members_count,
        '#active_committers_p' => number_format(
          $active_by_commits_count / $all_user_count,
          2
        ) * 100,
        '#active_commenters_p' => number_format(
            $active_by_comments_count / $all_user_count,
            2
          ) * 100,
        '#assoc_individual_members_p' => number_format(
            $individual_members_count / $all_user_count,
            2
          ) * 100
      );
    }
    return $block;
  }
}

/**
 * hook_theme callback for this plugin.
 */
function pp_front_page_community_quality_content_type_theme(&$theme, $plugin) {
  $theme['pp_frontpage_community_quality'] = array(
    'variables' => array(
      'total' => 0,
      'active_committers' => 0,
      'active_commenters' => 0,
      'assoc_individual_members' => 0,
      'active_committers_p' => 0,
      'active_commenters_p' => 0,
      'assoc_individual_members_p' => 0
    ),
    'path' => $plugin['path'] . '/templates/',
    'template' => 'community_quality'
  );
}
