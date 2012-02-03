<?php
/**
 * @file pinboard.common.inc
 *    Common utility functions for the pinboard module
 */

/**
 * Utility function that gets a raw list of bookmarks for given user
 *
 * @param $uid
 *    User id to get the bookmark list from
 * @return
 *    An array with bookmark details
 */
function pinboard_user_bookmarks($uid = 0) {

  if (!$uid) {
    return;
  }

  $result = db_query("SELECT bid, url, title, description FROM {pinboard} WHERE uid = %d", $uid);

  while ($r = db_fetch_object($result)) {
    $bookmarks[] = array(
      'id' => $r->bid,
      'url' => $r->url,
      'title' => $r->title,
      'description' => $r->description
    );
  }

  return $bookmarks;
}