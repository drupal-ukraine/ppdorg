<?php

/**
 * @file
 * Add user form plugin.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Add new user'),
  'single' => TRUE,
  'category' => t('PP'),
);

/**
 * Render the Queue status.
 */
function pp_frontpage_adduser_content_type_render($subtype, $conf, $panel_args, $context) {
  $block = new stdClass();
  $block->module = 'pp_frontpage';
  $block->title = t('Add user');

  $form = drupal_get_form('pp_frontpage_adduser');
  $output = '<div class="add-user-form">' . drupal_render($form) . '</div>';

  $block->content = $output;
  return $block;
}

/**
 * Form constructor.
 */
function pp_frontpage_adduser($form, $form_state) {
  $form['nickname'] = array(
    '#type' => 'textfield',
    '#title' => t('Drupal.org username'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
  );

  return $form;
}

/**
 * Validation handler.
 */
function pp_frontpage_adduser_validate($form, &$form_state) {
  $nickname = $form_state['values']['nickname'];

  $exists = db_query('SELECT nid FROM {node} WHERE type = :type AND title = :title',
    array(
      ':type' => PPGETSTAT_USER_NODE_TYPE,
      ':title' => $nickname,
    ))->fetchField();
  if (!empty($exists)) {
    form_set_error('nickname', t('User !link already exists', array(
      '!link' => l($nickname, 'node/' . $exists),
    )));
    return;
  }

  $doid = ppgetstat_get_id_by_nick($nickname);
  if (empty($doid)) {
    form_set_error('nickname', t('There is no such user on drupal.org'));
  }

  $form_state['values']['doid'] = $doid;

  return $form;
}

/**
 * Submit handler.
 */
function pp_frontpage_adduser_submit($form, &$form_state) {
  $nickname = $form_state['values']['nickname'];
  $doid = $form_state['values']['doid'];

  $node = (object) array(
    'title' => $nickname,
    'type' => PPGETSTAT_USER_NODE_TYPE,
    'field_user_id' => array(
      LANGUAGE_NONE => array(
        array('value' => $doid),
      )
    ),
  );
  node_object_prepare($node);
  node_save($node);

  drupal_set_message(t('User !link has been created', array(
    '!link' => l($nickname, 'node/' . $node->nid),
  )));
}
