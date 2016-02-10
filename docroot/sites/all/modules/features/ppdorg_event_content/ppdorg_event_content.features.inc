<?php
/**
 * @file
 * ppdorg_event_content.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function ppdorg_event_content_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function ppdorg_event_content_node_info() {
  $items = array(
    'event' => array(
      'name' => t('event'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
