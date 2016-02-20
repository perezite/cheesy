<?php

function checkIfOnline($url) 
{
	//check, if a valid url is provided
	if(!filter_var($url, FILTER_VALIDATE_URL)) {
		   return false;
	}

	//initialize curl
	$curlInit = curl_init($url);
	curl_setopt($curlInit,CURLOPT_CONNECTTIMEOUT,10);
	curl_setopt($curlInit,CURLOPT_HEADER,true);
	curl_setopt($curlInit,CURLOPT_NOBODY,true);
	curl_setopt($curlInit,CURLOPT_RETURNTRANSFER,true);

	//get answer
	$response = curl_exec($curlInit);

	curl_close($curlInit);

	if ($response) {
		return true;
	}

	return false;
}

function createErrorFile($filePath)
{
	$f = fopen($filePath, 'w');
	$res = fwrite($f, 'fail');
	fclose($f);
	return $res;
}

function errorFileExists($filePath) {
	return file_exists($filePath);
}

function sendErrorMail($url) {
	$message = "Warning: Server is unreachable, please fix:\n$url";
	$message = wordwrap($message, 70);

	mail('xxx@xxx.uzh.ch', 'Server down (' . $url . ')', $message);
}

function printErrorMessage($url) 
{
	echo "<html>";
	echo "The server at <a href='" . $url . "'>" . $url . "</a> seems to be down.<br>";
	echo "A mail has been sent to the admin and we will fix the problem as soon as possible.<br>";
	echo "We apologize for the inconvenience.<br>";
	echo "<br>";
	echo "</html>";
}

function redirectToPage($url) 
{
	header('Location: ' . $url);
	exit;
}

$errorFilePath = "error.txt";
$targetUrl = "http://simonschwegler.selfhost.eu/portfolio";

$isOnline = checkIfOnline($targetUrl);
if($isOnline == True) {
	redirectToPage($targetUrl);
} else {
	if (errorFileExists($errorFilePath) == False) {
		if (createErrorFile($errorFilePath) != False) {
			sendErrorMail($targetUrl);
		}
	}	
	printErrorMessage($targetUrl);
}

?>