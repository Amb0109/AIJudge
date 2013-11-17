<?php
	
	require_once("../viewConf.php");
	require_once(TEMPLATE_PATH . "header.php");
	require_once(CONTROLS_PATH . "games/faq.php");

	$gs->restart(XTPL_PATH . "/games/faq.xtpl");
	$gs->assign('game_title', $game_title);
	$gs->assign('game_id', $pid);
	$gs->assign('game_faq', $game_faq);
	$gs->parse('faq');
	$gs->out('faq');

	require_once(TEMPLATE_PATH . "footer.php");
?>