<?php


$plugin = array(
  'title' => t('Project description for main page'),
  'single' => TRUE,
  'category' => t('PP'),
  'hook theme' => array(
    'pp_frontpage_about' => array(
      'path' => drupal_get_path('module', 'pp_frontpage') . '/plugins/content_types',
      'template' => 'about',
      'variables' => array(
        'text' => NULL,
      ),
    ),
  ),
);


/**
 * Render of project description widget.
 */
function pp_frontpage_about_content_type_render() {
  $block = new stdClass();
  $block->module = 'pp_frontpage';
  $block->title = '';
  $block->id = 'about_block';
  $block->content = '
    <div id="logo-large"><img src="/sites/all/themes/custom/ppdorg_bootstrap/logo-large.png"></div>
    <h3>
    ' . t('PPDORG is a project that scans Drupal.org contributions and prepare reports about contributions, grouped by country parameter.') . ' <br>
    ' . t('Current version investigate Drupal communities of the eastern Europe') . '
    </h3>
  ';
  $block->content = array(
    '#theme' => 'pp_frontpage_about',
  );

  return $block;
}
