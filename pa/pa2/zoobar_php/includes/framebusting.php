<?php
	// Don't allow pages that include this file to be displayed in a frame, regardless of origin
	header('X-Frame-Options: DENY');

	// TODO: is DENY neccessary or I am being to stringent? should it be SAMEORIGIN?
	// TODO: why does login.php did not include framebusting code in first place? Should I not be including framebusting code for this?
?>