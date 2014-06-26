<?php
/**
 * @file
 * ppdorg_user_content.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_handlers().
 */
function ppdorg_user_content_default_page_manager_handlers() {
  $export = array();

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'node_view_panel_context';
  $handler->task = 'node_view';
  $handler->subtask = '';
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
    'access' => array(
      'plugins' => array(
        0 => array(
          'name' => 'entity_bundle:node',
          'settings' => array(
            'type' => array(
              'user' => 'user',
            ),
          ),
          'context' => 'argument_entity_id:node_1',
          'not' => FALSE,
        ),
      ),
      'logic' => 'and',
    ),
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
  $display->uuid = '1709e7e1-11f5-4898-9ff7-e039cd049d60';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-44f45882-2b86-456b-b2e7-d3ee92070b7d';
    $pane->panel = 'middle';
    $pane->type = 'node_content';
    $pane->subtype = 'node_content';
    $pane->shown = FALSE;
    $pane->access = array();
    $pane->configuration = array(
      'links' => 1,
      'no_extras' => 1,
      'override_title' => 0,
      'override_title_text' => '',
      'identifier' => '',
      'link' => 1,
      'leave_node_title' => 0,
      'build_mode' => 'teaser',
      'context' => 'argument_entity_id:node_1',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '44f45882-2b86-456b-b2e7-d3ee92070b7d';
    $display->content['new-44f45882-2b86-456b-b2e7-d3ee92070b7d'] = $pane;
    $display->panels['middle'][0] = 'new-44f45882-2b86-456b-b2e7-d3ee92070b7d';
    $pane = new stdClass();
    $pane->pid = 'new-297a6767-5461-4c35-a4bc-239217190770';
    $pane->panel = 'middle';
    $pane->type = 'visualization';
    $pane->subtype = 'visualization';
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
    $pane->uuid = '297a6767-5461-4c35-a4bc-239217190770';
    $display->content['new-297a6767-5461-4c35-a4bc-239217190770'] = $pane;
    $display->panels['middle'][1] = 'new-297a6767-5461-4c35-a4bc-239217190770';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-44f45882-2b86-456b-b2e7-d3ee92070b7d';
  $handler->conf['display'] = $display;
  $export['node_view_panel_context'] = $handler;

  return $export;
}
