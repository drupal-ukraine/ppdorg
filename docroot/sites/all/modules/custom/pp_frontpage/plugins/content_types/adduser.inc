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
  $block->title = t('Add users');

  $form = menu_execute_active_handler('admin/config/services/sync_user_list', FALSE);
  $output = '<div class="add-user-form">' . drupal_render($form) . '</div>';

  $block->content = $output;
  return $block;
}
