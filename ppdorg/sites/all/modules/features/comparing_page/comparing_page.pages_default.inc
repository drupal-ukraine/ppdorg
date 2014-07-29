<?php
/**
 * @file
 * comparing_page.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_pages().
 */
function comparing_page_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'users_stats_comparing';
  $page->task = 'page';
  $page->admin_title = 'Users stats comparing';
  $page->admin_description = '';
  $page->path = 'comparing';
  $page->access = array();
  $page->menu = array();
  $page->arguments = array();
  $page->conf = array(
    'admin_paths' => FALSE,
  );
  $page->default_handlers = array();
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_users_stats_comparing_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'users_stats_comparing';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Panel',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
  );
  $display = new panels_display();
  $display->layout = 'onecol';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'middle' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = 'e7aaee4b-f52f-4214-9b95-ac62ec734c1b';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-ecad5f8f-d642-40ff-8c39-23e7005ea71c';
    $pane->panel = 'middle';
    $pane->type = 'comparing_table';
    $pane->subtype = 'comparing_table';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array();
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'ecad5f8f-d642-40ff-8c39-23e7005ea71c';
    $display->content['new-ecad5f8f-d642-40ff-8c39-23e7005ea71c'] = $pane;
    $display->panels['middle'][0] = 'new-ecad5f8f-d642-40ff-8c39-23e7005ea71c';
    $pane = new stdClass();
    $pane->pid = 'new-f1a7e7cc-e7a6-4fef-9805-e72e76e2aead';
    $pane->panel = 'middle';
    $pane->type = 'comparing_chart';
    $pane->subtype = 'comparing_chart';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array();
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = 'f1a7e7cc-e7a6-4fef-9805-e72e76e2aead';
    $display->content['new-f1a7e7cc-e7a6-4fef-9805-e72e76e2aead'] = $pane;
    $display->panels['middle'][1] = 'new-f1a7e7cc-e7a6-4fef-9805-e72e76e2aead';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-ecad5f8f-d642-40ff-8c39-23e7005ea71c';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['users_stats_comparing'] = $page;

  return $pages;

}
