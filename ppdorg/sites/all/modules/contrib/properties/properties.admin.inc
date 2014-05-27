<?php
// $Id: properties.admin.inc,v 1.1 2010/12/22 19:55:20 berdir Exp $

/**
 * @file
 * Contains admin menu callbacks for properties.module.
 */

/**
 * Menu callback; Display a list of categories.
 */
function properties_admin_categories_list() {
  $header = array(
    array(
      'data' => t('Name'),
      'field' => 'name',
    ),
    array(
      'data' => t('Label'),
      'field' => 'label',
      'sort' => 'asc',
    ),
    t('Operations'),
  );
  $categories = properties_category_load_paging(20, array('header' => $header));
  $rows = array();
  $url_options = array('query' => drupal_get_destination());
  foreach ($categories as $category) {
    $links = array(l(t('edit'), 'admin/config/content/properties/categories/edit/' . $category->name, $url_options), l(t('delete'), 'admin/config/content/properties/categories/delete/' . $category->name, $url_options));

    $rows[] = array(
      check_plain($category->name),
      check_plain($category->label),
      implode(' ', $links),
    );
  }
  $content['table'] = array(
    '#theme' => 'table',
    '#rows' => $rows,
    '#header' => $header,
    '#empty' => t('No categories found.'),
  );
  $content['pager']['#markup'] = theme('pager');
  return $content;
}

/**
 * Menu callback; Display a list of attributes.
 */
function properties_admin_attributes_list($form, $form_state) {
  $header = array(
    array(
      'data' => t('Name'),
      'field' => 'name',
    ),
    array(
      'data' => t('Label'),
      'field' => 'label',
      'sort' => 'asc',
    ),
    t('Operations'),
  );

  $options = array('header' => $header);
  if (!empty($form_state['values']['search'])) {
    $options['search'] = $form_state['values']['search'];
  }

  $attributes = properties_attribute_load_paging(20, $options);
  $rows = array();
  $url_options = array('query' => drupal_get_destination());
  foreach ($attributes as $attribute) {
    $links = array(l(t('edit'), 'admin/config/content/properties/attributes/edit/' . $attribute->name, $url_options), l(t('delete'), 'admin/config/content/properties/attributes/delete/' . $attribute->name, $url_options));

    $rows[] = array(
      check_plain($attribute->name),
      check_plain($attribute->label),
      implode(' ', $links),
    );
  }

  drupal_add_css(drupal_get_path('module', 'properties') . '/properties.css');

  $form['filter'] = array(
    '#prefix' => '<div class="clearfix">',
    '#suffix' => '</div>',
  );

  $form['filter']['search'] = array(
    '#type' => 'textfield',
    '#title' => t('Search'),
    '#title_display' => 'invisible',
    '#size' => 20,
    '#ajax' => array(
      'callback' => 'properties_admin_attributes_list_js',
      'wrapper' => 'attributes-list',
      'effect' => 'fade',
      'event' => 'blur',
    ),
  );

  $form['filter']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );

  $form['filter']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    '#submit' => array('properties_admin_attributes_list_submit_reset'),
  );

  $form['content'] = array(
    '#prefix' => '<div id="attributes-list">',
    '#suffix' => '</div>',
  );

  $form['content']['table'] = array(
    '#theme' => 'table',
    '#rows' => $rows,
    '#header' => $header,
    '#empty' => t('No attributes found.'),
  );
  $form['content']['pager']['#markup'] = theme('pager');
  return $form;
}

/**
 * Submit callback, rebuilds the form.
 */
function properties_admin_attributes_list_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}

/**
 * Rset callback, does not rebuild the form.
 */
function properties_admin_attributes_list_submit_reset($form, &$form_state) {

}

/**
 * Ajax callback function to return the updated table.
 */
function properties_admin_attributes_list_js($form, $form_state) {
  return $form['content'];
}

/**
 * Form builder; Add/Edit form for attributes.
 */
