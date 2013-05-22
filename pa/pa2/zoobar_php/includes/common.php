<?php

// txt-db-api library: http://www.c-worker.ch/txtdbapi/index_eng.php
require_once("txt-db-api/txt-db-api.php");
require_once("login.php");
require_once("auth.php");
require_once("navigation.php");

// Allow users to use the back button without reposting data
header ("Cache-Control: private");

// Don't allow pages that include this file to be displayed in a frame, regardless of origin
header('X-Frame-Options: DENY');

// Init global variables
$db = new Database("zoobar");
$user = new User($db);

// Check for logout and maybe display login page
if($_GET['action'] == 'logout') { 
  $user->_logout();
  display_login();
  exit();
}

// Validate user and maybe display login page
if(!validate_user($user)) {
  display_login();
  exit();
}

// This function takes a whitelist approach to filtering characters in 
// usernames. It will take a username and only allow alphanumeric + {_ , -}
// characters.
function sanatize_username ($username) {
  if ( $username )
    return ereg_replace("[^A-Za-z0-9_-]", "", $username);
  else
    return "";
}

function sanatize_profile ($profile) {
	// dont allow any html markup. all markup needs to be done via our custom markup language
	$profile = strip_tags($profile); 

	$profile = htmlspecialchars($profile, ENT_QUOTES, "UTF-8");

	// HTML encode backslashes
	$profile = preg_replace("{\\\}", "&#92;", $profile);

	return $profile;
}

function prepare_profile_for_output ($profile) {
	$profile = htmlspecialchars_decode($profile, ENT_QUOTES);

	// parse our own markup and 'put back' HTML from markup
	$allowed_tags = array('br', 'b', 'h1', 'h2', 'h3', 'h4', 'i', 'li', 'ol', 'p', 'strong', 'table', 'tr', 'td', 'th', 'u', 'ul', 'em', 'span');
	foreach ($allowed_tags as $tag){
		$regex1 = "/#\*" . $tag . "\*#/i"; $regex2 = "/#\*\/" . $tag . "\*#/i"; $regex3 = "/#\*" . $tag . "\/\*#/i";
		$subs1 = "<" . $tag . ">"; $subs2 = "</" . $tag . ">"; $subs3 = "<" . $tag . "/>"; 
		$profile = preg_replace($regex1, $subs1, $profile);
		$profile = preg_replace($regex2, $subs2, $profile);
		$profile = preg_replace($regex3, $subs3, $profile);	
	}

	return $profile;
}

?>
