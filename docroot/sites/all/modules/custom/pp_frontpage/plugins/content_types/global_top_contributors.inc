<?php

$plugin = array(
  'title' => t('Global statistics: top contributors.'),
  'single' => TRUE,
  'category' => t('PP'),
);

/**
* Render of global communities size statistics.
 */
function pp_frontpage_global_top_contributors_content_type_render($subtype, $conf, $panel_args) {
  $block = new stdClass();
  $block->module = 'pp_frontpage';
  $block->title = t('Top contributors of all countries.');

  $vocab = taxonomy_vocabulary_machine_name_load('users_list');
  $terms = taxonomy_get_tree($vocab->vid);
  foreach ($terms as $t) {
    $terms_data[$t->tid] = taxonomy_term_load($t->tid);
  }

  // Getting active by commits statistics
  $active_by_commits_query = db_select('ppgetstat', 'pp');
  $active_by_commits_query->condition('pp.type', PPGETSTAT_TYPE_COMMITS);
  $active_by_commits_query->fields('pp',array('doid', 'user_group_tid'));
  $active_by_commits_query->addExpression('COUNT(pp.doid)', 'ac');
  $active_by_commits_query->groupBy('pp.doid');
  $active_by_commits_query->range(0, 100);
  $active_by_commits_query->orderBy('ac', 'DESC');

  $active_by_commits = $active_by_commits_query->execute()->fetchAll();
  foreach ($active_by_commits as $temp) {
    $top_stat[$temp->doid] = $temp;
    $doids[] = $temp->doid;
    $groupd_tids[] = $temp->user_group_tid;
  }

  $rows = array();

  $query = db_select('node', 'n');
  $query->innerJoin('field_data_field_user_id', 'fu', 'n.nid=fu.entity_id');
  $query->fields('n', array('nid'));
  $query->fields('fu', array('field_user_id_value'));
  $query->condition('fu.field_user_id_value', $doids, 'IN');
  $result = $query->execute()->fetchAll();
  foreach ($result as $temp) {
    $nids[] = $temp->nid;
    $nid_map[$temp->nid]= $temp->field_user_id_value;
  }
  $nodes = node_load_multiple($nids);

  foreach ($nodes as $node) {
    $dorg_users[$nid_map[$node->nid]] = $node;
  }
  $i = 1;$country_group = array();
  foreach ($active_by_commits as $temp) {
    $doid = $temp->doid;
    $node = $dorg_users[$doid];
    $flag = '';
    $term = $terms_data[$temp->user_group_tid];

    if (!empty($term->field_logo)) {
      $field_logo = field_get_items('taxonomy_term', $term, 'field_logo');
      $flag = theme('image_style', array(
        'style_name' => '48x',
        'path'=> $field_logo[0]['uri'],
      ));
    }

    $rows[] = array(
      $i,
      $flag,
      l($terms_data[$temp->user_group_tid]->name,'taxonomy/term/' . $temp->user_group_tid),
      $node->title,
      $temp->ac,
      );
    $i++;
    $country_group[$term->name]++;
  }

  $render_array = array(
    '#theme' => 'table',
    '#header' => array(t('Position'),t('Country'), t('Country'), t('Username'), t('Count of commit')),
    '#rows' => $rows
  );

  $block->content = $render_array;

  return $block;
}