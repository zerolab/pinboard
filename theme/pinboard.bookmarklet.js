var frame_count = 0;

(function() {

  var dd;
  try {
    dd = ((window.getSelection && window.getSelection()) || (document.getSelection && document.getSelection()) || (document.selection && document.selection.createRange && document.selection.createRange().text))
        .toString().replace(/(<[^>]*>)/g,'').substring(0, 219);
  }
  catch(e) { //https denies access
    dd = '';
  }

  dd = dd.length ? dd + '\u2026' : '';

  var t = document.title.replace(/(<[^>]*>)/g,''),
      u = encodeURIComponent(location.href),
      iframe_url = "http://lab/thebrowser.com/bookmark/let?" + "u=" + u + "&t=" + t + "&d=" + dd;

  var div = document.getElementById('pinboard_bookmarklet');
  if (div && typeof(div) !== "undefined") {
    document.body.removeChild(div);
  }

  div = document.createElement("div");
  div.id = "pinboard_bookmarklet";
  div.setAttribute("style", "background-color:rgba(0,0,0, 0.7); position:fixed; z-index:1024; width:100%; height:100%; top:0; left:0;transition:display 0s linear 0.5s;");
  i = document.createElement("iframe");
  i.id = "pinboard_bookmarklet_frame";
  i.src = iframe_url;
  i.setAttribute("style", "background:#fff;border:1px solid #c5c5c5;left:50%;height:348px;margin:-225px 0 0 -175px;position:absolute;width:448px;top:50%;-webkit-box-shadow:4px 4px 12px #1f1f1f;-moz-box-shadow:4px 4px 12px #1f1f1f;box-shadow:4px 4px 12px #1f1f1f;");
  i.setAttribute("scrolling", "no");
  i.setAttribute('allowtransparency', 'true');

  div.appendChild(i);
  document.body.insertBefore(div, document.body.firstChild);
})();