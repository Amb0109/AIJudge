<?php

	$check = new Check();
	isset($_GET['pid']) && $check->GET_GP('pid');
	$check->isPidExsit();
	$pid = $check->pid;
	$game_title = $check->getGameName($pid);
	$game_home	= $check->getGameInfo($pid);
	$game_home 	= $game_home['gameInfo'];

	preg_match_all('/<pre>(.*?)<\/pre>/si', $game_home, $code);
	$game_home = @str_replace($code[1][0], $check->toHTML($code[1][0]), $game_home);
?>