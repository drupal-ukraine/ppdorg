<?php
/**
 * @file
 * ppdorg_all_users_feature.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function ppdorg_all_users_feature_taxonomy_default_vocabularies() {
  return array(
    'users_list' => array(
      'name' => 'users_list',
      'machine_name' => 'users_list',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
  );
}
