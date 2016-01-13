<?php
/**
 * @file
 * ppdorg_user_lists.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function ppdorg_user_lists_taxonomy_default_vocabularies() {
  return array(
    'users_list' => array(
      'name' => 'users list',
      'machine_name' => 'users_list',
      'description' => 'User list scan to',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
  );
}
