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
function pinboard_load_bookmarks($uid = 0) {

  if (!$uid) {
    return;
  }

  $result = db_query("SELECT * FROM {pinboard} WHERE uid = %d", $uid);

  while ($bookmark = db_fetch_object($result)) {
    $bookmarks[] = $bookmark;
  }

  return $bookmarks;
}