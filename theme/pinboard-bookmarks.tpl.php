<?php

/**
 * @file pinboard-bookmarks.tpl.php
 *
 * Bookmarks list template
 */

if ($is_owner) : ?>
  <?php if (!$bookmarks || empty($bookmarks)) : ?>
    <p id="pinboard-empty">You have no bookmarked items. Why not use the <?php print $bookmarklet ?> to get you started?</p>
  <?php else : ?>
  <div id="pinboard-help">
      <?php print $bookmarklet; ?>
  </div>
  <?php endif; ?>
<?php endif; ?>
  <ul class="<?php print $classes ?>">
  <?php foreach ($bookmarks as $bookmark) : ?>
    <li><?php print $bookmark ?></li>
  <?php endforeach; ?>
</ul>
  <?php if ($pager) : ?>
    <?php print $pager; ?>
  <?php endif; ?>