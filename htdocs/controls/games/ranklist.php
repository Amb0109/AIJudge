<?php

	$check = new Check();
	$check->GET_GP('pid');
	$check->isPidExsit();
	$pid = $check->pid;
	$game_title = $check->getGameName($pid);


	isset($_GET['page']) && $check->GET_GP('page');
	$ranklist = new Ranklist($pid, $check->toInt($check->page));
	

?>