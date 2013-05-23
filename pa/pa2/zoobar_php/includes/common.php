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



global $php_self;
global $form_token;
global $secret_token; /* CSRF token */

$php_self = $_SERVER['PHP_SELF'];
$form_token = $_POST['token'];
  
// Check for logout and maybe display login page
if ($_POST['action'] == 'logout') {
  $user_cookie = $_COOKIE[$user->cookieName]; 
  $secret_token = md5 ($user_cookie);

  if ($form_token && $user_cookie && $form_token == $secret_token){
    $user->_logout();
    display_login();
    exit();
  }
}

// Validate user and maybe display login page
if(!validate_user($user)) {
  display_login();
  exit();
}

global $cookieData;

$user_cookie = (!$_COOKIE[$user->cookieName] && $cookieData) ? $cookieData : $_COOKIE[$user->cookieName];
$secret_token = md5 ($user_cookie);



/* 
 * This function takes a whitelist approach to filtering characters in 
 * usernames. It will take a username and only allow alphanumeric + {_ , -}
 * characters.
 */
function sanatize_username ($username) {
  if ( $username )
    return ereg_replace("[^A-Za-z0-9_-]", "", $username);
  else
    return "";
}

/*
 * This function takes a profile and sanatizes it so that it can be safely stored
 * in the DB. First it will strip all HTML tags (this is done to avoid stored XSS)
 * and then it HTML encodes it to avoid SQLi. It is important to HTML encode the
 * backslashes since if our profile ends in '\' then the end quote surrounding the
 * profile value in the SQL statement would be escaped and bad things could happen. 
 * i.e. if the SQL statement is UPDATE Person SET Profile='$profile' WHERE .... and 
 * $person ends in a '\' then the ending quote surrounding $profile would be escaped. 
 */ 
function sanatize_profile ($profile) {
	/* dont allow any html markup. all markup needs to be done via our custom markup language */
	$profile = strip_tags($profile); 

	/* HTML encode the profile. This makes sure quotes are escaped, so that it won't mess with the SQL statements. */
	$profile = htmlspecialchars($profile, ENT_QUOTES, "UTF-8");
	/* Also HTML encode backslashes to cover the case where the string ends with a backslash and it messes with SQL statements. */
	$profile = preg_replace("{\\\}", "&#92;", $profile);

	return $profile;
}

/* 
 * This function is called on a profile retrieved from the DB. Since 
 * sanatize_profile gets rid of all the nasty stored XSS vulnerabilities all
 * we need to do is HTML decode the profile and then parse the custom markup 
 * and convert to HTML markup.
 */
function prepare_profile_for_output ($profile) {
	/* decode the HTML encoding done in sanatize_profile */
	$profile = htmlspecialchars_decode($profile, ENT_QUOTES);

	/* parse our own markup and 'generate back' HTML from markup */
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
