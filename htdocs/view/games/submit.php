<?php
	
	require_once("../viewConf.php");
	require_once(TEMPLATE_PATH . "header.php");
	require_once(CONTROLS_PATH . "games/submit.php");

	$gs->restart(XTPL_PATH . "/games/submit.xtpl");
	$gs->assign('game_title', $game_title);
	$gs->assign('game_id', $pid);
	isset($msg) && $gs->assign('submit_msg', $msg);
	isset($wrongcode) && $gs->assign('wrong_code', $wrongcode);
	$gs->parse('submit');
	$gs->out('submit');

	require_once(TEMPLATE_PATH . "footer.php");
?>