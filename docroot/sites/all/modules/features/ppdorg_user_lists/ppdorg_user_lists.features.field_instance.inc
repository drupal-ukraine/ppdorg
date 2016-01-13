<?php
/**
 * @file
 * ppdorg_user_lists.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function ppdorg_user_lists_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'taxonomy_term-users_list-field_request_query'.
  $field_instances['taxonomy_term-users_list-field_request_query'] = array(
    'bundle' => 'users_list',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'double_field',
        'settings' => array(
          'first' => array(
            'format' => '_none',
            'hidden' => 0,
            'prefix' => '',
            'suffix' => ':&nbsp;',
          ),
          'second' => array(
            'format' => '_none',
            'hidden' => 0,
            'prefix' => '',
            'suffix' => '',
          ),
          'style' => 'inline',
        ),
        'type' => 'double_field_unformatted_list',
        'weight' => 1,
      ),
    ),
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_request_query',
    'label' => 'request_query',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'double_field',
      'settings' => array(
        'first' => array(
          'general' => array(
            'prefix' => '',
            'required' => 1,
            'suffix' => '',
          ),
          'textfield' => array(
            'placeholder' => '',
            'size' => 10,
          ),
        ),
        'inline' => 1,
        'second' => array(
          'general' => array(
            'prefix' => '',
            'required' => 1,
            'suffix' => '',
          ),
          'textfield' => array(
            'placeholder' => '',
            'size' => 10,
          ),
        ),
      ),
      'type' => 'textfield_&_textfield',
      'weight' => 2,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('request_query');

  return $field_instances;
}
