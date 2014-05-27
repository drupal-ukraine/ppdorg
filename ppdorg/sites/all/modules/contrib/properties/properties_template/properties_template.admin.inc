<?php

// $Id: properties_template.admin.inc,v 1.1 2010/12/22 19:55:20 berdir Exp $

/**
 * @file
 * Contains admin menu callbacks for properties_template.module.
 */

/**
 * Menu callback; Display a list of templates
 */
function properties_template_admin_templates_list() {
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
  $templates = properties_template_load_paging(20, array('header' => $header));
  $rows = array();
  $url_options = array('query' => drupal_get_destination());
  foreach ($templates as $template) {
    $links = array(l(t('edit'), 'admin/config/content/properties/templates/edit/' . $template->name, $url_options), l(t('delete'), 'admin/config/content/properties/templates/delete/' . $template->name, $url_options));

    $rows[] = array(
        check_plain($template->name),
        check_plain($template->label),
        implode(' ', $links),
    );
  }
  return theme('table', array('rows' => $rows, 'header' => $header, 'empty' => t('No templates found.')));
}

/**
 * Form builder; Add/Edit form for templates.
 */
function properties_template_admin_templates_form($form, &$form_state, $template = NULL) {
  if (!isset($form_state['template'])) {
    if (isset($_GET['entity_type'])) {
      $template = (object) array(
          'label' => '',
          'name' => '',
          'categories' => array(),
          'new' => TRUE,
      );

      $entity_type = $_GET['entity_type'];
      $entity_id = $_GET['entity_id'];
      $field_name = $_GET['field_name'];

      if (entity_get_info($entity_type) && field_info_field($field_name)) {

        $entity = reset(entity_load($entity_type, array($entity_id)));

        if ($entity && isset($entity->{$field_name})) {
          $langcode = field_language($entity_type, $entity, $field_name);

          $field = $entity->{$field_name};
          $categories = array();
          foreach ($field[$langcode] as $item) {
            $categories[$item['category']] = $item['category'];
          }

          if (!(empty($categories))) {
            $category_objects = properties_category_load_multiple($categories);

            foreach ($category_objects as $category_object) {
              $category_object->weight = count($template->categories);
              $template->categories[$category_object->name] = $category_object;
            }
          }
        }
      }
    } else if (empty($template) || !is_object($template)) {
      $template = (object) array(
          'label' => '',
          'name' => '',
          'categories' => array(),
          'new' => TRUE,
      );
    }
    $form_state['template'] = $template;
  }

  // Make sure there are always 3 empty categories displayed.
  $add_empty = TRUE;
  foreach ($form_state['template']->categories as $category) {
    if (empty($category->name)) {
      $add_empty = FALSE;
      break;
    }
  }
  if ($add_empty) {
    // Use a random id for these to trick out browsers.
    $form_state['template']->categories[] = (object) array(
        'name' => '',
        'label' => '',
        'weight' => count($form_state['template']->categories),
    );
  }

  drupal_add_css(drupal_get_path('module', 'properties') . '/properties_admin.css');

  $form['label'] = array(
      '#title' => t('Template label'),
      '#type' => 'textfield',
      '#default_value' => $form_state['template']->label,
      '#maxlength' => 255,
      '#required' => TRUE,
  );

  $form['name'] = array(
      '#type' => 'machine_name',
      '#default_value' => $form_state['template']->name,
      '#maxlength' => 32,
      '#disabled' => empty($form_state['template']->new),
      '#machine_name' => array(
          'exists' => 'properties_template_load',
      ),
      '#description' => t('A unique machine-readable name for this category. It must only contain lowercase letters, numbers, and underscores. This name will be used when using this category.'),
  );

  $categories = array();
  $delta = 0;
  foreach ($form_state['template']->categories as $name => $category) {
    if (empty($category->name) && (isset($form_state['input']['categories'][$delta]))) {
      unset($form_state['input']['categories'][$delta]);
    }
    $element = array();
    $element['category'] = array(
        '#type' => 'textfield',
        '#default_value' => $category->name,
        '#autocomplete_path' => 'properties_autocomplete/category',
        '#ajax' => array(
            'callback' => 'properties_admin_update_label_js',
            'wrapper' => 'attributes-wrapper',
            'effect' => 'fade',
        ),
    );
    // TODO: add categories-wrapper

    $element['label'] = array(
        '#markup' => $category->label,
    );
    // We name the element '_weight' to avoid clashing with elements
    // defined by widget.
    $element['_weight'] = array(
        '#type' => 'weight',
        '#title' => t('Weight for row @number', array('@number' => $delta)),
        '#title_display' => 'invisible',
        '#default_value' => $delta,
        '#weight' => 100,
    );
    $categories[!empty($category->name) ? $category->name : $delta] = $element;
    $delta++;
  }

  $categories += array(
      '#type' => 'fieldset',
      '#theme' => 'properties_template_admin_templates_categories_form',
      '#title' => t('Categories'),
      '#description' => t('Configure the default categories of this template.'),
      '#prefix' => '<div id="attributes-wrapper">',
      '#suffix' => '</div>',
      '#max_delta' => $delta,
      '#tree' => TRUE,
  );
  $categories['add_more'] = array(
      '#type' => 'submit',
      '#name' => 'categories_add_more',
      '#value' => t('Add another category'),
      '#categories' => array('class' => array('field-add-more-submit')),
      '#limit_validation_errors' => array(array('categories')),
      '#submit' => array('properties_template_admin_add_more_submit'),
      '#ajax' => array(
          'callback' => 'properties_template_admin_add_more_js',
          'wrapper' => 'attributes-wrapper',
          'effect' => 'fade',
      ),
  );

  $form['categories'] = $categories;

  if (empty($template->name)) {
    $form['add'] = array(
        '#value' => t('Save'),
        '#type' => 'submit',
        '#submit' => array('properties_template_admin_templates_add'),
    );

    $form['add_another'] = array(
        '#value' => t('Save and add another'),
        '#type' => 'submit',
        '#submit' => array('properties_template_admin_templates_add_another'),
    );
  } else {
    $form['save'] = array(
        '#value' => t('Save'),
        '#type' => 'submit',
        '#submit' => array('properties_template_admin_templates_save'),
    );
  }
  return $form;
}

