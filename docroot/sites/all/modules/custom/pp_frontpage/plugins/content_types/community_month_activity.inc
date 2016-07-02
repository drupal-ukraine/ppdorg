<?php

$plugin = array(
  'title' => t('Community this month activity'),
  'single' => TRUE,
  'category' => t('PP'),
  'required context' => new ctools_context_required('title','taxonomy_term'),
  'hook theme' => 'pp_frontpage_community_month_activity_content_type_theme'
);

function pp_frontpage_community_month_activity_content_type_render($subtype, $conf, $panel_args, $context) {
  // Localising of user group display to.
  if (isset($context) && isset($context->data) && isset($context->data->tid)) {
    $user_lists_tid = $context->data->tid;

    $block = new stdClass();
    $block->module = 'pp_frontpage';
    $block->title = t('Users activity this month');
    $query = db_select('ppgetstat', 'p')
      ->fields('p', array('doid', 'timestamp', 'type'))
      ->condition('p.user_group_tid', $user_lists_tid)
      ->condition('p.timestamp', mktime(0,0,0, 1 , 1, date('Y')), '>=')
      ->execute()
      ->fetchAll();
    $doids = $day_stat = array();
    foreach ($query as $temp) {
      if (isset($doids[$temp->doid])) {
        $doids[$temp->doid]++;
      } else {
        $doids[$temp->doid] = 1;
      }
      $day_stat[date('m', $temp->timestamp)][date('d', $temp->timestamp)][$temp->doid][$temp->type]++;

    }
    $users = _ppgetstat_get_users_by_doids(array_keys($doids));
    $doids = asort($doids);
    drupal_add_js('http://d3js.org/d3.v3.min.js', 'external');
    drupal_add_js(drupal_get_path('module','pp_frontpage') . '/plugins/content_types/js/month_activity.js');
    drupal_add_js(array('pp_frontpage' => array('community_tid' => $user_lists_tid)), 'setting');
    $block = new stdClass();
    $block->module = 'pp_frontpage';
    $block->title = 'Timeline';
    $block->id = 'pp_frontpage_month_activity';
    $block->content = array(
      '#theme' => 'pp_frontpage_month_activity',
    );
    return $block;
  }
}

function pp_frontpage_community_month_activity_content_type_theme(&$theme, $plugin) {
  $theme['pp_frontpage_month_activity'] = array(
    'variables' => array(
      'logo' => '',
    ),
    'path' => $plugin['path'],
    'template' => 'month_activity'
  );
}
