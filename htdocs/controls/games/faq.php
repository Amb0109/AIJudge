<?php

	$check = new Check();
	$check->GET_GP('pid');
	$check->isPidExsit();
	$pid = $check->pid;
	$game_title = $check->getGameName($pid);
	$game_faq	= $check->getGameInfo($pid);
	$game_faq = $game_faq['gameFaq'];

	preg_match_all('/<pre>(.*?)<\/pre>/si', $game_faq, $code);
	$game_faq = @str_replace($code[1][0], $check->toHTML($code[1][0]), $game_faq);

?>