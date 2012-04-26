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

    pinboard_log_activity($bid, PINBOARD_ADD, $bookmark->uid, $timestamp);
  }

  return $bid;
}

/**
 * Build a bookmark object from a node
 *
 * @param $nid
 *    The node id
 *
 * @return $bookmark
 *    A bookmark object ready to be saved
 */
function pinboard_node_bookmark($nid) {
  $node = node_load($nid);

  if (!$node) {
    return;
  }

  global $user;

  $bookmark = new StdClass();

  $bookmark->uid    = $user->uid;
  $bookmark->nid    = $node->nid;
  $bookmark->url    = url('node/'. $node->nid, array('absolute' => TRUE));
  $bookmark->title  = $node->title;
  $bookmark->description = strip_tags($node->teaser);

  // skip module_invoke_all as it doesn't support references
  // @see http://drupal.org/node/353494
  $hook = 'pinboard_node_bookmark';
  foreach (module_implements($hook) as $module) {
    $function = $module . '_' . $hook;
    $function($bookmark, $node);
  }

  return $bookmark;
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
    return l(variable_get('pinboard_text_remove', 'Remove bookmark'), 'bookmark/delete/'. $bid, array('absolute' => TRUE, 'attributes' => array('class' => 'pinboard-link pinboard-bookmarked', 'data-id' => $nid)));
  }
  else {
    return l(variable_get('pinboard_text_add', 'Bookmark'), 'bookmark/add/'. $nid, array('absolute' => TRUE, 'attributes' => array('class' => 'pinboard-link', 'data-id' => $nid)));
  }
}

/**
 * Utility function that gets a raw list of bookmarks for given user
 *
 * @param $uid
 *    User id to get the bookmark list from
 * @param $bids mixed
 *    bookmark id OR array of bookmark IDs. if none given, all are returned
 * @param $paged bool
 *    turn paged queries on/off.
 *
 * @return
 *    An array with bookmark details
 */
function pinboard_load_bookmarks($uid = 0, $bids = NULL, $paged = FALSE) {

  if (!$uid || !is_numeric($uid)) {
    return;
  }

  $query = "SELECT * FROM {pinboard} WHERE uid = $uid";

  if ($bids) {
    if (is_array($bids) && count($bids)) {
      $query .= " AND bid IN (". implode(', ', $bids) .")";
    }
    else {
      $query .= " AND bid = $bids";
    }
  }

  $query .= " ORDER BY created DESC";
  if (!$paged) {
    $results = db_query($query);
  }
  else {
    $results = pager_query($query, 10, 0, NULL);
  }

  while ($bookmark = db_fetch_object($results)) {
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
  $cache_id = 'url__'. pinboard_url_hash($url);

  if ($cache = cache_get($cache_id, 'cache_pinboard')) {
    return $cache->data;
  }

  $response = drupal_http_request($url);

  $data = array(
    'code' => $response->code,
    'headers' => $response->headers,
    'url' => isset($response->redirect_url) ? $response->redirect_url : $url,
    'title' => '',
    'description' => '',
  );

  if ($response->code == '200' && !empty($response->data)) {

    $charset = 'utf-8';
    if (isset($response->headers['Content-Type'])) {
      $charset = trim(preg_replace('/.*charset=([^\:].*)/', '$1', $response->headers['Content-Type']));
    }

    if (strtolower($charset) != 'utf-8') {
      $response->data = drupal_convert_to_utf8($response->data, $charset);
      $data['title'] = preg_match('/<title>(.*?)<\/title>/i', $response->data, $matches) ? $matches[1] : '';
      $data['description'] = preg_match('/<meta[\s]+[^>]*?name[\s]?=[\s\"\']?description[\s\"\']?+content[\s]?=[\s\"\']+(.*?)[\"\']+.*?>/i', $response->data, $matches) ? $matches[1] : '';
    }
    else {
      $doc = new DOMDocument('1.0', 'UTF-8');
      @$doc->loadHTML($response->data, 'UTF-8');
      $doc->saveHTML();

      $titles = $doc->getElementsByTagName('title');
      $data['title'] = check_plain($titles->item(0)->nodeValue);

      $metas = $doc->getElementsByTagName('meta');
      for ($i = 0; $i < $metas->length; $i++) {
        $meta = $metas->item($i);
        if ($meta->getAttribute('name') == 'description') {
          $data['description'] = check_plain($meta->getAttribute('content'));
        }
      }
    }
  }

  $data = module_invoke_all('pinboard_url_lookup', $data);

  if (!empty($data['description'])) {
    cache_set($cache_id, $data, 'cache_pinboard');
  }

  return $data;
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
 *
 * @todo Ensure unique entries!
 */
function pinboard_log_activity($bid, $action, $uid = NULL, $timestamp = NULL) {
  if (!$uid) {
    global $user;
    $uid = $user->uid;
  }

  if (!$timestamp) {
    $timestamp = time();
  }

  return db_query("INSERT INTO {pinboard_activity} VALUES(%d, %d, %d, %d)", $bid, $uid, $action, $timestamp);
}

/**
 * Returns user bookmarking activity for a given timespan
 *
 * Useful for syncing bookmarks or just showing some history
 *
 * @param $uid
 *    The user id to get the activity for
 * @param $since
 *    The start timestamp
 *
 * @return array
 *    An array with
 */
function pinboard_get_activity($uid, $timestamp) {
  $results = db_query("SELECT bid, action, timestamp FROM {pinboard_activity} "
    . "WHERE uid = %d AND timestamp >= %d ORDER BY timestamp ASC", $uid, $timestamp);

  while ($r = db_fetch_array($results)) {
    $activity[] = $r;
  }

  if (count($activity)) {
    return $activity;
  }

  return;
}

/**
 * Prepare a bookmark object.
 * Fetch data if necessary
 *
 * @return object
 *
 * @todo expose a hook to do the url lookup
 */
function pinboard_bookmark_prepare($url, $title = '', $description = '') {
  global $user;

  $bookmark = new StdClass;
  $bookmark->uid    = $user->uid;
  $bookmark->url    = $url;
  $bookmark->title  = $title;
  $bookmark->description = $description;

  if (empty($bookmark->title) || empty($description)) {
    $data = pinboard_url_lookup($bookmark->url);

    if (isset($data['title']) && !empty($data['title'])) {
      $bookmark->title = $data['title'];
    }
    else if (empty($bookmark->title)) {
      $bookmark->title = $bookmark->url;
    }

    if (empty($bookmark->description) && !empty($data['description'])) {
      $bookmark->description = $data['description'];
    }
  }

  $bookmark->title = html_entity_decode($bookmark->title, ENT_QUOTES, 'UTF-8');
  $bookmark->description = html_entity_decode($bookmark->description, ENT_QUOTES, 'UTF-8');

  return $bookmark;
}

/**
 * Get private token to prevent CSRF
 *
 * @see flag_get_token()
 */
function pinboard_get_token($content_id) {
  return ($GLOBALS['users']->uid) ? drupal_get_token($content_id) : md5(drupal_get_private_key() . $content_id);
}

/**
 * Check provided token against content id
 */
function pinboard_check_token($token, $content_id) {
  return $token == pinboard_get_token($content_id);
}