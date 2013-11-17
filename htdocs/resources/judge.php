<?php

	require_once("../view/viewConf.php");

	$runid = intval($_GET['r_id']);
	$type = $_GET['_tp'];
	$b = $_GET['_b'];
	$check = new Check();
	$check->GET_GP('gameid');
	$check->isPidExsit();
	$pid = $check->gameid;

	$insertmatch = new InsertMatch($runid, $type, $b, $pid);
	
	if(preg_match('/Finish/', $insertmatch->res))
		echo sprintf($insertmatch->REPLAYLINK, "replay.php?mid=" . $insertmatch->matchid . "&pid=" . $insertmatch->gameid, "查看过程");

?>