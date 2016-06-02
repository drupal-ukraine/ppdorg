<?php


$plugin = array(
  'title' => t('Project description for main page'),
  'single' => TRUE,
  'category' => t('PP'),
  'hook theme' => 'pp_front_page_about_content_type_theme'
);


/**
 * Render of project description widget.
 */
function pp_frontpage_about_content_type_render() {
  $block = new stdClass();
  $block->module = 'pp_frontpage';
  $block->title = '';
  $block->id = 'about_block';
  $block->content = array(
    '#theme' => 'pp_frontpage_about',
  );
  return $block;
}

function pp_front_page_about_content_type_theme(&$theme, $plugin) {
  $theme['pp_frontpage_about'] = array(
    'variables' => array(
      'logo' => '',
    ),
    'path' => $plugin['path'],
    'template' => 'about'
  );
}