/**
 * Form builder; Valdate template form.
 */
function properties_template_admin_templates_form_validate($form, &$form_state) {
  // Update template object.
  $form_state['template']->label = $form_state['values']['label'];
  $form_state['template']->name = $form_state['values']['name'];
  $form_state['template']->categories = array();
  uasort($form_state['values']['categories'], '_field_sort_items_helper');
  foreach ($form_state['values']['categories'] as $name => $category) {
    if (is_array($category) && isset($category['category'])) {
      if (empty($category['category'])) {
        $category_object = (object) array(
            'label' => '',
            'name' => '',
            'weight' => count($form_state['template']->categories),
        );
        $form_state['template']->categories[$name] = $category_object;
      } elseif ($category_object = properties_category_load($category['category'])) {
        $category_object->weight = count($form_state['template']->categories);
        $form_state['template']->categories[$category_object->name] = $category_object;
      } else {
        // Update empty category.
        $form_state['template']->categories[$category['category']]->name = $category['category'];
        $form_state['template']->categories[$category['category']]->label = isset($category['label']) ? $category['label'] : '';
        $form_state['template']->categories[$category['category']]->weight = count($form_state['template']->categories);
        $form_state['template']->categories[$category['category']]->new = TRUE;

        // If label is empty, add validation error.
        if (empty($form_state['template']->categories[$category['category']]->label)) {
          // Not possible to do this as a proper form validation step because
          // the form is not rebuilt in case of validation errors.
          drupal_set_message(t('Category %name does not exist, create a category before adding it to the template.', array('%name' => $category['category'])), 'error');
          $form_state['rebuild'] = TRUE;
        }
      }
    }
  }
}

/**
 * Ajax callback in response to a new empty widget being added to the form.
 *
 * This returns the new page content to replace the page content made obsolete
 * by the form submission.
 *
 * @see field_add_more_submit()
 */
function properties_template_admin_add_more_js($form, $form_state) {
  $element = $form['categories'];

  return $element;
}

/**
 * Form builder; Submit callback to save a template and redirect to list.
 */
function properties_template_admin_templates_add($form, &$form_state) {
  properties_template_save($form_state['template']);
  $form_state['redirect'] = 'admin/config/content/properties/templates';
  drupal_set_message(t('Template created.'));
}

/**
 * Form builder; Submit callback to save a template and redirect to list.
 */
function properties_template_admin_templates_save($form, &$form_state) {
  properties_template_save($form_state['template']);
  $form_state['redirect'] = 'admin/config/content/properties/templates';
  drupal_set_message(t('Template updated.'));
}

/**
 * Form builder; Submit callback to save a template and stay on form.
 */
function properties_template_admin_templates_add_another($form, &$form_state) {
  properties_template_save($form_state['template']);
  drupal_set_message(t('Template created.'));
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
function properties_template_admin_add_more_submit($form, &$form_state) {
  $form_state['template']->categories[] = (object) array(
      'label' => '',
      'name' => '',
      'weight' => count($form_state['template']->categories),
  );

  foreach ($form_state['template']->categories as $name => $category) {

    if (is_int($name)) {

      unset($form_state['input']['categories'][$name]);
    }
  }

  $form_state['rebuild'] = TRUE;
}

/**
 * Form builder function; Provide a confirmation form to delete a template.
 */
function properties_template_admin_templates_delete($form, &$form_state, $template) {
  $form['template'] = array(
      '#type' => 'value',
      '#value' => $template,
  );

  return confirm_form($form, t('Delete template %template', array('%template' => $template->label)), '');
}

/**
 * Submit handler to delete a template.
 */
function properties_template_admin_templates_delete_submit($form, &$form_state) {
  properties_template_delete($form_state['values']['template']);
  drupal_set_message(t('Template deleted.'));
}

/**
 * Returns HTML for the categories table in the template table.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: A render element representing the form element.
 *
 * @ingroup themeable
 */
function theme_properties_template_admin_templates_categories_form($variables) {
  $element = $variables['element'];
  $output = '';

  $table_id = 'templates-values';
  $order_class = 'templates-delta-order';
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
    } else {
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
        array('data' => drupal_render($item), 'class' => array('properties-category-row-small')),
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
