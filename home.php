<?php

$servername = "mysql.hostinger.com";
$username = "u132998352_admin";
$password = "123456";
$dbname = "u132998352_track";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$sql = "SELECT Latitude, Longtitude FROM tracking";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
    // output data of each row
    while($row = $result->fetch_assoc()) {
        echo "Latitude: " . $row["Latitude"]. " - Longtitude: " . $row["Longtitude"]. " <br>";
    }
} else {
    echo "No location";
}
$conn->close();
?> 