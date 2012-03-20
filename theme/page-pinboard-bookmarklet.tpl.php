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
  s.onClick = function() {
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
      else if (window.XMLHTTPRequest) {
        x = new XMLHTTPRequest();
      }

      if (x) {
        x.open("POST", document.getElementById("pinboard-form").action, true);
        x.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');

        x.onreadystatechange = function() {
          if (x.readyState === 4) {
            if (x.status === 200) {
              console.log(x.responseText);
            }
          }
          else {
            console.log("Something went wrong with the request");
          }
        }
      }

      return false;
    }
  }

</script>
</body>
</html>