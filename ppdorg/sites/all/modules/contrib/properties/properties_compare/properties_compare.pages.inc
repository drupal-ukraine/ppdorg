<?php
// $Id: properties_compare.pages.inc,v 1.1 2010/12/22 19:55:20 berdir Exp $

/**
 * @file
 * User page callbacks for properties_compare module.
 */

/**
 * Page callback, display comparison table.
 */
function properties_compare_page() {
  drupal_set_title(t('Comparison'));

  // Load current compare items.
  $list = properties_compare_list();
  if (empty($list)) {
    return t('Nothing to compare.');
  }

  $categories = array();
  $attributes = array();
  $entities = array();
  $first = array();
  // First, loop over compare items to collect categories and attributes.
  foreach ($list as $key => $item) {
    $entity = reset(entity_load($item['entity_type'], array($item['entity_id'])));
    if (!$first) {
      $first = $item + array('entity' => $entity);
    }
    else {
      // Verify if property can be compared. Ignore if not.
      if (!properties_compare_is_comparable($first['entity_type'], $first['entity'], $item['entity_type'], $entity)) {
        unset($list[$key]);
        drupal_set_message(t('%label can not be compared.', array('%label' => entity_label($item['entity_type'], $entity))), 'warning');
        continue;
      }
    }
    $entities[properties_compare_create_key($item['entity_type'], $entity)] = $entity;
    foreach (properties_extract_fields($item['entity_type'], $entity) as $field_content) {
      foreach ($field_content as $property) {
        $id = properties_compare_create_key($item['entity_type'], $entity);

        // Collect number of entities that have a given category.
        $categories[$property['category']][$id] = TRUE;

        // Keep values in a hierarchic way for the next step.
        $attributes[$property['category']][$property['attribute']][$id] = $property['value'];
      }
    }
  }
  // Remove categories which are only used once.
  $shared_categories = array_filter($categories, 'properties_compare_filter_categories');
  if (empty($shared_categories)) {
    return t('Nothing to compare.');
  }
  // Load categories.
  $loaded_categories = properties_category_load_multiple(array_keys($shared_categories));

  // Collect header.
  $header = array(t('Attributes'));
  foreach ($list as $item) {
      $entity = $entities[$item['entity_type'] . '_' . $item['entity_id']];
      // View mode functionality only implemented for entity_type node.
      if ($item['entity_type'] == 'node' && variable_get('properties_compare_view_mode', TRUE)) {
        $content = node_view($entity, 'properties_compare_page');
        $content['links']['#access'] = FALSE;
        $header[] = drupal_render($content);
        $uri['options']['html'] = TRUE;
      }
      else {
        $header[] = entity_label($item['entity_type'], $entity);
      }
  }

  // Collect rows.
  $rows = array($header);
  foreach ($shared_categories as $shared_category => $value) {
    // Add a header row for the category.
    $rows[] = array(
      array(
        'data' => check_plain($loaded_categories[$shared_category]->label),
        'header' => TRUE,
        'colspan' => count($list) + 1,
      ),
    );

    // Load attributes for this category.
    $loaded_attributes = properties_attribute_load_multiple(array_keys($attributes[$shared_category]));

    // Loop over attributes of this category.
    foreach ($attributes[$shared_category] as $attribute_name => $field_values) {
      // First cell is the attribute label.
      $row = array(array('data' => check_plain($loaded_attributes[$attribute_name]->label)));
      // Loop over entites in list.
      foreach ($list as $item) {
        $entity = $entities[$item['entity_type'] . '_' . $item['entity_id']];
        $id = properties_compare_create_key($item['entity_type'], $entity);
        // Check if this entity has a value for this attribute. If yes, display
        // the value, if not, display as an empty cell.
        if (isset($field_values[$id])) {
          $row[] = check_plain($field_values[$id]);
        }
        else {
          $row[] = '&nbsp;';
        }
      }
      $rows[] = $row;
    }
  }
  return theme('table', array('rows' => $rows));
}

/**
 * Page calllback to remove an entity from the list.
 */
function properties_compare_delete($entity_type, $entity_id) {
  properties_compare_list_delete($entity_type, $entity_id);
  drupal_goto();
}