<?php
	header('Content-Type: application/json');
	$servername = "mysql.hostinger.com";
	$username = "u132998352_admin";
	$password = "123456";
	$dbname = "u132998352_track";

	$dbh = mysqli_connect($MyHostname, $MyUsername, $MyPassword, $My_db);
	
	$strSQL = "SELECT * FROM tracking ";

	$objQuery = mysqli_query($dbh, $strSQL);
	$resultArray = array();
	while($obResult = mysqli_fetch_assoc($objQuery))
	{
		array_push($resultArray,$obResult);
	}
	
	echo json_encode($resultArray);
?>
