<?php

$plugin = array(
  'title' => t('Association individual memebers by country stat'),
  'single' => TRUE,
  'category' => t('PP'),
);

/**
 * Render global individual members statistics.
 */
function pp_frontpage_members_by_country_content_type_render($subtype, $conf, $panel_args, $context) {
  $block = new stdClass();
  $block->module = 'pp_frontpage';
  $block->title = t('Drupal Association individual members by country statistics');

  // Getting info for showing count of individual members at this country.
  $members_stat = _ppgetstat_get_individual_members();
  $i = 1; $rows = array();
  foreach ($members_stat['members_country_count'] as $country => $count) {
    $rows[] = array(
      $i,
      $country,
      $count
    );
    $i++;
  }

  $block->content = array(
    '#theme' => 'table',
    '#header' => array(
      t('Position'),
      t('Country'),
      t('Count of members')
    ),
    '#rows' => $rows
  );

  return $block;
}