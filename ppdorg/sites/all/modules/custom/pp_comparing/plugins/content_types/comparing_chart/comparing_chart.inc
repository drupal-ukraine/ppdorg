<?php

/**
 * @file
 * Visualization of user stats via chart.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('User stats comparing via chart'),
  'single' => TRUE,
  'category' => t('PP'),
);

/**
 * Render the chart for user stats.
 */
function pp_comparing_comparing_chart_content_type_render($subtype, $conf, $panel_args, $context) {
  $block = new stdClass();
  $block->module = 'pp_comparing';
  $block->title = t('Users stats');

  if (empty($_GET['users'])) {
    return FALSE;
  }

  $data_tsv_url = base_path() . 'comparing/' . $_GET['users'] . '/data.tsv';

  if (isset($_GET['types'])) {
    $data_tsv_url .= '?types=' . $_GET['types'];
  }

  drupal_add_js(array('pp_comparing' => array('data' => $data_tsv_url)), 'setting');
  _ppgetstat_add_d3_js();
  drupal_add_js(drupal_get_path('module', 'pp_comparing') . '/plugins/content_types/comparing_chart/pp_comparing_chart.js');
  drupal_add_css(drupal_get_path('module', 'pp_comparing') . '/plugins/content_types/comparing_chart/pp_comparing_chart.css');

  $block->content = '<div class="pp-comparing-chart"></div>';

  return $block;
}
