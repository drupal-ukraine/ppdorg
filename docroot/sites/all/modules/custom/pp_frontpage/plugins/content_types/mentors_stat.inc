<?php

$plugin = array(
  'title' => t('Top mentors'),
  'single' => TRUE,
  'category' => t('PP'),
  'required context' => new ctools_context_required('title','taxonomy_term'),
);

/**
 * Render mentors stat statistics.
 */
function pp_frontpage_mentors_stat_content_type_render($subtype, $conf, $panel_args, $context) {

  // Localising of user group display to.
  if (isset($context) && isset($context->data) && isset($context->data->tid)) {
    $user_lists_tid = $context->data->tid;

    $block = new stdClass();
    $block->module = 'pp_frontpage';
    $block->title = t('Top mentors');

    $query = db_select('field_data_field_mentors', 'fm');
    $query->innerJoin('field_data_field_user_id', 'fd', 'fm.field_mentors_value = fd.field_user_id_value');
    $query->innerJoin('field_data_field_user_lists', 'fu', 'fu.entity_id = fd.entity_id');
    $query->condition('fu.field_user_lists_tid', $user_lists_tid);
    $query->innerJoin('node', 'n', 'fd.entity_id = n.nid');
    $query->fields('fm', array('field_mentors_value'));
    $query->fields('fd', array('field_user_id_value'));
    $query->fields('n', array('nid','title'));
    $query->addExpression('COUNT(nid)', 'kol');
    $query->groupBy('n.nid');
    $query->orderBy('kol', 'DESC');
    $result = $query->execute()->fetchAll();
    $i =1;
    $rows = array();
    foreach ($result as $temp) {
      $rows[] = array(
        $i,
        l($temp->title, 'http://drupal.org/user/' . $temp->field_mentors_value, array('external' => TRUE)),
        $temp->kol,
      );
      $i++;
    }

    $block->content = array(
      '#theme' => 'table',
      '#header' => array(
        t('Position'),
        t('User'),
        t('Count of mentions')
      ),
      '#rows' => $rows,
    );
    return $block;
  }
}
