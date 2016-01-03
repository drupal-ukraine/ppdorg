<?php

/**
 * @file
 * Visualization of user Commits.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Commits visualization'),
  'single' => TRUE,
  'required context' => new ctools_context_required(t('Node'), 'node'),
  'category' => t('PP'),
);

/**
 * Render the visualization for commits.
 */
function ppgetstat_ppgetstat_visualization_content_type_render($subtype, $conf, $panel_args, $context) {
  $node = isset($context->data) ? clone($context->data) : NULL;
  if (!$node) {
    return NULL;
  }

  $block = new stdClass();
  $block->module = 'ppgetstat';
  $block->title = t('Commits');

  drupal_add_js(array('ppgetstat' => array('data' => base_path() . 'node/' . $node->nid . '/commits/data.tsv')), 'setting');
  _ppgetstat_add_d3_js();
  drupal_add_js(drupal_get_path('module', 'ppgetstat') . '/plugins/content_types/ppgetstat_visualization.js');
  drupal_add_css(drupal_get_path('module', 'ppgetstat') . '/plugins/content_types/ppgetstat_visualization.css');

  $block->content = '<div class="commits-vizualization"></div>';

  return $block;
}
