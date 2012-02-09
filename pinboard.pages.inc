<?php

/**
 * @file
 *    pinboard.pages.inc - Pinboard page(s)
 */

/**
 * Callback function for the user bookmarks page
 *
 * @param $account
 *    The user object
 * @return
 *  An html item list with the bookmarks
 *
 * @todo show link to bookmark/add
 * @todo show descriptions
 */
function pinboard_user_page($account) {
  global $user;
  $account_data = unserialize($account->data);

  // set to private by default
  $is_private = $account_data['pinboard_visibility'] == 'private' || !isset($account_data['pinboard_visibility']);

  if ($is_private && $account->uid !== $user->uid) {
    return 'There is nothing to see here. Carry on.';
  }

  $bookmarks = pinboard_load_bookmarks($account->uid);


  if (!$bookmarks || empty($bookmarks)) {
    return 'No bookmarks yet.';
  }

  foreach($bookmarks as $bookmark) {
    $list[] = l($bookmark->title, $bookmark->url, array('absolute' => TRUE) ) .' '.
      l('&times;', 'bookmark/delete/'. $bookmark->bid, array('absolute' => TRUE, 'html' => TRUE, 'attributes' => array('class' => 'pinboard-link')));
  }

  return theme_item_list($list, 'Bookmarks', 'ul', array('class' => 'pinboard-list pinboard-'. $account_data['pinboard_visibility']));
}

/**
 * Callback function for bookmark actions (add/delete)
 *
 * @param $action
 *    The action to perform
 * @param $content_id
 *    The id of content to perform an action on. Can be a node id for add.
 *    Always a bookmark id for delete.
 */
function pinboard_actions_page($action, $content_id = NULL) {
  if (!in_array($action, array('add', 'delete')) || !is_numeric($content_id)) {
    $error = t('Unexpected error. Unsupported action performed!');
    drupal_set_message($error, 'error');
    drupal_access_denied();
    return;
  }

  global $user;

  // Oh, Devel, we love you but not here
  $GLOBALS['devel_shutdown'] = FALSE;

  $js = $_REQUEST['js'];

  if ($action == 'add') {
    $node = node_load($content_id);

    $bookmark = new StdClass;
    $bookmark->uid   = $user->uid;
    $bookmark->nid    = $node->nid;
    $bookmark->url    = url('node/'. $node->nid, array('absolute' => TRUE));
    $bookmark->title  = $node->title;
    $bookmark->description = check_plain($node->teaser);

    if ($bid = pinboard_save($bookmark)) {
      $message = t('Bookmark successfully added');
      $link = 'bookmark/delete/'. $bid;
      $text = 'Remove bookmark';
      $status = 'bookmarked';
    }
    else {
      $error = t('Could not add bookmark.');
    }

  }
  else if ($action == 'delete') {
    pinboard_delete($content_id);
    $message = t('Bookmark successfully deleted');
    $text = 'Bookmark';
    $status = 'remove';

    if (is_numeric($_REQUEST['nid']) ) {
      $link = url('bookmark/add/'. $_REQUEST['nid'], array('absolute' => TRUE));
      $status = 'unbookmarked';
    }
  }

  if ($js) {
    drupal_set_header('Content-Type: text/javascript; charset=utf-8');
    print drupal_to_js(array(
      'status' => $status,
      'link' => $link,
      'text' => $text,
    ));
  }
  else {
    drupal_set_message($message);
    drupal_goto();
  }
}