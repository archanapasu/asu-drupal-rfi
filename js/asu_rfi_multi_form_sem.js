// shows the select options for college and programs when users cometo Events form via sundevildays page.

(function($) {
Drupal.behaviors.majors = {
 attach: function (context) {
    

    
        var degree = $('input[name="program_code"]').val();
        console.log(degree);
        var semUrl = "/asu_rfi/semester/" + degree;
            $("#edit-start-date").removeOption(/./);
            //$("#edit-start-date").addOption({'0':'select...'});
            $("#edit-start-date").ajaxAddOption(semUrl , null, false);
            
   
    
    }
  }
})(jQuery);