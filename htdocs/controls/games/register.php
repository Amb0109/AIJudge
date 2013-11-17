<?php

	$msg = "";
	$checkno = "";
	isset($_SESSION[$SESSION_CHECKNO]) && $checkno = trim($_SESSION[$SESSION_CHECKNO]);
	$_SESSION[$SESSION_CHECKNO] = num_rand(4);

	$action = "";
	isset($_GET['action']) && $action = $_GET['action'];

	$register_error = "";
	$isError		= true;
	if($action == "go") {
		$username 	= $_POST['username'];
		$password 	= $_POST['password'];
		$password2 	= $_POST['password2'];
		$email	 	= $_POST['email'];
		$nickname	= $_POST['nickname'];
		$verify		= $_POST['verify'];

		if(strlen($username) > 12)
			AddError("The UserName is too long.");
		else
		if($username == "")
			AddError("The UserName can't be empty.");
		else
		if(!IsUserName($username)) 
			AddError("The UserName is INVALID. Please use letters or digits.");

		if(strlen($password) > 18)
			AddError("The PassWord is too long.");
		else
		if($password == "")
			AddError("The PassWord can't be empty.");
		else
		if($password2 == "")
			AddError("The Confirm PW can't be empty.");
		else
		if($password != $password2)
			AddError("The Confirm PW is not the same as PassWord.");
		else
		if(!IsPassword($password)) 
			AddError("The Password is INVALID.");

		if(strlen($nickname) > 12)
			AddError("The NickName is too long.");
		else
		if($nickname == "")
			AddError("The NickName can't be empty.");

		if($email == "")
			AddError("The Email can't be empty.");
		else
		if(!IsEmailAddr($email))
			AddError("The Email is INVALID.");

		if(strcasecmp($verify, $checkno) != 0)
			AddError("The Verify Code is INVALID.");

		$user = new DbUser($username, $password);
		if($user->isUserExsit(addslashes($username))) {
			AddError("The UserName has already existed.");
		} else if($isError) {
			$user->user_register($username, $password, $email, $nickname);
		}

	} else {
		$username 	= "";
		$password 	= "";
		$password2 	= "";
		$email	 	= "";
		$nickname	= "";
		$verify		= "";
	}
	
?>