function properties_admin_attributes_form($form, &$form_state, $attribute = NULL) {
  if (empty($attribute)) {
    $attribute = (object) array(
      'label' => '',
      'name' => '',
    );
  }

  $form['label'] = array(
    '#title' => t('Attribute label'),
    '#type' => 'textfield',
    '#default_value' => $attribute->label,
    '#maxlength' => 255,
    '#required' => TRUE,
  );

  $form['name'] = array(
    '#type' => 'machine_name',
    '#default_value' => $attribute->name,
    '#maxlength' => 32,
    '#disabled' => !empty($attribute->name),
    '#machine_name' => array(
      'exists' => 'properties_attribute_load',
    ),
    '#description' => t('A unique machine-readable name for this attribute. It must only contain lowercase letters, numbers, and underscores. This name will be used when using this attribute.'),
  );

  if (empty($attribute->name)) {
    $form['add'] = array(
      '#value' => t('Save'),
      '#type' => 'submit',
      '#submit' => array('properties_admin_attributes_add'),
    );

    $form['add_another'] = array(
      '#value' => t('Save and add another'),
      '#type' => 'submit',
      '#submit' => array('properties_admin_attributes_add_another'),
    );
  }
  else {
    $form['save'] = array(
      '#value' => t('Save'),
      '#type' => 'submit',
      '#submit' => array('properties_admin_attributes_save'),
    );
  }

  return $form;
}

/**
 * Form builder; Submit callback to save a new attribute and redirect to list.
 */
function properties_admin_attributes_add($form, &$form_state) {
  form_state_values_clean($form_state);
  properties_attribute_save((object) $form_state['values']);
  $form_state['redirect'] = 'admin/config/content/properties/attributes';
  drupal_set_message(t('Attribute created.'));
}

/**
 * Form builder; Submit callback to save an attribute and redirect to list.
 */
function properties_admin_attributes_save($form, &$form_state) {
  form_state_values_clean($form_state);
  properties_attribute_save((object) $form_state['values']);
  $form_state['redirect'] = 'admin/config/content/properties/attributes';
  drupal_set_message(t('Attribute updated.'));
}

/**
 * Form builder; Submit callback to save a new attribute and stay on form.
 */
function properties_admin_attributes_add_another($form, &$form_state) {
  form_state_values_clean($form_state);
  properties_attribute_save((object) $form_state['values']);
  drupal_set_message(t('Attribute created.'));
}

/**
 * Form builder; Add/Edit form for categories.
 */
function properties_admin_categories_form($form, &$form_state, $category = NULL) {
  if (!isset($form_state['category'])) {
    if (empty($category)) {
      $category = (object) array(
        'label' => '',
        'name' => '',
        'attributes' => array(),
        'new' => TRUE,
      );
    }
    $form_state['category'] = $category;
  }

  // Make sure there are always 3 empty attribute displayed.
  $add_empty = TRUE;
  foreach ($form_state['category']->attributes as $attribute) {
    if (empty($attribute->name)) {
      $add_empty = FALSE;
      break;
    }
  }
  if ($add_empty) {
    // Use a random id for these to trick out browsers.
    $form_state['category']->attributes[] = (object)array(
      'name' => '',
      'label' => '',
      'weight' => count($form_state['category']->attributes),
    );
  }

  drupal_add_css(drupal_get_path('module', 'properties') . '/properties_admin.css');

  $form['label'] = array(
    '#title' => t('Category label'),
    '#type' => 'textfield',
    '#default_value' => $form_state['category']->label,
    '#maxlength' => 255,
    '#required' => TRUE,
  );

  $form['name'] = array(
    '#type' => 'machine_name',
    '#default_value' => $form_state['category']->name,
    '#maxlength' => 32,
    '#disabled' => empty($form_state['category']->new),
    '#machine_name' => array(
      'exists' => 'properties_category_load',
    ),
    '#description' => t('A unique machine-readable name for this category. It must only contain lowercase letters, numbers, and underscores. This name will be used when using this attribute.'),
  );

  $attributes = array();
  $delta = 0;
  foreach ($form_state['category']->attributes as $name => $attribute) {
    if (empty($attribute->name) && (isset($form_state['input']['attributes'][$delta]))) {
      unset($form_state['input']['attributes'][$delta]);
    }
    $element = array();
    $element['attribute'] = array(
      '#type' => 'textfield',
      '#default_value' => $attribute->name,
      '#autocomplete_path' => 'properties_autocomplete/attribute',
      '#ajax' => array(
        'callback' => 'properties_admin_update_label_js',
        'wrapper' => 'attributes-wrapper',
        'effect' => 'fade',
      ),
    );
    // If name is given but label is empty, this means that this is a new
    // attribute. Display a textfield to enter a label.
    if (!empty($attribute->new)) {
      $element['label'] = array(
        '#type' => 'textfield',
        '#default_value' => $attribute->label,
        '#maxlength' => 255,
        '#required' => TRUE,
      );
    }
    else {
      $element['label'] = array(
        '#markup' => check_plain($attribute->label),
      );
    }

    // We name the element '_weight' to avoid clashing with elements
    // defined by widget.
    $element['_weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight for row @number', array('@number' => $delta)),
      '#title_display' => 'invisible',
      '#default_value' => $delta,
      '#weight' => 100,
    );
    $attributes[!empty($attribute->name) ? $attribute->name : $delta] = $element;
    $delta++;
  }

  $attributes += array(
    '#type' => 'fieldset',
    '#theme' => 'properties_admin_categories_attributes_form',
    '#title' => t('Attributes'),
    '#description' => t('Configure the default attributes of this category.'),
    '#prefix' => '<div id="attributes-wrapper">',
    '#suffix' => '</div>',
    '#max_delta' => $delta,
    '#tree' => TRUE,
  );
  $attributes['add_more'] = array(
    '#type' => 'submit',
    '#name' => 'attributes_add_more',
    '#value' => t('Add another attribute'),
    '#attributes' => array('class' => array('field-add-more-submit')),
    '#limit_validation_errors' => array(array('attributes')),
    '#submit' => array('properties_admin_add_more_submit'),
    '#ajax' => array(
      'callback' => 'properties_admin_add_more_js',
      'wrapper' => 'attributes-wrapper',
      'effect' => 'fade',
    ),
  );

  $form['attributes'] = $attributes;

  if (empty($category->name)) {
    $form['add'] = array(
      '#value' => t('Save'),
      '#type' => 'submit',
      '#submit' => array('properties_admin_categories_add'),
    );

    $form['add_another'] = array(
      '#value' => t('Save and add another'),
      '#type' => 'submit',
      '#submit' => array('properties_admin_categories_add_another'),
    );
  }
  else {
    $form['save'] = array(
      '#value' => t('Save'),
      '#type' => 'submit',
      '#submit' => array('properties_admin_categories_save'),
    );
  }
  return $form;
}

