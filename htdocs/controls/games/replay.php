<?php

	$check = new Check();
	$check->GET_GP('pid');
	$check->isPidExsit();
	$pid = $check->pid;
	$check->Islogin($pid);
	$game_title = $check->getGameName($pid);
	isset($_GET['mid']) && $check->GET_GP('mid');

	include_once (CONTROLS_PATH . "games/replay/" . $game_title . ".php");

?>