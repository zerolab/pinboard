<?php

/**
 * @file pinboard-bookmark.tpl.php
 *
 * Bookmark template
 *
 * Available variables
 *
 * @var $title
 *    Bookmark title
 * @var $url
 *    Bookmark url
 * @var $link
 *    HTML link - l($title, $url);
 * @var $description
 *    The bookmark description.
 * @var $host
 *    The bookmark host (e.g. http://example.com)
 * @var $node Object
 *    A Drupal node object available if the bookmark is an internal node
 *
 * @var $classes
 *    various helper classes
 */
?><div class="<?php print $classes?>">
  <h2><?php print $link ?></h2>
  <p class="meta"><?php print $host ?></p>
  <?php if ($description) : ?>
  <p><?php print $description ?></p>
  <?php endif; ?>
</div>