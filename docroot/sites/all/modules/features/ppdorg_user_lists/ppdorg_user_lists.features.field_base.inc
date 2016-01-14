<?php
/**
 * @file
 * ppdorg_user_lists.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function ppdorg_user_lists_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_request_query'.
  $field_bases['field_request_query'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_request_query',
    'indexes' => array(),
    'locked' => 0,
    'module' => 'double_field',
    'settings' => array(
      'first' => array(
        'maxlength' => 255,
        'precision' => 10,
        'scale' => 2,
        'size' => 'normal',
        'type' => 'varchar',
      ),
      'second' => array(
        'maxlength' => 255,
        'precision' => 10,
        'scale' => 2,
        'size' => 'normal',
        'type' => 'varchar',
      ),
    ),
    'translatable' => 0,
    'type' => 'double_field',
  );

  return $field_bases;
}