function properties_admin_categories_form_validate($form, &$form_state) {
  // Update category object.
  $form_state['category']->label = $form_state['values']['label'];
  $form_state['category']->name = $form_state['values']['name'];
  $form_state['category']->attributes = array();

  uasort($form_state['values']['attributes'], '_field_sort_items_helper');
  foreach ($form_state['values']['attributes'] as $name => $attribute) {
    if (is_array($attribute) && isset($attribute['attribute'])) {
      if (empty($attribute['attribute'])) {
        $attribute_object = (object)array(
          'label' => '',
          'name' => '',
          'weight' => count($form_state['category']->attributes),
        );
        $form_state['category']->attributes[$name] = $attribute_object;
      }
      elseif ($attribute_object = properties_attribute_load($attribute['attribute'])) {
        $attribute_object->weight = count($form_state['category']->attributes);
        $form_state['category']->attributes[$attribute_object->name] = $attribute_object;
      }
      else {
        // Update empty attribute.
        $attribute_object = (object)array(
          'label' => isset($attribute['label']) ? $attribute['label'] : '',
          'name' => $attribute['attribute'],
          'weight' => count($form_state['category']->attributes),
          'new' => TRUE,
        );
        $form_state['category']->attributes[$attribute['attribute']] = $attribute_object;

        // If label is empty, add validation error.
        if (empty($form_state['category']->attributes[$attribute['attribute']]->label)) {
          // Not possible to do this as a proper form validation step because
          // the form is not rebuilt in case of validation errors.
          drupal_set_message(t('Attribute %name does not exist, a label must be provided to create it.', array('%name' => $attribute['attribute'])), 'error');
          $form_state['rebuild'] = TRUE;
        }
      }
    }
  }
}

/**
 * Form builder; Submit callback to save a category and redirect to list.
 */
function properties_admin_categories_add($form, &$form_state) {
  properties_category_save($form_state['category']);
  $form_state['redirect'] = 'admin/config/content/properties/categories';
  drupal_set_message(t('Category created.'));
}

/**
 * Form builder; Submit callback to save a category and redirect to list.
 */
function properties_admin_categories_save($form, &$form_state) {
  properties_category_save($form_state['category']);
  $form_state['redirect'] = 'admin/config/content/properties/categories';
  drupal_set_message(t('Category updated.'));
}

/**
 * Form builder; Submit callback to save a category and stay on form.
 */
function properties_admin_categories_add_another($form, &$form_state) {
  properties_category_save($form_state['category']);
  drupal_set_message(t('Category created.'));
}


