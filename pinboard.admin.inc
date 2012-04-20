<?php

/**
 * @file pinboard.admin.inc
 *
 * Administrative settings
 */

/**
 *
 */
function pinboard_admin_settings() {
  $form = array();

  $form['pinboard_strings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Strings'),
    '#collapsible' => FALSE
  );

  $form['pinboard_strings']['pinboard_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => variable_get('pinboard_title', 'Bookmarks'),
    '#required' => TRUE,
    '#description' => 'Used on user pages/tabs'
  );

  return system_settings_form($form);
}