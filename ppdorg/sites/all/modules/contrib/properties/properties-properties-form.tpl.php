<?php

$element = $variables['element'];
$output = '';

$table_id = 'properties-table';

if (user_access('add properties attributes')) {
  $header = array(
    t('Attribute name'),
    t('Attribute label'),
    t('Value'),
    t('Category'),
    t('Order'),
    t('Delete'),
  );
}
else {
  $header = array(
    t('Attribute'),
    t('Value'),
  );
  if (user_access('add properties categories')) {
    $header[] = t('Order');
  }
}
$rows = array();
if (user_access('add properties attributes') || user_access('add properties categories')) {
  drupal_add_tabledrag($table_id, 'match', 'parent', 'property-category-select', 'property-category-select', 'category-name');
  drupal_add_tabledrag($table_id, 'order', 'sibling', 'property-weight');
}

foreach (element_children($element['listing']) as $category_name) {
  $category = $element['listing'][$category_name];
  $category['_weight']['#attributes']['class'] = array('property-weight');
  $category['name']['#attributes']['class'] = array('category-name');
  $category['category']['#attributes']['class'] = array('property-category-select');

  $cells = array();
  $cells[] = '<strong>' . $category['#label'] . '</strong>';
  $cells[] = '&nbsp;';
  if (user_access('add properties attributes')) {
    $cells[] = '&nbsp;';
    $cells[] = '&nbsp;';
  }
  if (user_access('add properties categories')) {
    $cells[] = drupal_render($category['category']) . drupal_render($category['name']) . drupal_render($category['_weight']);
    $cells[] = drupal_render($category['delete']);
  }
  $rows[] = array(
    'data' => $cells,
    'class' => array('draggable', 'tabledrag-root'),
  );

  foreach (element_children($category['properties']) as $delta) {
    $property = $category['properties'][$delta];

    // Set special classes needed for table drag and drop.
    $property['category']['#attributes']['class'] = array('property-category-select');
    $property['_weight']['#attributes']['class'] = array('property-weight');

    $cells = array();
    if (user_access('add properties attributes')) {
      $cells[] = array('data' => theme('indentation', array('size' => 1)) . drupal_render($property['attribute']), 'class' => array('properties-attribute-row-small'));
      $cells[] = drupal_render($property['label']);
    }
    else {
      $cells[] = theme('indentation', array('size' => 1)) . drupal_render($property['label']);
    }

    $cells[] = drupal_render($property['value']);
    if (user_access('add properties attributes')) {
      $cells[] = drupal_render($property['category']);
      $cells[] = drupal_render($property['_weight']);
      $cells[] = drupal_render($property['delete']);
    }
    elseif (user_access('add properties categories')) {
      $cells[] = '&nbsp;';
    }
    $rows[] = array(
      'data' => $cells,
      'class' => user_access('add properties attributes') ? array('draggable', 'tabledrag-leaf') : array('properties-tabledrag-children'),
    );
  }
}

$output = '<div class="form-item field-widget-properties-table">';
$output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => $table_id, 'class' => array('properties-table'))));
$output .= '<div class="clearfix inline">' . drupal_render($element['actions']) . '</div>';
$output .= '</div>';

echo $output;
