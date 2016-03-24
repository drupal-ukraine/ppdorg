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
    'name' => 'Ukraine',
    'description' => '',
    'format' => 'plain_text',
    'weight' => 3,
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
    'field_request_query' => array(),
  );

  $terms[] = array(
    'name' => 'Russian Federation',
    'description' => '',
    'format' => 'plain_text',
    'weight' => 0,
    'uuid' => '5a32b7be-69d3-44fc-af39-e82eb91d3ca2',
    'vocabulary_machine_name' => 'users_list',
    'field_query' => array(
      'und' => array(
        0 => array(
          'first' => 'field_country',
          'second' => 'Russian Federation',
        ),
      ),
    ),
    'field_request_query' => array(),
  );

  $terms[] = array(
    'name' => 'Belarus',
    'description' => '',
    'format' => 'plain_text',
    'weight' => 0,
    'uuid' => '1ec39d48-30dc-47c2-8412-aad31256c4d4',
    'vocabulary_machine_name' => 'users_list',
    'field_query' => array(
      'und' => array(
        0 => array(
          'first' => 'field_country',
          'second' => 'Belarus',
        ),
      ),
    ),
    'field_request_query' => array(),
  );

  $terms[] = array(
    'name' => 'Poland',
    'description' => '',
    'format' => 'plain_text',
    'weight' => 0,
    'uuid' => '49f45070-de6a-4b68-a4ce-474c52498fc7',
    'vocabulary_machine_name' => 'users_list',
    'field_query' => array(
      'und' => array(
        0 => array(
          'first' => 'field_country',
          'second' => 'Poland',
        ),
      ),
    ),
    'field_request_query' => array(),
  );

  $terms[] = array(
    'name' => 'Romania',
    'description' => '',
    'format' => 'plain_text',
    'weight' => 0,
    'uuid' => 'da6af8f8-28de-492b-9ca0-7541affc8e5f',
    'vocabulary_machine_name' => 'users_list',
    'field_query' => array(
      'und' => array(
        0 => array(
          'first' => 'field_country',
          'second' => 'Romania',
        ),
      ),
    ),
    'field_request_query' => array(),
  );

  $terms[] = array(
    'name' => 'Moldova',
    'description' => '',
    'format' => 'plain_text',
    'weight' => 0,
    'uuid' => '64b8c297-b70e-4f6f-a622-f0065f7637f7',
    'vocabulary_machine_name' => 'users_list',
    'field_query' => array(
      'und' => array(
        0 => array(
          'first' => 'field_country',
          'second' => 'Moldova',
        ),
      ),
    ),
    'field_request_query' => array(),
  );

  $terms[] = array(
    'name' => 'Hungary',
    'description' => '',
    'format' => 'plain_text',
    'weight' => 0,
    'uuid' => '33cbdde0-1b82-4e90-af56-634035048718',
    'vocabulary_machine_name' => 'users_list',
    'field_query' => array(
      'und' => array(
        0 => array(
          'first' => 'field_country',
          'second' => 'Hungary',
        ),
      ),
    ),
    'field_request_query' => array(),
  );

  $terms[] = array(
    'name' => 'Slovakia',
    'description' => '',
    'format' => 'plain_text',
    'weight' => 0,
    'uuid' => '8a999be1-f201-47dd-9622-3a11254fb835',
    'vocabulary_machine_name' => 'users_list',
    'field_query' => array(
      'und' => array(
        0 => array(
          'first' => 'field_country',
          'second' => 'Slovakia',
        ),
      ),
    ),
    'field_request_query' => array(),
  );

  $terms[] = array(
    'name' => 'Estonia',
    'description' => '',
    'format' => 'plain_text',
    'weight' => 0,
    'uuid' => 'edaa8c9c-ff03-4302-ac1a-ff14139c34a4',
    'vocabulary_machine_name' => 'users_list',
    'field_query' => array(
      'und' => array(
        0 => array(
          'first' => 'field_country',
          'second' => 'Estonia',
        ),
      ),
    ),
    'field_request_query' => array(),
  );

  return $terms;
}
