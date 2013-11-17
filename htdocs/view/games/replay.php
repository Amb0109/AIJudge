<?php
	
	require_once("../viewConf.php");
	require_once(TEMPLATE_PATH . "header.php");
	require_once(CONTROLS_PATH . "games/replay.php");

	$gs->restart(XTPL_PATH . "/games/replay/" . $game_title . ".xtpl");
	$gs->assign('game_title', $game_title);
	$gs->assign('game_id', $pid);

	include_once ("replay/" . $game_title . ".php");

	$gs->parse('replay');
	$gs->out('replay');

	require_once(TEMPLATE_PATH . "footer.php");
?>