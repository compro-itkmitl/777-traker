<?php

$servername = "mysql.hostinger.com";
$username = "u132998352_admin";
$password = "123456";
$dbname = "u132998352_track";

$Lat = $_GET["t"];
$Lng = $_GET["h"];

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);

//update the input value from arduino to database
$sql = "UPDATE tracking SET Latitude=$Lat, Longtitude=$Lng WHERE 1";
$result = $conn->query($sql);

$conn->close();
?> 

<!-- url be like : ..../add_data.php?t=".."&h=".." -->