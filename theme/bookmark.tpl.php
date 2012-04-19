<?php

/**
 * @file bookmark.tpl.php
 *
 * Bookmark template
 *
 * Available variables
 *
 * $title
 * $url
 * $link
 * $description
 * $host
 * $node
 *
 * $classes
 */
?><div class="<?php print $classes?>">
  <h2><?php print $link ?></h2>
  <p class="meta"><?php print $host ?></p>
  <?php if ($description) : ?>
  <p><?php print $description ?></p>
  <?php endif; ?>
</div>