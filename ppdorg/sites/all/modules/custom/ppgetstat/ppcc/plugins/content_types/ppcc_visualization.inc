<?php

/**
 * @file
 * Visualization of user Comments.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Core commits visualization'),
  'single' => TRUE,
  'required context' => new ctools_context_required(t('Node'), 'node'),
  'category' => t('PP'),
);

/**
 * Render the visualization for comments.
 */
function ppcc_ppcc_visualization_content_type_render($subtype, $conf, $panel_args, $context) {
  $node = isset($context->data) ? clone($context->data) : NULL;
  if (!$node) {
    return NULL;
  }

  $block = new stdClass();
  $block->module = 'ppcc';
  $block->title = t('Core Commits');

  drupal_add_js(array('ppcc' => array('data' => base_path() . 'node/' . $node->nid . '/core_commits/data.tsv')), 'setting');
  _ppgetstat_add_d3_js();
  drupal_add_js(drupal_get_path('module', 'ppcc') . '/plugins/content_types/ppcc_visualization.js');
  drupal_add_css(drupal_get_path('module', 'ppcc') . '/plugins/content_types/ppcc_visualization.css');

  $block->content = '<div class="core-commits-vizualization"></div>';

  return $block;
}
