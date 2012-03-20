<!DOCTYPE html>
<html>
<head>
  <link type="text/css" rel="stylesheet" href="<?php print $pinboard_css ?>" media="all" />
</head>
<body>
  <div id="page">
  <?php print $messages; ?>
  <?php print $content; ?>
  </div>
<script>
  var s = document.getElementById("edit-submit");

  if (s) {
    if (s.attachEvent) {
      s.attachEvent("onclick", "pinboard_submit");
    }
    else {
      s.setAttribute("onclick", "pinboard_submit();return false;");
    }
  }

function pinboard_submit() {
  var x;
  if (window.ActiveXObject) {
    try {
      x = new ActiveXObject("Msxml2.XMLHTTP");
    }
    catch(e) {
      try {
        x = new ActiveXObject("Microsoft.XMLHTTP")
      }
      catch(e) {}
    }
  }
  else if (window.XMLHttpRequest) {
    x = new XMLHttpRequest();
  }

  if (x) {
    var data = "js=1&u=" + encodeURIComponent(document.getElementById("edit-url").value) + "&t=" + encodeURIComponent(document.getElementById("edit-title").value) + "&d=" + encodeURIComponent(document.getElementById("edit-description").value);
    x.open("POST", document.getElementById("pinboard-form").action, true);
    x.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");

    x.onreadystatechange = function() {
      if (x.readyState === 4) {
        if (x.status === 200) {
          var response = JSON.parse(x.responseText);

          if (response.status === "success") {
            window.location = response.message;
          }
          else {
            var e = document.getElementById("messages");
            e.innerHTML = response.message;
            e.setAttribute("class", "error");
          }
        }
      }
    }

    x.send(data);
  }

  return false;
}
</script>
</body>
</html>