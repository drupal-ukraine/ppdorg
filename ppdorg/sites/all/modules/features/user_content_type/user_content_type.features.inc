<?php
/**
 * @file
 * user_content_type.features.inc
 */

/**
 * Implements hook_node_info().
 */
function user_content_type_node_info() {
  $items = array(
    'user' => array(
      'name' => t('User'),
      'base' => 'node_content',
      'description' => t('User to pull information from Drupal.org about.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
