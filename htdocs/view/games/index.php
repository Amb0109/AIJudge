<?php
	
	require_once("../viewConf.php");
	require_once(TEMPLATE_PATH . "header.php");
	require_once(CONTROLS_PATH . "games/index.php");

	$gs->restart(XTPL_PATH . "games/index.xtpl");
	$gs->assign('game_title', $game_title);
	$gs->assign('game_home', $game_home);
	$gs->assign('game_id', $pid);
	$gs->parse('index');
	$gs->out('index');

	require_once(TEMPLATE_PATH . "footer.php");
?>