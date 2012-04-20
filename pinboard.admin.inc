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

  $form['pinboard_strings']['pinboard_bookmarklet_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Bookmarklet title'),
    '#default_value' => variable_get('pinboard_bookmarklet_text', 'Bookmarklet'),
    '#required' => TRUE,
    '#description' => 'Bookmarklet text. Used when dragged to the bookmarks bar in your browser'
  );

  return system_settings_form($form);
}