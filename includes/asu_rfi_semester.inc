<?php
// $Id: asu_rfi_confirmation.inc 

/**
 * @file
 * Request info form generation using form API
 */

/**** semester code from web services**/
function asu_rfi_semester()
{
 $plancode = arg(2);
 //print $plancode;
  // Make sure we have valid parameters
  if(empty($plancode) ) {
    print 'Error, no operation specified' . '<br />';
    return;
  }
  
    
   $data = _asu_rfi_semester($plancode);
   
   print json_encode($data);
}

function asu_rfi_semester_ajax_callback(){
  $plancode = arg(2);
  $xmldata = xmlrpc('https://webapp4.asu.edu/programs/XmlRpcServer', array('eAdvisorDSFind.findDegreeByAcadPlanMapArray' => array($plancode)));
  $terms = $xmldata[0]['planDeadlines'];
  // print "<pre>"; print_r($xmldata[0]); print "</pre>";
  foreach($terms as $skey=>$svalue){
    $sem_code = explode(":",$skey);
    $sem_value = explode(":",$svalue);
    $semes[$sem_code[2]] = $sem_value[0];
  }
  
  //Get semester dates from dabatase
  $semdates_query = db_query("select * from {asu_rfi_sem_dates} order by CHAR_LENGTH(semId)");
  foreach ($semdates_query as $semester) {
    $termdate[$semester->semId] = $semester->semLabel;
  }
  
  if (!empty($termdate)) {
    $sem_dates_array = array_intersect($termdate, $semes);
  } else {
    $sem_dates_array = $semes;
  }
  return $sem_dates_array;
}

function _asu_rfi_semester(){
  $plancode = arg(2);
  $xmldata = xmlrpc('https://webapp4.asu.edu/programs/XmlRpcServer', array('eAdvisorDSFind.findDegreeByAcadPlanMapArray' => array($plancode)));
  $terms = $xmldata[0]['planDeadlines'];
  
  foreach($terms as $skey=>$svalue){
    $sem_code = explode(":",$skey);
    $sem_value = explode(":",$svalue);
    $semes[$sem_code[2]] = $sem_value[0];
  }
  
  //creturn general resulst if codes are empty
  if(sizeof($semes) == 0){
    $date = array();
  $month = date('m');
  $day = date('d');
  $new = array();
  $options = array();
  $semester = array("1" => "Spring", "7" => "Fall");
  $year = date('Y');

  for ($i = 0; $i <= 2; $i++) {
    $date[$i] = $year + $i;
    $new[$i] = substr_replace($date[$i], "", 1, 1);
    if ($i == 0) {
      if ($year == $date[0]) // check if it is current year
      {
        if (($month <= 8)) {
          if (($month != 8) || ($month == 8) && ($day <= 14)) {
            $sem = $semester["7"];
            $trim_year = $new[$i] . "7";  // append 1 to the key depending on the season. For spring, add 1.
            $list = $date[$i];
            $options[$trim_year] = $list . " " . $sem;
          }

        }
        if (($month >= 8)) {
          if (($month != 8) || ($month == 8) && ($day > 14)) {
            $sem = $semester["1"];
            $trim_year = ($new[$i] + 1) . "1";  // append 1 to the key depending on the season. For spring, add 1.
            $list = $date[$i] + 1;
            $options[$trim_year] = $list . " " . $sem;


            $up_new = $new[$i] + 1;
            $new_date = $date[$i] + 1;
            $sem = $semester["7"];
            $trim_year = $up_new . "7";
            $list = $new_date;
            $options[$trim_year] = $list . " " . $sem;
          }
        }
      }
    } else {
      $sem = $semester["1"];
      $trim_year = ($new[$i]) . "1";  // append 1 to the key depending on the season. For spring, add 1.
      $list = $date[$i];
      $options[$trim_year] = $list . " " . $sem;

      $upnext_new = $new[$i];
      $nextnew_date = $date[$i];
      $next_sem = $semester["7"];
      $nexttrim_year = $upnext_new . "7";
      $next_list = $nextnew_date;
      $options[$nexttrim_year] = $next_list . " " . $next_sem;
    }
  }
  }
  
  
  //watchdog('sem1','<pre>' . print_r($semes, true) . '</pre>');
  //Get semester dates from dabatase
  $semdates_query = db_query("select * from {asu_rfi_sem_dates} order by CHAR_LENGTH(semId)");
  foreach ($semdates_query as $semester) {
    $termdate[$semester->semId] = $semester->semLabel;
  }
  //watchdog('sem2','<pre>' . print_r($termdate, true) . '</pre>');
  
  if (!empty($termdate)) {
    if(isset($semes)){
      $sem_dates_array = array_intersect($termdate, $semes);
    }
    if(isset($options)){
      $sem_dates_array = array_intersect($termdate, $options);
    }
    
  } else {
    $sem_dates_array = $semes;
  }
  
  return $sem_dates_array;
}

?>