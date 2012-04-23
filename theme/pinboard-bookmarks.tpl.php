<?php

/**
 * @file pinboard-bookmarks.tpl.php
 *
 * Bookmarks list template
 */

if ($is_owner) : ?>
  <?php if ($pinboard_empty) : ?>
    <p id="pinboard-empty">You have no bookmarked items. <?php print $pinboard_help ?></p>
  <?php else : ?>
  <div id="pinboard-help">
      <?php print $bookmarklet; ?>
  </div>
  <?php endif; ?>
<?php endif; ?>

<?php if (!$pinboard_empty) : ?>
  <ul class="<?php print $classes ?>">
  <?php foreach ($bookmarks as $index => $bookmark) : ?>
    <li><?php print $bookmark ?>
      <?php if ($links[$index]) : ?>
        <div class="links">
          <?php print $links[$index]; ?>
        </div>
      <?php endif; ?>
    </li>
  <?php endforeach; ?>
</ul>
  <?php if ($pager) : ?>
    <?php print $pager; ?>
  <?php endif; ?>
<?php elseif (!$is_owner) : ?>
  <p id="pinboard-empty">No bookmarked items yet.</p>
<?php endif; ?>
