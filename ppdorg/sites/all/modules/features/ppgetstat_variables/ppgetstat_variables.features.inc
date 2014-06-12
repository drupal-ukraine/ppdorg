<?php
/**
 * @file
 * ppgetstat_variables.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function ppgetstat_variables_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}
