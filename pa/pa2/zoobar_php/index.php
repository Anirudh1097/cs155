<?php 
  require_once("includes/common.php"); 
  nav_start_outer("Home");
  nav_start_inner();

  // Untrusted data. 
  $php_self = $_SERVER['PHP_SELF'];
  $profile_submit = $_POST['profile_submit'];
  $profile = sanatize_profile ($_POST['profile_update']);
?>
<b>Balance:</b> 
<?php 
  $sql = "SELECT Zoobars FROM Person WHERE PersonID=$user->id";
  $rs = $db->executeQuery($sql);
  $balance = (int)($rs->getValueByNr(0,0));
  echo $balance > 0 ? $balance : 0;
?> zoobars<br/>
<b>Your profile:</b>
<form method="POST" name="profileform" action="<?php echo $php_self ?>">
<textarea name="profile_update">
<?php
  if($profile_submit) {  // Check for profile submission
    $sql = "UPDATE Person SET Profile='$profile' ".
           "WHERE PersonID=$user->id";
    $db->executeQuery($sql);  // Overwrite profile in database
  }
  $sql = "SELECT Profile FROM Person WHERE PersonID=$user->id";
  $rs = $db->executeQuery($sql);
  echo $rs->getValueByNr(0,0);  // Output the current profile
?>
</textarea><br/>
<input type="submit" name="profile_submit" value="Save"></form>
<?php 
  nav_end_inner();
  nav_end_outer(); 
?>
