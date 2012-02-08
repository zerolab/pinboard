<?php

/**
 * @file
 *    pinboard.pages.inc - Pinboard page(s)
 */

function pinboard_user_page($account) {

  $bookmarks = pinboard_load_bookmarks($account->uid);

  foreach($bookmarks as $bookmark) {
    $list[] = l($bookmark->title, $bookmark->url, array('absolute' => TRUE)) .' '.
      l('&times;', 'bookmark/delete/'. $bookmark->bid, array('absolute' => TRUE, 'html' => TRUE, 'attributes' => array('class' => 'pinboard-link')));
  }

  return theme_item_list($list, 'Bookmarks', 'ul', array('class' => 'pinboard-list'));
}