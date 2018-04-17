<?php

$servername = "mysql.hostinger.com";
$username = "u132998352_admin";
$password = "123456";
$dbname = "u132998352_track";

$Latitude = $_GET["t"];
$Longtitude = $_GET["h"];

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$sql = "UPDATE tracking SET Latitude=$Latitude, Longtitude=$Longtitude WHERE 1";
$result = $conn->query($sql);

$conn->close();
?> 