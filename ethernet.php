<?php
class rfid{
 
 public $link='';
 
 function __construct($id, $rfid, $allow, $date){
  $this->connect();
  $this->storeInDB($id, $rfid, $allow, $date);
 }
 
 function connect(){
  $this->link = mysql_connect('localhost','root','root') or die('Cannot connect to the DB');
  mysql_select_db('rfidesp') or die('Cannot select the DB');
 }

 function storeInDB($id, $rfid, $allow, $date){
  $query = "insert into rfid set id='".$id."', rfid='".$rfid."', allow='".$allow."', date='".$date."'";

  $result = mysql_query($query) or die('Errant query:  '.$query);
 }

 if($_GET['allow'] != '' and $_GET['id'] != '')
 
 {
 $rfid=new rfid($_GET['allow'],$_GET['id']);
 }
 
}
?>