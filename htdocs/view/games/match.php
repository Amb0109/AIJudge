<?php
	
	require_once("../viewConf.php");
	require_once(TEMPLATE_PATH . "header.php");
	require_once(CONTROLS_PATH . "games/match.php");

	$gs->restart(XTPL_PATH . "/games/match.xtpl");
	$gs->assign('game_title', $game_title);
	$gs->assign('game_id', $pid);
	foreach ($matchlist->clist as $values) {
		
		foreach ($values as $key => $value) {

			$gs->assign('match_info', $value);
			$gs->parse('match.match_row.match_col');
		}
		$gs->parse('match.match_row');
	}

	$page_link = $matchlist->get_page_link();
	$gs->assign('page_link',$page_link);
	$gs->parse('match');
	$gs->out('match');

	require_once(TEMPLATE_PATH . "footer.php");
?>