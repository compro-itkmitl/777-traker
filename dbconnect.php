<?php
$MyUsername = "admin";
$MyPassword = "123456";
$MyHostname = "localhost";
$My_db = "u132998352_track";

$dbh = mysqli_connect($MyHostname, $MyUsername, $MyPassword, $My_db);
$selected = mysqli_select_db($dbh, "u132998352_track");
?>