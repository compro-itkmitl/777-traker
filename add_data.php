<?php
	//Connect to Mysql
	include("dbconnect.php");

	//Prepare the sql statement
	$SQL = "INSERT INTO Tracking (Latitude, Longtitude) VALUES ('".$_GET["addLatitude"]."', '".$_GET["addLongtitude"]."')";

	// Execute sql statement
	mysqli_query($dbh, $SQL);

?>