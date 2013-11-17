<?php

	$check = new Check();
	isset($_GET['pid']) && $check->GET_GP('pid');
	isset($_GET['go']) && $check->GET_GP('go');

	if($check->go == 'submit') {

		$check->GET_GP('pid', 'P');
		$check->isPidExsit();
		$check->GET_GP('language', 'P');
		$check->GET_GP('code', 'P');
		$submit = new DbSubmit();
		$submit->gs_submit($check->pid, $check->language, $check->code);
	}
	$check->isPidExsit();
	$pid = $check->pid;
	$game_title = $check->getGameName($pid);
	if($check->go == 'submit' && $submit->msg != "") {
		
		$msg  		= $submit->msg;
		$wrongcode 	= $submit->code;
	}

?>