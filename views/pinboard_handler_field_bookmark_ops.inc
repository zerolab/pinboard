<?php
/**
 * Base field handler to present a bookmark link
 */

class pinboard_handler_field_bookmark_ops extends views_handler_field {
  function construct() {
    parent::construct();
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['link_type'] = array('default' => '');

    return $options;
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $nid = $values->nid;

    return pinboard_create_link($nid);
  }
}