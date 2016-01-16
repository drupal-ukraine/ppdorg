<?php
/**
 * @file
 * ppdorg_user_content.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function ppdorg_user_content_taxonomy_default_vocabularies() {
  return array(
    'company' => array(
      'name' => 'company',
      'machine_name' => 'company',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'job_title' => array(
      'name' => 'job_title',
      'machine_name' => 'job_title',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
  );
}
