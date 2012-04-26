<?php
/**
 * Bookmarklet JavaScript
 *
 * Dynamically generated for each user.
 */
?>
var pinboard_frame_<?php print $timestamp; ?> = 0;

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
      hash = encodeURIComponent("<?php print $hash; ?>"),
      p = 'cors=' + hash + '&pid=' + pid + '&token=' + token + '&ts=' + ts + '&u=' + u + '&t=' + t + '&d=' + dd;
  pinboard_overlay("Saving...");
  pinboard_cors(
    'POST',
    d.location.protocol + '<?php print $url; ?>/bookmark/let',
    p,
    function(data) {
      response = JSON.parse(data);
      pinboard_overlay(response.message, true);
    },
    function() {
      var i = d.createElement('iframe');
      i.name = "pinboard_frame";
      i.setAttribute('id', 'pinboard_frame');
      i.setAttribute('style', 'border:0; height:1px; left:0; position: absolute; top:0; width:1px; ')
      i.setAttribute('allowtransparency', 'true');
      i.setAttribute('onload', 'pinboard_frame_<?php print $timestamp; ?>++;pinboard_frame_load();');
      d.body.appendChild(i);

      p = p.replace(/'/g, '%27');
      window.frames["pinboard_frame"].document.write('<html><body>'
        + '<form id="f" action="' + d.location.protocol + '//<?php print $url; ?>/bookmark/let/post" method="post" accept-charset="utf-8">'
        + '<input type="hidden" name="p" id="p" value="' + p + '"/>'
        + '</form>'
        + "<scr" + "ipt>document.getElementById('f').submit();"
        + "</scr"+"ipt></body></html>");
    }
  );
<?php endif; ?>
}(document));

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
      if (!x || x.withCredentials === undefined) throw(0);
    }
    else throw(0);

    x.onerror = function(){ console.log('onerror'); error(); };
    x.onload = function(){ success(x.responseText); };
    x.open(method, url, true);
    x.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
    x.send(data);
  }
  catch (e) {
    error();
  }
}

function pinboard_overlay(text, hide) {

  var d = document,
      o = d.getElementById('pinboard_overlay');

  if (!o || typeof(o) === "undefined") {
    var o = d.createElement('div');
    o.id = "pinboard_overlay";
    o.setAttribute("style", "background:#1f1f1f; color:#fff; font-size:24px!important; padding:16px 32px; position:fixed; text-align:center; top:-9999px; z-index:10240; width:100%; left:0;"
      + "-webkit-transition:all .5s ease;-moz-transition:all .5s ease;-o-transition:all .5s ease;transition:all .5s ease;");
    d.body.appendChild(o);
  }

  o.innerHTML = text;
  o.style.top = "0";

  if (hide) {
    setTimeout(function(){o.style.top = "-9999px"; }, 800);
  }
}

function pinboard_frame_load() {
  if (pinboard_frame_<?php print $timestamp; ?> == 2) {
    pinboard_overlay('Saved!', true);

    var i = document.getElementById("pinboard_frame");
    if (i) {
      i.parentNode.removeChild(i);
    }
  }
}