<?php
	
	if(isset($_GET['pid']))
		$pid = intval($_GET['pid']);
	else
		$pid = 1000;

	if(isset($_GET['page']))
		$page = intval($_GET['page']);
	else
		$page = 0;
	$matchlist = new Matchlist($pid, $page);
	$matchlist->Islogin($pid);
	$check = new Check();
	$game_title = $check->getGameName($pid);

?>