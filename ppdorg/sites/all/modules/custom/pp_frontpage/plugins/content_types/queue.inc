<?php

/**
 * @file
 * Visualization of user Core Commits.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Queue stats'),
  'single' => TRUE,
  'category' => t('PP'),
);

/**
 * Render the Queue status.
 */
function pp_frontpage_queue_content_type_render($subtype, $conf, $panel_args, $context) {
  $block = new stdClass();
  $block->module = 'pp_frontpage';
  $block->title = t('Queue status');

  $output = '';
  $queue = DrupalQueue::get('dorg_scrapping');
  $output .= t('%number urls in drupal.org scanning queue',
    array('%number' => $queue->numberOfItems())) . '<br/>';

  $dir = drupal_realpath('public://page_cache');
  $files = scandir($dir);
  $output .= t('%number cached urls', array('%number' => count($files))) . '<br/>';

  $block->content = $output;
  return $block;
}
