/**
 * Pinboard Bookmarklet
 *
 * @todo add keypress handler for Esc
 */
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
      u = encodeURIComponent(location.href);

  var o = document.getElementById('pinboard_overlay');
  if (o && typeof(o) !== "undefined") {
    //document.body.removeChild(o);
    pinboard_show_overlay(o);
  }
  else {
    o = document.createElement("div");
    o.id = "pinboard_overlay";
    o.setAttribute("style", "background-color:rgba(0,0,0, 0.7); position:fixed; z-index:1024; width:100%; height:100%; top:0; left:0;transition:display 0s linear 0.5s;");

    i = document.createElement("iframe");
    i.src = "http://thebrowser.com/bookmark/let?u=" + u + "&t=" + t + "&d=" + dd;
    i.setAttribute("style", "background:#fff;border:1px solid #d5d5d5;width:398px;height:298px;position:absolute;top:50%;left:50%;margin:-150px 0 0 -200px;");
    i.setAttribute("border", "0");
    i.setAttribute("allowtransparency", "true");
    i.setAttribute("scrolling", "no");
    i.setAttribute("onLoad", "frame_count++;pinboard_frame_load();");

    var x = document.createElement("a");
    x.href = "#";
    x.innerHTML  = "Close";
    x.setAttribute("onclick", "this.parentNode.style.display='none';return false;");
    x.setAttribute("style", "background:#fff;display:block;color:#333;font-size:12px;left:50%;margin:-150px 0 0 150px;position:absolute;text-align:center;text-decoration:none;top:50%;width:50px;");

    o.appendChild(i);
    o.appendChild(x);
    document.body.appendChild(o);
  }
})();

function pinboard_frame_load(event) {
  var o = document.getElementById("pinboard_overlay");

  if (o && frame_count === 2) {
    setTimeout(function(){ document.body.removeChild(o);}, 1000);
  }
}

function pinboard_show_overlay(el) {
  el.style.display = "";
}