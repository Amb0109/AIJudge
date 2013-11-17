<?php
	
	require_once("../viewConf.php");
	require_once(TEMPLATE_PATH . "header.php");
	require_once(CONTROLS_PATH . "games/ranklist.php");

	$gs->restart(XTPL_PATH . "/games/ranklist.xtpl");
	$gs->assign('game_title', $game_title);
	$gs->assign('game_id', $pid);
	foreach ($ranklist->list as $values) {

		foreach ($values as $key => $value) {

			$gs->assign('ranklist_info', $value);
			$gs->parse('ranklist.ranklist_row.ranklist_col');
		}
		$gs->parse('ranklist.ranklist_row');
	}

	$page_link = $ranklist->get_page_link();
	$gs->assign('page_link',$page_link);
	$gs->parse('ranklist');
	$gs->out('ranklist');

	require_once(TEMPLATE_PATH . "footer.php");
?>