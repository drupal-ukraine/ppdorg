<?php

$plugin = array(
  'title' => t('Global statistics: community size.'),
  'single' => TRUE,
  'category' => t('PP'),
);

/**
 * Render of global communities size statistics.
 */
function pp_frontpage_global_countries_members_content_type_render($subtype, $conf, $panel_args) {

  $block = new stdClass();
  $block->module = 'pp_frontpage';
  $block->title = t('Countries under our investigation.');
  // Try to get info from cache first.
  $cache = cache_get(__FUNCTION__);
  if (!empty($cache)) {
    $block->content = $cache->data;
    return $block;
  }

  //Getting data of count of individual members.
  $members_stat = _ppgetstat_get_individual_members();

  $vocab = taxonomy_vocabulary_machine_name_load('users_list');
  $terms = taxonomy_get_tree($vocab->vid);
  foreach ($terms as $t) {
    $terms_data[$t->tid] = taxonomy_term_load($t->tid);
    $terms_data[$t->tid]->individuals_count = $members_stat['members_country_count'][$terms_data[$t->tid]->name];
  }
  // Query for global count uids, attached to the country.
  $query = db_select('node', 'n');
  $query->innerJoin(
    'field_data_field_user_lists',
    'fu',
    'fu.entity_id = n.nid'
  );
  $query->fields('fu', array('field_user_lists_tid'));
  $query->addExpression('COUNT(fu.field_user_lists_tid)', 'gc');
  $query->groupBy('fu.field_user_lists_tid');
  $query->orderBy('gc', 'DESC');
  $global_user_count = $query->execute()->fetchAll();
  foreach ($global_user_count as $temp) {
    $global_total[$temp->field_user_lists_tid] = $temp->gc;
  }


  // Getting active by commits statistics
  $active_by_commits_query = db_select('field_data_field_user_lists', 'fu');
  $active_by_commits_query->innerJoin(
    'field_data_field_user_id',
    'fn',
    'fu.entity_id = fn.entity_id'
  );
  $active_by_commits_query->innerJoin(
    'ppgetstat',
    'pp',
    'pp.doid = fn.field_user_id_value'
  );
  $active_by_commits_query->condition('pp.type', PPGETSTAT_TYPE_COMMITS);
  $active_by_commits_query->fields('fu',array('field_user_lists_tid'));
  $active_by_commits_query->addExpression('COUNT(pp.doid)', 'ac');

  $active_by_commits_query->groupBy('fu.field_user_lists_tid');
  $active_by_commits_query->groupBy('pp.doid');
  $active_by_commits = $active_by_commits_query->execute()->fetchAll();
  $active_commits = array();
  foreach ($active_by_commits as $temp) {
    if (!empty($active_commits[$temp->field_user_lists_tid])) {
      $active_commits[$temp->field_user_lists_tid]++;
    } else {
      $active_commits[$temp->field_user_lists_tid] = 1;
    }
  }

  $rows = array();

  foreach ($global_total as $tid => $count) {
    $rows[] = array(
      $terms_data[$tid]->name,
      $count,
      $active_commits[$tid],
      $terms_data[$tid]->individuals_count
    );
  }

  $render_array = array(
    '#theme' => 'table',
    '#header' => array(t('Country'), t('Total users'), t('Active commiters'), t('Individual members')),
    '#rows' => $rows
  );

  cache_set(
    __FUNCTION__,
    $render_array,
    'cache',
    CACHE_TEMPORARY
  );

  $block->content = $render_array;

  return $block;
}