/**
 * Returns HTML for the attributes table in the category table.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: A render element representing the form element.
 *
 * @ingroup themeable
 */
function theme_properties_admin_categories_attributes_form($variables) {
  $element = $variables['element'];
  $output = '';

  $table_id = 'attributes-values';
  $order_class = 'attributes-delta-order';
  $required = !empty($element['#required']) ? '<span class="form-required" title="' . t('This field is required.') . '">*</span>' : '';

  $header = array(
    array(
      'data' => '<label>' . t('!title: !required', array('!title' => $element['#title'], '!required' => $required)) . "</label>",
      'colspan' => 2,
      'class' => array('field-label'),
    ),
    t('Label'),
    t('Order'),
  );
  $rows = array();

  // Sort items according to '_weight' (needed when the form comes back after
  // preview or failed validation)
  $items = array();
  foreach (element_children($element) as $key) {
    if ($key === 'add_more') {
      $add_more_button = &$element[$key];
    }
    else {
      $items[] = &$element[$key];
    }
  }
  uasort($items, '_field_sort_items_value_helper');

  // Add the items as table rows.
  foreach ($items as $key => $item) {
    $item['_weight']['#attributes']['class'] = array($order_class);
    $delta_element = drupal_render($item['_weight']);

    $label = drupal_render($item['label']);
    $cells = array(
      array('data' => '', 'class' => array('field-multiple-drag')),
      array('data' => drupal_render($item), 'class' => array('properties-attribute-row-small')),
      $label,
      array('data' => $delta_element, 'class' => array('delta-order')),
    );
    $rows[] = array(
      'data' => $cells,
      'class' => array('draggable'),
    );
  }

  $output = '<div class="form-item">';
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => $table_id, 'class' => array('field-multiple-table'))));
  $output .= $element['#description'] ? '<div class="description">' . $element['#description'] . '</div>' : '';
  $output .= '<div class="clearfix">' . drupal_render($add_more_button) . '</div>';
  $output .= '</div>';

  drupal_add_tabledrag($table_id, 'order', 'sibling', $order_class);

  return $output;
}

function properties_admin_update_label_js($form, &$form_state) {
  return $form['attributes'];
}


/**
 * Submit handler for the "Add another item" button of a field form.
 *
 * This handler is run regardless of whether JS is enabled or not. It makes
 * changes to the form state. If the button was clicked with JS disabled, then
 * the page is reloaded with the complete rebuilt form. If the button was
 * clicked with JS enabled, then ajax_form_callback() calls field_add_more_js()
 * to return just the changed part of the form.
 */
function properties_admin_add_more_submit($form, &$form_state) {
  $form_state['category']->attributes[] = (object)array(
    'label' => '',
    'name' => '',
    'weight' => count($form_state['category']->attributes),
  );

  $form_state['rebuild'] = TRUE;
}

/**
 * Ajax callback in response to a new empty widget being added to the form.
 *
 * This returns the new page content to replace the page content made obsolete
 * by the form submission.
 *
 * @see field_add_more_submit()
 */
function properties_admin_add_more_js($form, $form_state) {
  $element = $form['attributes'];

  return $element;
}

/**
 * Form builder function; Provide a confirmation form to delete an attribute.
 *
 * @see properties_admin_attributes_delete_submit()
 */
function properties_admin_attributes_delete($form, &$form_state, $attribute) {
  $form['attribute'] = array(
    '#type' => 'value',
    '#value' => $attribute,
  );

  return confirm_form($form, t('Delete attribute %attribute', array('%attribute' => $attribute->label)), '');
}

/**
 * Submit handler to delete an attribute.
 */
function properties_admin_attributes_delete_submit($form, &$form_state) {
  properties_attribute_delete($form_state['values']['attribute']);
  drupal_set_message(t('Attribute deleted.'));
}

/**
 * Form builder function; Provide a confirmation form to delete a category.
 *
 * @see properties_admin_categories_delete_submit()
 */
function properties_admin_categories_delete($form, &$form_state, $category) {
  $form['category'] = array(
    '#type' => 'value',
    '#value' => $category,
  );

  return confirm_form($form, t('Delete category %category', array('%category' => $category->label)), '');
}

/**
 * Submit handler to delete a category.
 */
function properties_admin_categories_delete_submit($form, &$form_state) {
  properties_category_delete($form_state['values']['category']);
  drupal_set_message(t('Category deleted.'));
}