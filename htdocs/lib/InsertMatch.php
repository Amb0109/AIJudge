<?php

	class InsertMatch extends DbConfig 
	{
		public function __construct($runid, $type, $b, $pid) {
			
			$this->loger = new Log($this->LOG_FILENAME);
			$this->runid = $runid;
			$this->get_players($b, $pid);
			$this->insert_players($type);
		}

		public function get_players($b, $pid) {
			if($b == "b") {
				$this->player1 = $this->runid;
				$this->player2 = $_SESSION[$this->SESSION_USERID];
				$sql = "SELECT runId, gameId FROM $this->TB_WEBSTATUS WHERE userId = $this->player2 AND gameId = $pid;";
				$row = $this->get_one_nocache($sql);
				$this->player2 = $row[0];
				$this->gameid = $row[1];
			} else {
				$this->player2 = $this->runid;
				$this->player1 = $_SESSION[$this->SESSION_USERID];
				$sql = "SELECT runId, gameId FROM $this->TB_WEBSTATUS WHERE userId = $this->player1 AND gameid = $pid;";
				$row = $this->get_one_nocache($sql);
				$this->player1 = $row[0];
				$this->gameid = $row[1];
			}
		}

		public function insert_players($type) {
			if($type === "i") {
				$sql = "SELECT COUNT(*) FROM $this->TB_WEBMATCH WHERE gameId = $this->gameid AND playerId1 = $this->player1 AND playerId2 = $this->player2;";
				$row = $this->get_one_nocache($sql);
				if($row[0] == 0) {
					$sql = "INSERT INTO $this->TB_WEBMATCH (gameId, playerId1, playerId2, playerCnt) VALUES($this->gameid, $this->player1, $this->player2, 2);";
					if(!$this->query($sql)) {
						$this->loger->logDAOError(__FILE__, __CLASS__, __METHOD__, $this->error());
					}
				}
			} else {
				$sql = "SELECT fetched, matchId FROM $this->TB_WEBMATCH WHERE gameId = $this->gameid AND playerId1 = $this->player1 AND playerId2 = $this->player2;";
				if($row = $this->get_one_nocache($sql)) {
					$this->res = sprintf($this->RESCOLOR, $this->color[$row['fetched']], $this->ResArr[ $row['fetched'] ]);
					$this->matchid = $row['matchId'];
				} else {
					$this->loger->logDAOError(__FILE__, __CLASS__, __METHOD__, $this->error());
				}
			}
		}

		public $runid	= 0;
		public $player1	= 0;
		public $player2 = 0;
		public $gameid 	= 0;
		public $ResArr	= array("Queuing", "Running", "Finish"); 
		public $res 	= "";
		public $matchid = 0;
		public $color	= array("green", "green", "red");
	}

?>