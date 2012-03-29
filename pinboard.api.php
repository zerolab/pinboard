<?php

/**
 * @file
 * Hooks provided by the Pinboard module
 */

/**
 * @addtogroup hooks
 */

/*
 * Implementation of hook_pinboard_node_bookmark()
 *
 * Provide customized details for the bookmarked node
 *
 * @param $bookmark
 *    The default bookmark object, ready for customizations.
 *    Defaults:
 *      - $bookmark->uid = the saving user id
 *      - $bookmark->nid = node id
 *      - $bookmark->url = node url
 *      - $bookmark->title  = node title
 *      - $bookmark->description = node teaser, stripped of tags
 * @param $node
 *    The node being bookmarked
 */
function hook_pinboard_node_bookmark(&$bookmark, $node) {
  $bookmark->uid    = $user->uid;
  $bookmark->nid    = $node->nid;
  $bookmark->url    = url('node/'. $node->nid, array('absolute' => TRUE));
  $bookmark->title  = $node->title;
  $bookmark->description = strip_tags($node->teaser);
}


/**
 * Example implementation of hook_pinboard_url_description()
 *
 * Override the description for external URLs.
 * While you can change anything, it is recommended to change only title/description
 *
 * @param $data
 *    An cacheable array with the lookup information
 *    Available keys:
 *    - code      = response code
 *    - headers   = response headers
 *    - url       = the requested url
 *    - title     = fetched page title
 *    - description = fetched page description
 *
 * @return modified $data
 */
function hook_pinboard_url_lookup($data) {
  $response = drupal_http_request($url);

  // ...

  $data['title'] = $title;
  $data['description'] = $description;
}