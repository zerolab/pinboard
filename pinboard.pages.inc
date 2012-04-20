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
 * @todo use theme function to display each bookmark
 */
function pinboard_user_page($account) {
  global $user;

  // the user may be accessing his bookmarks with the shorthand
  if (!$account) {
    $account = $user;
  }

  // must've accessed /bookmarks directly. oops
  if (!$account->uid) {
    drupal_goto('user', array('destination' => 'bookmarks'));
  }

  $name = check_plain($account->name);
  $pinboard_title = strtolower(variable_get('pinboard_title', 'bookmarks'));

  if ($user->uid == $account->uid) {
    $title = "My $pinboard_title";
  }
  else {
    $title = "$name&rsquo;".($name[strlen($name) - 1] != 's' ? 's' : '') ." $pinboard_title";
  }
  drupal_set_title($title);

  if (!pinboard_bookmarks_access($account)) {
    return 'This user has no public bookmarks.';
  }

  // load paged bookmarks
  $bookmarks = pinboard_load_bookmarks($account->uid, NULL, TRUE);
  return theme('pinboard_bookmarks', $bookmarks, $account);
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
    $bookmark = pinboard_node_bookmark($content_id);

    if ($bid = pinboard_save($bookmark)) {
      $message = t('Bookmark successfully added');
      $link = 'bookmark/delete/'. $bid;
      $text = 'Remove bookmark';
      $status = 'pinboard-bookmarked';
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
      $status = TRUE;
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

/**
 * Handler for the bookmarklet callback page
 *
 * @todo optimize/simplify the output cases
 */
function pinboard_bookmarklet_page($mode = NULL) {
  drupal_set_header("Access-Control-Allow-Origin: *");
  drupal_set_header("Access-Control-Allow-Headers: Content-Type");

  if ($_SERVER['REQUEST_METHOD'] == 'GET') {
    if (!$mode) {
      drupal_set_header("Content-Type: application/javascript");
      print theme('pinboard_bookmarklet');
    }
    else {
      print "Saved...";
    }
  }

  if ($_SERVER['REQUEST_METHOD'] == 'POST') {
    // Some defaults;
    $status = 'error';
    $message = t('Uh-oh! Could not save bookmark. Try again later');

    if (isset($_POST['cors']) && !$mode) {
      $token = check_plain($_POST['token']);
      $hash = check_plain($_POST['cors']);
      $timestamp = check_plain($_POST['ts']);
      $uid = check_plain($_POST['pid']);
    }
    else if (isset($_POST['p'])) {
      parse_str($_POST['p'], $params);
      $token = check_plain($params['token']);
      $hash = check_plain($params['cors']);
      $timestamp = check_plain($params['ts']);
      $uid = check_plain($params['pid']);

      $_POST['u'] = $params['u'];
      $_POST['t'] = $params['t'];
      $_POST['d'] = $params['d'];
    }

    if ($hash == base64_encode(hash_hmac('sha256', "pinboard__{$uid}__{$token}__{$timestamp}", "pinboard_user_{$token}", TRUE)) ) {

      global $user;
      $id = base_convert($uid, 36, 10);

      if (!$user->uid) {
        $user = user_load(array('uid' => $id));

        if (!user_access('create bookmarks', $user)) {
          $message = t('Uh-oh! You do not have enough privileges. Who would have thought?!');

          print $mode ? $message : drupal_json(array('status' => 'error', 'message' => $message));
          exit();
        }

      }
      else if ($user->uid != $id) {
        print $mode ? $message : drupal_json(array('status' => 'error', 'message' => $message));
        exit();
      }
    }
    else {
      print $mode ? $message : drupal_json(array('status' => 'error', 'message' => $message));
      exit();
    }

    $url = $_POST['u'];

    if (function_exists('idn_to_ascii')) {
      $url = idn_to_ascii($url);
    }

    if (!valid_url($url, TRUE) ) {
      print $mode ? $message : drupal_json(array('status' => $status, 'message' => $message));
      exit();
    }

    $title = check_plain(strip_tags($_POST['t']));
    $description = check_plain(strip_tags($_POST['d']));

    $bookmark = pinboard_bookmark_prepare(check_url($url), $title, $description);

    if (pinboard_save($bookmark)) {
      $status = 'success';
      $message = t('Saved!');
    }

    print $mode ? $message : drupal_json(array('status' => $status, 'message' => $message));
  }
  exit();
}

/**
 * Implementation of hook_form()
 * This is the bookmark creation form
 */
function pinboard_form() {

  $form['url'] = array(
    '#type' => 'textfield',
    '#title' => 'URL',
    '#default_value' => $url,
    '#required' => TRUE
  );

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => 'Title',
    '#default_value' => $title
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => 'Short description',
    '#default_value' => $description,
    '#description' => 'Short description. Up to 220 characters.'
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Bookmark')
  );

  return $form;
}

/**
 *  Implementation of hook_validate()
 */
function pinboard_form_validate($form, &$form_state) {
  $url = $form_state['values']['url'];

  if (function_exists('idn_to_ascii')) {
    $url = idn_to_ascii($url);
  }
  if (!valid_url($url, TRUE) ) {
    form_set_error('url', 'Please enter a full, valid URL');
  }
}

/**
 *  Handles the bookmark creation
 *
 * @todo get title/description via drupal_http_request to $url if empty
 */
function pinboard_form_submit($form, &$form_state) {
  $url = check_url($form_state['values']['url']);
  $title = check_plain($form_state['values']['title']);
  $description = check_plain($form_state['values']['description']);

  $bookmark = pinboard_bookmark_prepare($url, $title, $description);

  if (pinboard_save($bookmark)) {
    $message = 'Bookmark successfully added';
  }
  else {
    $message = 'Uh-oh! Could not save bookmark.';
  }

  drupal_set_message($message);
}
