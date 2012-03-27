<?php
/**
 * Bookmarklet JavaScript
 *
 * Dynamically generated for each user.
 */
?>

(function(d){
  var dd;
  try {
    dd = ((window.getSelection && window.getSelection()) || (d.getSelection && d.getSelection()) || (d.selection && d.selection.createRange && d.selection.createRange().text))
        .toString().replace(/(<[^>]*>)/g,'').substring(0, 219);
  }
  catch(e) { //https denies access
    dd = '';
  }

  dd = dd.length ? encodeURIComponent(dd + '\u2026') : '';
  var t = encodeURIComponent(d.title.replace(/(<[^>]*>)/g,'')),
      u = encodeURIComponent(location.href);

<?php if ($anonymous) : ?>
  window.location.href = "<?php print $url; ?>/user?destination=" + window.location
  + "&token=<?php print md5('pinboard_redirect__'. $_SERVER['HTTP_REFERER']); ?>";
<?php else : ?>
  var pid = "<?php print $id ?>",
      ts = "<?php print $timestamp; ?>",
      token = "<?php print $token; ?>",
      hash = encodeURIComponent("<?php print $hash; ?>");

  pinboard_cors(
    'POST',
    d.location.protocol + '<?php print $url; ?>/bookmark/let',
    'cors=' + hash + '&pid=' + pid + '&token=' + token + '&ts=' + ts + '&u=' + u + '&t=' + t + '&d=' + dd,
    function(data) { console.log(data); },
    function(msg) {
      console.log('error: ' + msg);
    function() {
      // @todo Add iframe
    }
  );
<?php endif; ?>
})(document);

function pinboard_cors(method, url, data, success, error) {
  try {
    if (window.XDomainRequest) {
      var x = new XDomainRequest();
      if (!x) throw(0);
      x.ontimeout = error;
      x.onprogress = function(){};
    }
    else if (window.XMLHttpRequest) {
      var x = new XMLHttpRequest();
      if (!x) throw(0);
      if (x.withCredentials === undefined) error();
    }
    else error();

    x.onerror = error;
    x.onload = function(){ success(x.responseText); };
    x.open(method, url, true);
    x.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
    x.send(data);
  }
  catch (e) {
    error();
  }
}