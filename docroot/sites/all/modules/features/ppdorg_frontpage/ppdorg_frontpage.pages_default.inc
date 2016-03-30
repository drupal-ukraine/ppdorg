<?php
/**
 * @file
 * ppdorg_frontpage.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_handlers().
 */
function ppdorg_frontpage_default_page_manager_handlers() {
  $export = array();

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'term_view__panel_context_118b0053-3bf1-461f-8d0d-248a7f1c4190';
  $handler->task = 'term_view';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'term_panel',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(
      0 => array(
        'identifier' => 'Taxonomy vocabulary',
        'keyword' => 'taxonomy_vocabulary',
        'name' => 'entity:taxonomy_vocabulary',
        'entity_id' => '3',
        'id' => 1,
      ),
    ),
    'relationships' => array(),
    'name' => '',
    'access' => array(
      'logic' => 'and',
      'plugins' => array(
        0 => array(
          'name' => 'term_vocabulary',
          'settings' => array(
            'machine_name' => array(
              'users_list' => 'users_list',
            ),
          ),
          'context' => 'argument_term_1',
          'not' => FALSE,
        ),
      ),
    ),
  );
  $display = new panels_display();
  $display->layout = 'flexible';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'center' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = 'c2c3acbe-5198-4d25-9713-80c781bf18fd';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-b021dc71-4ca8-4c0c-9de3-026687a7c86f';
    $pane->panel = 'center';
    $pane->type = 'community_quality';
    $pane->subtype = 'community_quality';
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
    $pane->uuid = 'b021dc71-4ca8-4c0c-9de3-026687a7c86f';
    $display->content['new-b021dc71-4ca8-4c0c-9de3-026687a7c86f'] = $pane;
    $display->panels['center'][0] = 'new-b021dc71-4ca8-4c0c-9de3-026687a7c86f';
    $pane = new stdClass();
    $pane->pid = 'new-2df86a69-2fba-446b-be4a-aa1492cea63c';
    $pane->panel = 'center';
    $pane->type = 'mentors_stat';
    $pane->subtype = 'mentors_stat';
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
    $pane->uuid = '2df86a69-2fba-446b-be4a-aa1492cea63c';
    $display->content['new-2df86a69-2fba-446b-be4a-aa1492cea63c'] = $pane;
    $display->panels['center'][1] = 'new-2df86a69-2fba-446b-be4a-aa1492cea63c';
    $pane = new stdClass();
    $pane->pid = 'new-cf9aa361-5345-446b-90e1-d30a9ddc2c00';
    $pane->panel = 'center';
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
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = 'cf9aa361-5345-446b-90e1-d30a9ddc2c00';
    $display->content['new-cf9aa361-5345-446b-90e1-d30a9ddc2c00'] = $pane;
    $display->panels['center'][2] = 'new-cf9aa361-5345-446b-90e1-d30a9ddc2c00';
    $pane = new stdClass();
    $pane->pid = 'new-db89e7c5-52cd-4f4f-9799-4bef750172cb';
    $pane->panel = 'center';
    $pane->type = 'users_by_year';
    $pane->subtype = 'users_by_year';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array();
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 3;
    $pane->locks = array();
    $pane->uuid = 'db89e7c5-52cd-4f4f-9799-4bef750172cb';
    $display->content['new-db89e7c5-52cd-4f4f-9799-4bef750172cb'] = $pane;
    $display->panels['center'][3] = 'new-db89e7c5-52cd-4f4f-9799-4bef750172cb';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $export['term_view__panel_context_118b0053-3bf1-461f-8d0d-248a7f1c4190'] = $handler;

  return $export;
}

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
  $display->layout = 'onecol';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'left' => NULL,
      'right' => NULL,
      'top' => NULL,
      'bottom' => NULL,
      'middle' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '6715cba5-5f91-4382-b321-b1e812f40fe2';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-498c3c5b-4816-4cd3-a57e-dae36d0497e2';
    $pane->panel = 'middle';
    $pane->type = 'global_countries_members';
    $pane->subtype = 'global_countries_members';
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
    $pane->uuid = '498c3c5b-4816-4cd3-a57e-dae36d0497e2';
    $display->content['new-498c3c5b-4816-4cd3-a57e-dae36d0497e2'] = $pane;
    $display->panels['middle'][0] = 'new-498c3c5b-4816-4cd3-a57e-dae36d0497e2';
    $pane = new stdClass();
    $pane->pid = 'new-bb42ed93-23ea-4427-a5cb-5ed2a90c3180';
    $pane->panel = 'middle';
    $pane->type = 'members_by_country';
    $pane->subtype = 'members_by_country';
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
    $pane->uuid = 'bb42ed93-23ea-4427-a5cb-5ed2a90c3180';
    $display->content['new-bb42ed93-23ea-4427-a5cb-5ed2a90c3180'] = $pane;
    $display->panels['middle'][1] = 'new-bb42ed93-23ea-4427-a5cb-5ed2a90c3180';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['homepage'] = $page;

  return $pages;

}
