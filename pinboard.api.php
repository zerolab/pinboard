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