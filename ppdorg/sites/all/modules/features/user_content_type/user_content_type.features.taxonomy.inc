<?php
/**
 * @file
 * user_content_type.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function user_content_type_taxonomy_default_vocabularies() {
  return array(
    'office' => array(
      'name' => 'Office',
      'machine_name' => 'office',
      'description' => 'Propeople office',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'position' => array(
      'name' => 'Position',
      'machine_name' => 'position',
      'description' => 'Developer, Project manager, QA, etc',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
  );
}
