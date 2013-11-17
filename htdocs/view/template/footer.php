<?php

	$gs->restart(XTPL_PATH . "template/footer.xtpl");
	$current_time = time();
	$year = date('Y', $current_time);
	$gs->assign('cur_year', $year);
	$server_time = date('Y/m/d H:i:s', gmdate($current_time + 28800));
	$gs->assign('server_time', $server_time);
	if(defined('gz_enabled')) {
		$gs->assign('gzip', 'Enabled');		
	} else {
		$gs->assign('gzip', 'Disabled');
	}
	$gs->assign('jq_path', R_P. "resources/jquery-latest.js");
	$gs->assign('bootstrap_js_path', R_P. "resources/bootstrap/js/bootstrap.min.js");
	$gs->assign('gs_js_path', R_P. "resources/gs.js");
	$gs->parse('footer');
	$gs->out('footer');
?>