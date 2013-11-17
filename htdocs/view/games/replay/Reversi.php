<?php

	if(isset($game_mid))
		$gs->assign('game_mid', $game_mid);
	if(isset($str))
		$gs->assign('json_str', $str);
	else
		$gs->assign('json_str', "");
	if(isset($white_chess))
		$gs->assign('white_chess', $white_chess);
	else
		$gs->assign('white_chess', "0");
	if(isset($black_chess))
		$gs->assign('black_chess', $black_chess);
	else
		$gs->assign('black_chess', "0");
	if($judge_result == -1)
		$gs->assign('judge_error', $judge_error);
	elseif($judge_result == 1)
		$gs->assign('black_judge', "INVALID OUTPUT");
	elseif($judge_result == 2)
		$gs->assign('black_judge', "TIME LIMITE EXCEEDED");
	elseif($judge_result == 3)
		$gs->assign('black_judge', "MEMORY LIMITE EXCEEDED");
	elseif($judge_result == 4)
		$gs->assign('black_judge', "RUNTIME ERROR");

	elseif($judge_result == 5)
		$gs->assign('white_judge', "INVALID OUTPUT");
	elseif($judge_result == 6)
		$gs->assign('white_judge', "TIME LIMITE EXCEEDED");
	elseif($judge_result == 7)
		$gs->assign('white_judge', "MEMORY LIMITE EXCEEDED");
	elseif($judge_result == 8)
		$gs->assign('white_judge', "RUNTIME ERROR");

	$gs->assign('game_black', $game_black);
	$gs->assign('game_white', $game_white);

?>