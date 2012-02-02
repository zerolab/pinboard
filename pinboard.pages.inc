<?php

/**
 * @file
 *    pinboard.pages.inc - Pinboard page(s)
 */

function pinboard_user_page($account) {
  //dpm($account);

  $result = db_query("SELECT * FROM {pinboard} WHERE uid = %d ORDER BY created DESC", $account->uid);

  while ($bookmark = db_fetch_object($result)) {
    $bookmarks[] = l($bookmark->title, $bookmark->url, array('absolute' => TRUE));
  }

  return theme_item_list($bookmarks, 'Bookmarks');
}