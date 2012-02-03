/**
 *
 */

Drupal.behaviors.pinboard = function(context) {
  $('a.pinboard-link:not(.pinboard-processed)', context)
    .addClass('pinboard-processed')
    .click(function(){
      var element = $(this);

      if (element.is('.pinboard-waiting')) {
        return false;
      }

      element.addClass('pinboard-waiting');

      $.ajax({
        type: 'POST',
        url: element.attr('href'),
        data: {js: true, nid: element.attr('data-id')},
        dataType: 'json',
        success: function(data) {
          console.log(data);

          if (data.status) {
            element
              .attr('href', data.link)
              .html(data.text)
              .removeClass('bookmarked', 'unbookmarked')
              .addClass(data.status);
          }
          else {
            alert(data.error);
          }
        },
        error: function(xmlhttp) {
          alert('An HTTP error ' + xmlhttp.status + ' occurred.');
        }
      });

      element.removeClass('pinboard-waiting');

      return false;
    });
};