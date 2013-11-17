<?php
	
	/*
	 *Gzip
	 */
	if(preg_match('/gzip/i', $_SERVER['HTTP_ACCEPT_ENCODING'])) {
		ob_start("ob_gzhandler");
		define("gz_enabled", true);
	} else {
		ob_start();
	}

	$gs = new XTemplate(XTPL_PATH . "template/header.xtpl");
	$gs->assign('bootstrap_css_path', R_P . "resources/bootstrap/css/bootstrap.css");
	$gs->assign('header_title', 'War of AI');
	$gs->assign('home_path', R_P);
	$gs->assign('game_list', R_P . "view/games/list.php");
	$gs->assign('gs_css_path', R_P . "resources/gs.css");
	if(isset($_SESSION['username'])) {
		$gs->assign('action', '?go=logout');
		$gs->assign('user_name', $_SESSION[$SESSION_NICKNAME]);
		$gs->assign('login_msg', '[Logout]');
	} else {
		$gs->assign('action', '');
		$gs->assign('login_msg', '[Login]');
		$gs->assign("register", "<a href='./register.php'>Register</a>");
	}
	$gs->parse('header');
	$gs->out('header');

?>