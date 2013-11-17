<?php

	require_once("../viewConf.php");
	require_once(TEMPLATE_PATH . "header.php");
	require_once(CONTROLS_PATH . "games/login.php");

	$gs->restart(XTPL_PATH . "/games/login.xtpl");
	$gs->assign('error_msg', $msg);
	$gs->parse('login');
	$gs->out('login');

	require_once(TEMPLATE_PATH . "footer.php");

?>