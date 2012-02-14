<?php
/**
 * @file pinboard.common.inc
 *    Common utility functions for the pinboard module
 */

define('PINBOARD_ADD', 0);
define('PINBOARD_DELETE', 1);
define('PINBOARD_UPDATE', 2);
define('PINBOARD_MODERATE', 3); // The bookmark got deleted by an administrator

/**
 * Save/update a bookmark
 *
 * @param $bookmark
 *    An object with the bookmark uid, url, title and description
 * @return
 *    FALSE if there was an error. Bookmark id otherwise
 */
function pinboard_save($bookmark) {
  if (!is_object($bookmark) || !$bookmark->url) {
    return FALSE;
  }

  if (!$bookmark->uid) {
    // get the uid of the current user
    global $user;
    $bookmark->uid = $user->uid;
  }

  if ($bid = db_result(db_query("SELECT bid FROM {pinboard} WHERE url='%s' AND uid = %d", $bookmark->url, $bookmark->uid)) ) {
    // Update the title/description since it already exists
    db_query("UPDATE {pinboard} SET title='%s', description = '%s' WHERE bid = %d", $bookmark->title, $bookmark->description, $bid);
  }
  else {
    $nid = isset($bookmark->nid) ? $bookmark->nid : 0;
    $timestamp = time();

    // add new bookmark
    db_query("INSERT INTO {pinboard} (uid, nid, url, title, description, created) "
    ."VALUES (%d, %d, '%s', '%s', '%s', %d)", $bookmark->uid, $nid, $bookmark->url, $bookmark->title, $bookmark->description, $timestamp);
    $bid = db_last_insert_id('pinboard', 'bid');

    pinboard_log_activity($bid, $bookmark->uid, PINBOARD_ADD, $timestamp);
  }

  return $bid;
}

/**
 * Removes a bookmark.
 *
 * @param $bid
 *    The bookmark id
 *
 * @return TRUE if delete successful
 */
function pinboard_delete($bid) {
  global $user;

  $owner = db_result(db_query("SELECT uid FROM {pinboard} WHERE bid = %d", $bid));

  $query = "DELETE FROM {pinboard} WHERE bid = %d";
  $action = PINBOARD_DELETE;
  if (!user_access('delete any bookmarks', $user) || $user->uid == $owner) {
    $query .= " AND uid = ". $user->uid;
  }
  else {
    $action = PINBOARD_MODERATE;
  }

  if ($ret = db_query($query, $bid)) {
    pinboard_log_activity($bid, $action, $owner);

    return TRUE;
  }

  return;
}

/**
 * Utility function for outputting a bookmark link for nodes
 *
 * @param $nid
 *    Node id to generate bookmark link for
 * @return
 *    An html link
 */
function pinboard_create_link($nid) {
  global $user;

  if ($bid = db_result(db_query("SELECT bid FROM {pinboard} WHERE nid = %d AND uid = %d", $nid, $user->uid))) {
    return l('Remove bookmark', 'bookmark/delete/'. $bid, array('absolute' => TRUE, 'attributes' => array('class' => 'pinboard-link pinboard-bookmark', 'data-id' => $nid)));
  }
  else {
    return l('Bookmark', 'bookmark/add/'. $nid, array('absolute' => TRUE, 'attributes' => array('class' => 'pinboard-link', 'data-id' => $nid)));
  }
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

  $result = db_query("SELECT * FROM {pinboard} WHERE uid = %d ORDER BY created DESC", $uid);

  while ($bookmark = db_fetch_object($result)) {
    $bookmarks[] = $bookmark;
  }

  return $bookmarks;
}

/**
 * Return the url hash
 */
function pinboard_url_hash($url) {
  return md5($url);
}

/**
 * Get bookmark title/description from the given url
 * @param $url
 *    The url to retrieve information for
 */
function pinboard_url_lookup($url) {
  $hash = pinboard_url_hash($url);

  if ($cache = cache_get('pinboard:url__'. $hash)) {
    return $cache->data;
  }

  $response = drupal_http_request($url);

  $cache = new StdClass;
  $cache->cid = 'pinboard:url__'. $hash;
  $cache->created = REQUEST_TIME;
  $cache->expire = CACHE_PERMANENT;

  $cache->data = array(
    'code' => $response->code,
    'headers' => $response->headers,
    'url' => isset($response->redirect_url) ? $response->redirect_url : $url
  );

  if ($response->code == '200' && !empty($response->data)) {
    $doc = new DOMDocument();
    @$doc->loadHTML($response->data);

    $titles = $doc->getElementsByTagName('title');
    $cache->data['title'] = check_plain($titles->item(0)->nodeValue);

    $metas = $doc->getElementsByTagName('meta');
    for ($i = 0; $i < $metas->length; $i++) {
      $meta = $metas->item($i);
      if ($meta->getAttribute('name') == 'description') {
        $cache->data['description'] = check_plain($meta->getAttribute('content'));
      }
    }

    cache_set($cache->cid, $cache->data);
    return $cache->data;
  }
}

/**
 * Logs bookmark activity for a given user
 *
 * @param $bid
 *    The bookmark id
 * @param $action
 *    PINBOARD_ADD, PINBOARD_DELETE, PINBOARD_EDIT, PINBOARD_MODERATE
 * @param $uid
 *    The user id. Will get the current user id if none specified
 * @param $timestamp
 *    The activity timestamp. defaults to current time if none given.
 *
 * @return Boolean
 *    The success status of the log
 */
function pinboard_log_activity($bid, $action, $uid = NULL, $timestamp = NULL) {
  if (!$uid) {
    global $user;
    $uid = $user->uid;
  }

  if (!$timestamp) {
    $timestamp = time();
  }

  return db_query("INSERT INTO {pinboard_activity} VALUES(%d, %d, %d, %d)", $bid, $uid, $activity, $timestamp);
}