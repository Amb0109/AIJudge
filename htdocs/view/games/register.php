<?php

	require_once("../viewConf.php");
	require_once(TEMPLATE_PATH . "header.php");
	require_once(CONTROLS_PATH . "games/register.php");

	$gs->restart(XTPL_PATH . "/games/register.xtpl");
	if(!$isError) {
		$gs->assign('register_class', "alert alert-error");
		$gs->assign('register_error', $register_error);
		$gs->assign('username', HTMLize($username));
		$gs->assign('nickname', HTMLize($nickname));
		$gs->assign('email', HTMLize($email));
	}
	$gs->parse('register');
	$gs->out('register');

	require_once(TEMPLATE_PATH . "footer.php");

?>