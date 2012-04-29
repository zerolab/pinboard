/**
 *
 */

Drupal.behaviors.pinboard = function(context) {
  $('a.pinboard-link:not(.pinboard-ignore)', context).click(function(){
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
              var wrapper = element.parents(".pinboard-list:first");
              element.parents('.pinboard-list-item').hide().remove();

              if (wrapper && wrapper.children().length === 0) {
                if (wrapper.hasClass('pinboard-paged')) {
                  location.reload();
                }

                $("#pinboard-empty").removeClass("pinboard-hide");
                $('#pinboard-help').remove();
                wrapper.remove();
              }
            }
            else {
              element
                .attr('href', data.link)
                .html(data.text)
                .removeClass('pinboard-bookmarked')
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