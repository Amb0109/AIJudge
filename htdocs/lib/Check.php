<?php

	/**
	 * 检测GP是否合法
	 */

	class Check Extends Dbconfig {

		public function __construct() {
			
			$this->loger = new Log($this->LOG_FILENAME);
		}

		public function GET_GP ($G_P, $is_p = 'G') {

			if ($is_p == 'P') {
				$this->$G_P = $_POST[$G_P];
			} else {
				$this->$G_P = $_GET[$G_P];
			}
		}

		public function isPidExsit() {

			$this->pid = $this->toInt($this->pid);
			$sql = "SELECT COUNT(*) FROM $this->TB_WEBGAME WHERE gameId = $this->pid LIMIT 1;";
			if($row = $this->get_one_nocache($sql)) {
				if($row[0] == 0) {
					$this->pid = 1000;
				}
			} else {
				$this->loger->logDAOError(__FILE__, __CLASS__, __METHOD__, $this->error());
			}
		}

		public function getGameName($pid) {

			$sql = "SELECT gameName FROM $this->TB_WEBGAME WHERE gameId = $pid LIMIT 1;";
			if($row = $this->get_one_nocache($sql)) {
				return $row['gameName'];
			} else {
				$this->loger->logDAOError(__FILE__, __CLASS__, __METHOD__, $this->error());
			}
		}

		public function getGameInfo($pid) {

			$sql = "SELECT gameFaq, gameInfo FROM $this->TB_WEBGAME WHERE gameId = $pid LIMIT 1;";
			if($row = $this->get_one_nocache($sql)) {
				return $row;
			} else {
				$this->loger->logDAOError(__FILE__, __CLASS__, __METHOD__, $this->error());
			}
		}

		public function getJsonstr() {
			
			$sql1 = "SELECT A.userId FROM $this->TB_WEBSUBMITION AS A, $this->TB_WEBMATCH AS B WHERE A.runId = B.playerId1 AND B.matchId = $this->mid AND B.gameId = $this->pid;";
			$sql2 = "SELECT A.userId FROM $this->TB_WEBSUBMITION AS A, $this->TB_WEBMATCH AS B WHERE A.runId = B.playerId2 AND B.matchId = $this->mid AND B.gameId = $this->pid;";
			
			if(($row1 = $this->get_one_nocache($sql1)) && ($row2 = $this->get_one_nocache($sql2))) {
				
				array_push($this->userids, $row1['userId']);
				array_push($this->userids, $row2['userId']);
				
				$userid = $_SESSION[$this->SESSION_USERID];
				if(in_array($userid, $this->userids)) {
					$sql = "SELECT judgeRes FROM $this->TB_WEBMATCH WHERE matchId = $this->mid AND gameId = $this->pid;";
					if($row = $this->get_one_nocache($sql)) {
						$this->jsonstr = $row['judgeRes'];
					} else {
						$this->loger->logDAOError(__FILE__, __CLASS__, __METHOD__, $this->error());
					}
				}
			} else {
				$this->loger->logDAOError(__FILE__, __CLASS__, __METHOD__, $this->error());
			}
		}

		public function getUsername() {
			$user1 = $this->userids[0];
			$user2 = $this->userids[count($this->userids) - 1];
			$sql = "SELECT userName FROM $this->TB_WEBUSER WHERE userId = $user1;";
			$row = $this->get_one_nocache($sql);
			$this->username1 = $row['userName'];
			$sql = "SELECT userName FROM $this->TB_WEBUSER WHERE userId = $user2;";
			$row = $this->get_one_nocache($sql);
			$this->username2 = $row['userName'];
		}

		public function Islogin($pid) {

			if(!isset($_SESSION[$this->SESSION_USERNAME])) {
				$_SESSION[$this->SESSION_REDIRECT] = "/view/games/replay.php?pid=" . $pid;
				header("location:./login.php");
				exit();
			}
		}

		public function toHTML($string) {
			
			$string = str_replace('&', '&amp;', $string);
			$string = str_replace('"', '&quot;', $string);
			$string = str_replace('<', '&lt;', $string);
			$string = str_replace('>', '&gt;', $string);
			$string = str_replace(' ', '&nbsp;', $string);
			return $string;
		}

		public function toInt($val) {

			return intval($val);
		}

		public function adds($string) {

			return addslashes($string);
		}

		public function strips($string) {

			return stripslashes($string);
		}

		public $pid 		= 0;
		public $gameid 		= 0;
		public $go 			= "";
		public $language	= "";
		public $code 		= "";
		public $page		= 0;
		public $mid			= 0;
		public $userids     = array();
		public $jsonstr 	= "";
		public $username1	= "";
		public $username2	= "";
	}

?>