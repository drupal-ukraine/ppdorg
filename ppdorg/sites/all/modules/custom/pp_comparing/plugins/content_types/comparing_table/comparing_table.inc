<?php

/**
 * @file
 * Visualization of user stats via table.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('User stats comparing via table'),
  'single' => TRUE,
  'category' => t('PP'),
);

/**
 * Render the Comparing settings form and comparing table.
 */
function pp_comparing_comparing_table_content_type_render($subtype, $conf, $panel_args, $context) {
  $block = new stdClass();
  $block->module = 'pp_comparing';
  $block->title = t('Comparing of users');

  $form = drupal_get_form('pp_comparing_comparing_table_settings_form');

  if (empty($_GET['users'])) {
    $block->content = $form;
    return $block;
  }

  $users = explode(',', $_GET['users']);

  if (!empty($_GET['types'])) {
    $types = $_GET['types'];
  }
  else {
    $types = NULL;
  }

  $data = _pp_comparing_users_data($users, $types);

  $header = array(t('Date'));
  foreach (reset($data) as $key => $field) {
    if (is_numeric($key)) {
      $header[] = _ppgetstat_get_node_link_by_doid($key);
    }
  }

  $block->content = array(
    'form' => $form,
    'table' => array(
      '#markup' => theme('table', array('rows' => $data, 'header' => $header))
    )
  );

  return $block;
}

/**
 * Comparing settings form.
 */
function pp_comparing_comparing_table_settings_form($form, &$form_state) {
  $users = node_load_multiple(array(), array('type' => 'user'));

  $users_options = array();

  foreach ($users as $user) {
    $users_options[$user->nid] = $user->title;
  }

  $form['users'] = array(
    '#title' => t('Users you want to compare with each other'),
    '#type' => 'select',
    '#multiple' => TRUE,
    '#options' => $users_options,
    '#default_value' => isset($_GET['users']) ? explode(',', $_GET['users']) : '',
  );

  $form['types'] = array(
    '#title' => t('By what stats do you want to compare'),
    '#type' => 'select',
    '#multiple' => TRUE,
    '#options' => array(
      '1' => t('Contrib modules commits'),
      '2' => t('Drupal core commits'),
      '3' => t('Drupal.org comments'),
    ),
    '#default_value' => isset($_GET['users']) ? explode(',', $_GET['types']) : '',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Compare!'),
  );

  return $form;
}

/**
 * Comparing settings form submit callback.
 */
function pp_comparing_comparing_table_settings_form_submit($form, &$form_state) {
  $users = implode(',', $form_state['values']['users']);
  $types = implode(',', $form_state['values']['types']);

  drupal_goto('comparing', array('query' => array('users' => $users, 'types' => $types)));
}
