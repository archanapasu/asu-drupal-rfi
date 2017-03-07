// shows the select options for college and programs when users cometo Events form via sundevildays page.

(function($) {
Drupal.behaviors.majors = {
 attach: function (context) {
   
    var ini_degree = $('#edit-program-code').val();
    if (ini_degree.length > 0) {
      semester_codes(ini_degree);
      var sem = $("#dynamic-edit-start-date").val();
       console.log(ini_degree);
       if(sem.length > 0){
        $("#edit-start-date").val(sem);
       }
    }
    $("#edit-start-date").hide();
    $("#edit-start-date").before('<select id="dynamic-edit-start-date" name="semester1"><option value="" selected="selected">- Select -</option></select>');
    
    $('#edit-program-code').change(function(){
        var degree = $(this).val();
        semester_codes(degree);
        
    });
    
    
    function semester_codes(degree){   
        var semUrl = "/asu_rfi/semester/" + degree;
            $("#dynamic-edit-start-date").removeOption(/./);
            
            $("#dynamic-edit-start-date").ajaxAddOption(semUrl , null, false);
            $("#dynamic-edit-start-date").bind("change", function(){ semChanged(); })  
    }
    
    function semChanged(){
      var dynamic_sem_data = $("#dynamic-edit-start-date").val();
      console.log(dynamic_sem_data);
      $("#edit-start-date").val(dynamic_sem_data);
      console.log($("#edit-start-date").val());
    }
    
      }
   }
})(jQuery);
    


