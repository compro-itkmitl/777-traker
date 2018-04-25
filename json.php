<?php
	header('Content-Type: application/json');
	$servername = "mysql.hostinger.com";
	$username = "u132998352_admin";
	$password = "123456";
	$dbname = "u132998352_track";

	//connect to database
	$dbh = mysqli_connect($MyHostname, $MyUsername, $MyPassword, $My_db);
	
	//select the table
	$strSQL = "SELECT * FROM tracking ";
	//query database
	$objQuery = mysqli_query($dbh, $strSQL);
	$resultArray = array();

	//process to push data to array
	while($obResult = mysqli_fetch_assoc($objQuery))
	{
		array_push($resultArray,$obResult);	
	}
	
	// output the latlng array
	echo json_encode($resultArray);
?>
