<?php
	
	require_once("../viewConf.php");
	require_once(TEMPLATE_PATH . "header.php");
	require_once(CONTROLS_PATH . "games/list.php");

	$gs->restart(XTPL_PATH . "/games/list.xtpl");
	$gs->assign('game_list_title','Game List');
	foreach ($problemlist as $values) {

		foreach ($values as $key => $value) {
			if($key == 'gameName') {
				$gs->assign('problem_property', sprintf($list->PROBLEMLINK, $values['gameId'], $value));
			} else {
				$gs->assign('problem_property', $value);
			}
			$gs->parse('list.problem_row.problem_col');
		}
		$gs->parse('list.problem_row');
	}
	$gs->parse('list');
	$gs->out('list');

	require_once(TEMPLATE_PATH . "footer.php");
?>