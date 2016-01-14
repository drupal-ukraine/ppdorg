<?php
/**
 * @file
 * ukraine_users_feature.features.uuid_term.inc
 */

/**
 * Implements hook_uuid_features_default_terms().
 */
function ukraine_users_feature_uuid_features_default_terms() {
  $terms = array();

  $terms[] = array(
    'name' => 'Ukrainian user lists',
    'description' => '',
    'format' => 'plain_text',
    'weight' => 0,
    'uuid' => 'e1bef5a8-bdcc-4fa5-acda-b57d5930f374',
    'vocabulary_machine_name' => 'users_list',
    'field_query' => array(
      'und' => array(
        0 => array(
          'first' => 'field_country',
          'second' => 'Ukraine',
        ),
      ),
    ),
  );
  return $terms;
}
