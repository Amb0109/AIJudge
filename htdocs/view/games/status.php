<?php
	
	require_once("../viewConf.php");
	require_once(TEMPLATE_PATH . "header.php");
	require_once(CONTROLS_PATH . "games/status.php");

	$gs->restart(XTPL_PATH . "/games/status.xtpl");
	$gs->assign('game_title', $game_title);
	$gs->assign('game_id', $pid);
	foreach ($statuslist->clist as $values) {

		foreach ($values as $key => $value) {

			$gs->assign('status_info', $value);
			$gs->parse('status.status_row.status_col');
		}
		$gs->assign('status_info', $statuslist->username);
		$gs->parse('status.status_row.status_col');
		$gs->parse('status.status_row');
	}

	$page_link = $statuslist->get_page_link();
	$gs->assign('page_link',$page_link);
	$gs->parse('status');
	$gs->out('status');

	require_once(TEMPLATE_PATH . "footer.php");
?>