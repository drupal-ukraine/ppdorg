<?php
/**
 * @file
 * ppdorg_frontpage.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_pages().
 */
function ppdorg_frontpage_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'homepage';
  $page->task = 'page';
  $page->admin_title = 'Homepage';
  $page->admin_description = '';
  $page->path = 'home';
  $page->access = array(
    'type' => 'none',
    'settings' => NULL,
  );
  $page->menu = array();
  $page->arguments = array();
  $page->conf = array(
    'admin_paths' => FALSE,
  );
  $page->default_handlers = array();
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_homepage_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'homepage';
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
  $display->layout = 'twocol_stacked';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'left' => NULL,
      'right' => NULL,
      'top' => NULL,
      'bottom' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '6715cba5-5f91-4382-b321-b1e812f40fe2';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-7129b02f-9bfd-4804-b6b1-306c0b8666d9';
    $pane->panel = 'left';
    $pane->type = 'most';
    $pane->subtype = 'most';
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
    $pane->uuid = '7129b02f-9bfd-4804-b6b1-306c0b8666d9';
    $display->content['new-7129b02f-9bfd-4804-b6b1-306c0b8666d9'] = $pane;
    $display->panels['left'][0] = 'new-7129b02f-9bfd-4804-b6b1-306c0b8666d9';
    $pane = new stdClass();
    $pane->pid = 'new-0cf1e219-41b6-4177-b1ff-442afc226f71';
    $pane->panel = 'right';
    $pane->type = 'adduser';
    $pane->subtype = 'adduser';
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
    $pane->uuid = '0cf1e219-41b6-4177-b1ff-442afc226f71';
    $display->content['new-0cf1e219-41b6-4177-b1ff-442afc226f71'] = $pane;
    $display->panels['right'][0] = 'new-0cf1e219-41b6-4177-b1ff-442afc226f71';
    $pane = new stdClass();
    $pane->pid = 'new-3adef721-fbc9-4fc8-a16b-c25202b9ce3c';
    $pane->panel = 'right';
    $pane->type = 'views_panes';
    $pane->subtype = 'users-panel_pane_1';
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
    $pane->uuid = '3adef721-fbc9-4fc8-a16b-c25202b9ce3c';
    $display->content['new-3adef721-fbc9-4fc8-a16b-c25202b9ce3c'] = $pane;
    $display->panels['right'][1] = 'new-3adef721-fbc9-4fc8-a16b-c25202b9ce3c';
    $pane = new stdClass();
    $pane->pid = 'new-d26d5ad6-b7a3-426a-a089-c623468e44c0';
    $pane->panel = 'right';
    $pane->type = 'queue';
    $pane->subtype = 'queue';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array();
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = 'd26d5ad6-b7a3-426a-a089-c623468e44c0';
    $display->content['new-d26d5ad6-b7a3-426a-a089-c623468e44c0'] = $pane;
    $display->panels['right'][2] = 'new-d26d5ad6-b7a3-426a-a089-c623468e44c0';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-d26d5ad6-b7a3-426a-a089-c623468e44c0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['homepage'] = $page;

  return $pages;

}
