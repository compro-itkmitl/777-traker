<?php
$MyUsername = "u132998352_admin";
$MyPassword = "123456";
$MyHostname = "mysql.hostinger.com";
$My_db = "u132998352_track";

$dbh = mysqli_connect($MyHostname, $MyUsername, $MyPassword, $My_db);
$selected = mysqli_select_db($dbh, "u132998352_track");
?>