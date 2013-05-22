<?php 
  require_once("includes/common.php"); 
  nav_start_outer("Transfer");
  nav_start_inner();


  // Untrusted data. Sanatize the data that is reflected or used in SQL statements
  $recipient = sanatize_username ($_POST['recipient']); // reflected, used in SQL query
  $user_cookie = $_COOKIE[$user->cookieName];
  $secret_token = md5($user_cookie); // reflected, md5 will return alphanumeric chars
  $form_token = $_POST['token']; // not reflected or stored
  $submission_status = $_POST['submission']; // not reflected or stored
  $zoobars = (int) $_POST['zoobars']; // reflected, cast will sanatize
  $php_self = $_SERVER['PHP_SELF']; // reflected TODO: sanatize?


  if($submission_status && $form_token && $form_token == $secret_token) {
    $sql = "SELECT Zoobars FROM Person WHERE PersonID=$user->id";
    $rs = $db->executeQuery($sql);
    $sender_balance = ((int)$rs->getValueByNr(0,0)) - $zoobars;
    $sql = "SELECT PersonID FROM Person WHERE Username='$recipient'";
    $rs = $db->executeQuery($sql);
    $recipient_exists = $rs->getValueByNr(0,0);
    if($zoobars > 0 && $sender_balance >= 0 && $recipient_exists) {
      $sql = "UPDATE Person SET Zoobars = $sender_balance " .
             "WHERE PersonID=$user->id";
      $db->executeQuery($sql);
      $sql = "SELECT Zoobars FROM Person WHERE Username='$recipient'";
      $rs = $db->executeQuery($sql);
      $recipient_balance = ((int)$rs->getValueByNr(0,0)) + $zoobars;
      $sql = "UPDATE Person SET Zoobars = $recipient_balance " .
             "WHERE Username='$recipient'";
      $db->executeQuery($sql);
      $result = "Sent $zoobars zoobars";
    }
    else $result = "Transfer to $recipient failed.";
  }
  else if (!$form_token || $form_token != $secret_token)
  {
    $zoobars = null; $recipient = null;
  }
?>
<p><b>Balance:</b>
<span id="myZoobars"></span> zoobars</p>
<form method="POST" name="transferform" action="<?php echo $php_self; ?>">
<p>Send <input name="zoobars" type="text" value="<?php echo $zoobars; ?>" size="5"> zoobars</p>
<p>to <input name="recipient" type="text" value="<?php echo $recipient; ?>"></p>
<input type="submit" name="submission" value="Send">
<input type="text" name="token" value="<?php echo $secret_token; ?>" style="display:none">
</form>
<span class="warning"><?php echo $result; ?></span>
<?php 
  nav_end_inner();
?>
<script type="text/javascript" src="zoobars.js.php"></script>
<?php
  nav_end_outer(); 
?>
