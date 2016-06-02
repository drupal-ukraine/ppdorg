<?php

$plugin = array(
  'title' => t('Community this month activity'),
  'single' => TRUE,
  'category' => t('PP'),
  'required context' => new ctools_context_required('title','taxonomy_term'),
);

function pp_frontpage_community_month_activity_content_type_render($subtype, $conf, $panel_args, $context) {
  // Localising of user group display to.
  if (isset($context) && isset($context->data) && isset($context->data->tid)) {
    $user_lists_tid = $context->data->tid;

    $block = new stdClass();
    $block->module = 'pp_frontpage';
    $block->title = t('Users activity this month');
    $query = db_select('ppgetstat', 'p')
      ->fields('p', array('doid', 'timestamp'))
      ->condition('p.user_group_tid', $user_lists_tid)
      ->condition('p.timestamp', time(), '<=')
      ->condition('p.timestamp', mktime(0,0,0,date('m'), 0, date('Y')))
      ->execute()
      ->fetchAll();
    dpm($query);
  }
}