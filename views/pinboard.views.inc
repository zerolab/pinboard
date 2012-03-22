<?php

/**
 * @file pinboard.views.inc
 *
 * Exposing data to the Views module
 */

/**
 * Implementation of hook_views_data()
 *
 * Describe the pinboard table to Views
 *
 * @see hook_views_data() http://views.doc.logrus.com/group__views__hooks.html#g227057901681e4a33e33c199c7a8c989
 */
function pinboard_views_data() {
  $data = array();

  $data['pinboard']['table']['group'] = t('Pinboard');

  // Joins
  $data['pinboard']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid'
    )
  );

  $data['pinboard']['uid'] = array(
    'title' => t('User'),
    'help' => t('The user that bookmarked a node'),
    'relationship' => array(
      'base' => 'users',
      'base field' => 'uid',
      'handler' => 'views_handler_relationship',
      'label' => t('User'),
    ),
  );

  $data['pinboard']['nid'] = array(
    'title' => t('Node'),
    'help' => t('The bookmarked node'),
    'relationship' => array(
      'base' => 'node',
      'base field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Node'),
    )
  );

  // Bookmark content link
  $data['pinboard']['bookmark_ops'] = array(
    'title' => t('Bookmark link'),
    'help' => t('Provide a bookmark/unbookmark link'),
    'field' => array(
      'handler' => 'pinboard_handler_field_bookmark_ops',
    ),
  );

  return $data;
}

function pinboard_views_data_alter(&$data) {
  $data['node']['pinboard_link'] = array(
    'field' => array(
      'title' => t('Bookmark/Unbookmark link'),
      'help'  => t('Display the add/remove bookmark link for regular nodes.'),
      'handler' => 'pinboard_handler_field_pinboard_link'
    ),
  );
}

/**
 * Implementation of hook_views_handlers()
 */
function pinboard_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'pinboard') .'/views',
    ),
    'handlers' => array(
      'pinboard_handler_field_bookmark_ops' => array(
        'parent' => 'views_handler_field'
      ),
      'pinboard_handler_field_pinboard_link' => array(
        'parent' => 'views_handler_field'
      ),
    )
  );
}