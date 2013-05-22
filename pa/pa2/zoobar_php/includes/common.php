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
function sanatize_username($username) {
  if ( $username )
    return ereg_replace("[^A-Za-z0-9_-]", "", $username);
  else
    return "";
}


?>
