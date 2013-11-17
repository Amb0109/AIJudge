<?php

	$check = new Check();
	$check->GET_GP('pid');
	$check->isPidExsit();
	$pid = $check->pid;
	$game_title = $check->getGameName($pid);


	isset($_GET['page']) && $check->GET_GP('page');
	$statuslist = new DbStatus($pid, $check->toInt($check->page));
	$statuslist->toChange();

?>