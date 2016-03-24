<?php
/**
 * @file
 * ukraine_users_feature.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function ukraine_users_feature_taxonomy_default_vocabularies() {
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
