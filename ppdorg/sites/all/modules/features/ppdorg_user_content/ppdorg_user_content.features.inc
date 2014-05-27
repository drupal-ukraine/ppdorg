<?php
/**
 * @file
 * ppdorg_user_content.features.inc
 */

/**
 * Implements hook_node_info().
 */
function ppdorg_user_content_node_info() {
  $items = array(
    'user' => array(
      'name' => t('user'),
      'base' => 'node_content',
      'description' => t('User node.
Every node will store statistics for user from drupal.org
User may have more than one profile, so we need store all data from all user profiles.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
