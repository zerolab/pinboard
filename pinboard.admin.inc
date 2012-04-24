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

  $bookmarklet_text = variable_get('pinboard_bookmarklet_text', 'Bookmarklet');
  $form['pinboard_strings']['pinboard_bookmarklet_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Bookmarklet title'),
    '#default_value' => $bookmarklet_text,
    '#required' => TRUE,
    '#description' => 'Bookmarklet text. Used when dragged to the bookmarks bar in your browser'
  );

  $form['pinboard_strings']['pinboard_text_add'] = array(
    '#type' => 'textfield',
    '#title' => 'Add bookmark text',
    '#default_value' => variable_get('pinboard_add_text', 'Bookmark'),
    '#description' => 'Add bookmark link text. Defaults to <em>Bookmark</em>'
  );

  $form['pinboard_strings']['pinboard_text_remove'] = array(
    '#type' => 'textfield',
    '#title' => 'Remove bookmark text',
    '#default_value' => variable_get('pinboard_delete_text', 'Remove bookmark'),
    '#description' => 'Remove bookmark link text. Defaults to <em>Remove bookmark</em>'
  );

  $form['pinboard_help_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Help'),
  );


  $path = url('bookmark/let', array('absolute' => TRUE));
  $path = preg_replace('/http[s]?\:/', '', $path);

  $bookmarklet = <<<EOD
You have no bookmarked items. Why not use the <a href="javascript:(function(){var d=document,s=d.createElement('scri'+'pt');s.src='$path?x='+(Math.random());(d.getElementsByTagName('head')[0]||d.getElementsByTagName('body')[0]).appendChild(s);}());" id="pinboard-bookmarklet">$bookmarklet_text</a>
<textarea id="pinboard-bookmarklet-code">javascript:(function(){var d=document,s=d.createElement('scri'+'pt');s.src='$path?x='+(Math.random());(d.getElementsByTagName('head')[0]||d.getElementsByTagName('body')[0]).appendChild(s);}());</textarea> link to get you started? Just drag it to your browser's bookmark bar!
EOD;

  $form['pinboard_help_settings']['pinboard_help'] = array(
    '#type' => 'textarea',
    '#title' => t('Help message'),
    '#description' => 'Help users use the module. By default provides the bookmarklet code',
    '#default_value' => variable_get('pinboard_help', $bookmarklet)
  );

  return system_settings_form($form);
}