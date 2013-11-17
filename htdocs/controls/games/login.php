<?php

	$msg = "";
	if(isset($_GET['go']) && trim($_GET['go']) == "login") {
		$username = addslashes(trim($_POST['username']));
		$password = md5($_POST['password']);

		$login 	= new DbUser($username, $password);
		$msg 	= $login->msg;
	} else if(isset($_GET['go']) && trim($_GET['go']) == "logout") {
		unset($_SESSION[$SESSION_USERNAME]);
		unset($_SESSION[$SESSION_USERID]);
		unset($_SESSION[$SESSION_NICKNAME]);
		header("location:" . $_SESSION[$SESSION_REDIRECT]);
		exit();
	}

?>