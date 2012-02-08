<?php
/**
 * @file pinboard.common.inc
 *    Common utility functions for the pinboard module
 */

/**
 * Save/update a bookmark
 * @param $bookmark
 *    An object with the bookmark url, title and description
 * @return
 *    FALSE if there was an error. Bookmark id otherwise
 */
function pinboard_save($bookmark) {
  if (!is_object($bookmark) || !$bookmark->url) {
    return FALSE;
  }

  global $user;

  if ($bid = db_result(db_query("SELECT bid FROM {pinboard} WHERE url='%s' AND uid = %d", $bookmark->url, $user->uid)) ) {
    // Update the title/description since it already exists
    db_query("UPDATE {pinboard} SET title='%s', description = '%s' WHERE bid = %d", $bookmark->title, $bookmark->description, $bid);
  }
  else {
    // add new bookmark
    db_query("INSERT INTO {pinboard} (uid, url, title, description, created) "
    ."VALUES (%d, '%s', '%s', '%s', %d)", $user->uid, $bookmark->url, $bookmark->title, $bookmark->description, time());
    $bid = db_last_insert_id('pinboard', 'bid');
  }

  return $bid;
}

/**
 *  Removes a bookmark.
 * @param $bid
 *    The bookmark id
 */
function pinboard_delete($bid) {
  global $user;

  $query = "DELETE FROM {pinboard} WHERE bid = %d";
  if (!user_access('delete any bookmarks', $user)) {
    $query .= " AND uid = ". $user->uid;
  }

  return db_query($query, $bid);
}

/**
 * Utility function for outputting a bookmark link for nodes
 * @param $nid
 *    Node id to generate bookmark link for
 * @return
 *    An html link
 *
 * @todo check if node/link already bookmarked by user
 */
function pinboard_create_link($nid) {

  return l('Bookmark', 'bookmark/add/'. $node->nid, array('absolute' => TRUE, 'attributes' => array('class' => 'pinboard-link', 'data-id' => $node->nid)));
}

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