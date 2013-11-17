<?php

	if($check->mid != 0) {
		$game_mid = $check->mid;
		$check->getJsonstr();
		$jsonstr = $check->jsonstr;
		if($jsonstr != "") {
			$jsonstr = json_decode($jsonstr, true);
			isset($jsonstr['chess_manual']) && $chess_manual 	= $jsonstr['chess_manual'];
			isset($jsonstr['white']) && $white_chess			= $jsonstr['white'];
			isset($jsonstr['black']) && $black_chess			= $jsonstr['black'];
			isset($jsonstr['game_steps']) && $game_step 		= $jsonstr['game_steps'];
			isset($jsonstr['judge_result']) ? $judge_result		= $jsonstr['judge_result'] : $judge_result = 0;
			$check->getUsername();
		} else {
			$judge_result 	= 0;
			$white_chess		= 0;
			$black_chess		= 0;
		}

		if($judge_result == -1) {
			$judge_error= "JUDGE INTERNAL ERROR"; 
			$game_white = $check->username2;
			$game_black = $check->username1;
			$white_chess = 0;
			$black_chess = 0;
		} else if($judge_result == 0) {

			if($white_chess > $black_chess) {
				$game_white = sprintf($check->WINNER, $check->username2);
				$game_black = $check->username1;
			} elseif($black_chess > $white_chess) {
				$game_white = $check->username2;
				$game_black = sprintf($check->WINNER, $check->username1);
			} else {
				$game_white = $check->username2;
				$game_black = $check->username1;
			}
		} else if($judge_result >= 1 && $judge_result <= 4) {
			$game_white = sprintf($check->WINNER, $check->username2);
			$game_black = $check->username1;
		} else {
			$game_white = $check->username2;
			$game_black = sprintf($check->WINNER, $check->username1);
		}

		$num = 0;
		$str = "";
		if(isset($chess_manual) && count($chess_manual) > 0) {
			foreach ($chess_manual as $value) {
				if($num)
					$str .= ",";
				$str .= $value;
				$num ++;
			}
		}
	} else {
		$judge_result = 0;
		$game_white = "";
		$game_black = "";
	}

?>