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
      var data = {js: true};
      if (element.attr('data-id') !== 'undefined') {
        data.nid = element.attr('data-id');
      }

      $.ajax({
        type: 'POST',
        url: element.attr('href'),
        data: data,
        dataType: 'json',
        success: function(data) {
          if (data.status) {
            if (data.status == 'remove') {
              element.parent().hide('slow').remove();
            }
            else {
              element
                .attr('href', data.link)
                .html(data.text)
                .removeClass('pinboard-bookmark')
                .addClass(data.status);
            }
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