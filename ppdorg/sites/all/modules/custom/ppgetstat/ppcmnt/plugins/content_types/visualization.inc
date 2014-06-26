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
  'title' => t('Comments visualization'),
  'single' => TRUE,
  'required context' => new ctools_context_required(t('Node'), 'node'),
  'category' => t('PP'),
);

/**
 * Render the user profile content type.
 */
function ppcmnt_visualization_content_type_render($subtype, $conf, $panel_args, $context) {
  $node = isset($context->data) ? clone($context->data) : NULL;
  if (!$node) {
    return NULL;
  }

  $block = new stdClass();
  $block->module = 'user-profile';
  $block->title = t('Comments');

  drupal_add_js(array('ppcmnt' => array('data' => base_path() . 'node/' . $node->nid . '/comments/data.tsv')), 'setting');
  drupal_add_js('http://d3js.org/d3.v3.js');
  drupal_add_js(drupal_get_path('module', 'ppcmnt') . '/plugins/content_types/visualization.js');
  drupal_add_css(drupal_get_path('module', 'ppcmnt') . '/plugins/content_types/visualization.css');

  $block->content = '<div class="comments-vizualization"></div>';

  return $block;
}