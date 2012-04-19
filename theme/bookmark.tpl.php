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
 */
?><div class="bookmark">
  <h2><?php print $link ?></h2>
  <p class="meta"><?php print $host ?></p>
  <?php if ($description) : ?>
  <p><?php $description ?></p>
  <?php endif; ?>
</div>