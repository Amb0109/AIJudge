<?php

	class Matchlist extends DbConfig 
	{
		public function __construct($pid, $page) {

			$this->loger = new Log($this->LOG_FILENAME);
			$this->userid = $_SESSION[$this->SESSION_USERID];
			$this->__setparam($pid, $page);
			$this->check();
			$this->fetch_res();
			$this->translate();
		}

		public function __setparam($pid, $page) {

			$this->pid  = $pid;
			$sql = "SELECT COUNT(*) FROM $this->TB_WEBSTATUS WHERE gameId = $this->pid;";
			$row = $this->get_one_nocache($sql);
			$this->allpage = $row[0] % $this->PAGE_LIMIT == 0 ? intval($row[0] / $this->PAGE_LIMIT) - 1 : intval($row[0] / $this->PAGE_LIMIT);
			$this->allpage < 0 && $this->allpage = 0;
			$this->PAGE = $page;
			$page < 0 && $this->PAGE = 0;
			$page > $this->allpage && $this->PAGE = $this->allpage;
			$this->start = $this->PAGE * $this->PAGE_LIMIT;
		}

		public function check() {

			$sql = "SELECT COUNT(*) FROM $this->TB_WEBSTATUS WHERE gameId = $this->pid AND userId = $this->userid;";
			$row = $this->get_one_nocache($sql);
			if ($row[0] > 0) {
				$sql = "SELECT C.userName, A.runId, A.submitDate FROM $this->TB_WEBSUBMITION AS A, $this->TB_WEBSTATUS AS B, $this->TB_WEBUSER AS C WHERE A.runId = B.runId AND A.fetched = 2 AND C.userId = A.userId AND A.gameId = $this->pid AND B.gameId = $this->pid ORDER BY B.runId DESC LIMIT $this->start, $this->PAGE_LIMIT;";
				if($res = $this->query($sql)) {
					while($row = $this->fetch_array($res, MYSQL_ASSOC)) {
						array_push($this->list, $row);
					}
				} else {
					$this->loger->logDAOError(__FILE__, __CLASS__, __METHOD__, $this->error());
				}
			}
		}

		public function fetch_res() {
			
			$cur_userid = 0;
			$sql = "SELECT runId FROM $this->TB_WEBSTATUS WHERE userId = $this->userid AND gameId = $this->pid;";
			if($row = $this->get_one_nocache($sql))
				$cur_userid	= $row['runId'];
			else
				$this->loger->logDAOError(__FILE__, __CLASS__, __METHOD__, $this->error());
			
			$sql = "SELECT runId FROM $this->TB_WEBSTATUS WHERE gameId = $this->pid ORDER BY runId DESC LIMIT $this->start, $this->PAGE_LIMIT;";
			$arr = array();
			if($res = $this->query($sql)) {
				while($row = $this->fetch_array($res, MYSQL_ASSOC)) {
					array_push($arr, $row['runId']);
				}
				foreach ($arr as $value) {

					$sql = "SELECT matchId FROM $this->TB_WEBMATCH WHERE gameId = $this->pid AND playerId1 = $value AND playerId2 = $cur_userid AND fetched = 2;";
					if($row = $this->get_one_nocache($sql)) {
						array_push($this->rlist, $row);
					} else {
						array_push($this->rlist, array());
					}

					$sql = "SELECT matchId FROM $this->TB_WEBMATCH WHERE gameId = $this->pid AND playerId2 = $value AND playerId1 = $cur_userid AND fetched = 2;";

					if($row = $this->get_one_nocache($sql)) {
						array_push($this->llist, $row);
					} else {
						array_push($this->llist, array());
					}
				}
			} else {
				$this->loger->logDAOError(__FILE__, __CLASS__, __METHOD__, $this->error());
			}
		}

		public function translate() {
			
			$num = 0;
			foreach ($this->list as $key => $value) {
				$value['submitDate'] 	= gmdate("Y-m-d H:i:s", $value['submitDate'] + 28800);
				
				if(isset($this->llist[$num]['matchId']))
					$value['button1']		= sprintf($this->REPLAYLINK, "replay.php?mid=" . $this->llist[$num]['matchId'] . "&pid=" . $this->pid, "查看过程");
				else
					$value['button1']		= sprintf($this->BUTTON, $value['runId'], "a", $this->pid, $value['runId'], "a", '对战(先手)');
				if(isset($this->rlist[$num]['matchId']))
					$value['button2']	    = sprintf($this->REPLAYLINK, "replay.php?mid=" . $this->rlist[$num]['matchId'] . "&pid=" . $this->pid, "查看过程");
				else
					$value['button2']	    = sprintf($this->BUTTON, $value['runId'], "b", $this->pid, $value['runId'], "b", '对战(后手)'); 
				array_push($this->clist, $value);
				$num ++;
			}
		}

		public function get_page_link() {
			
			if($this->PAGE == 0 && $this->PAGE == $this->allpage) {
				return ;
			} else if($this->PAGE > 0 && $this->PAGE == $this->allpage) {
				return sprintf($this->PREVPAGELINK, "match", $this->pid, $this->PAGE - 1);
			} else if($this->PAGE == 0 && $this->PAGE < $this->allpage) {
				return sprintf($this->NEXTPAGELINK, "match", $this->pid, $this->PAGE + 1);
			} else {	
				return sprintf($this->PREVPAGELINK . "&nbsp;&nbsp;|&nbsp;|&nbsp;&nbsp;" . $this->NEXTPAGELINK, "match",$this->pid, $this->PAGE - 1, "match", $this->pid, $this->PAGE + 1);
			}
		}

		public function Islogin($pid) {

			if(!isset($_SESSION[$this->SESSION_USERNAME])) {
				$_SESSION[$this->SESSION_REDIRECT] = "/view/games/match.php?pid=" . $pid;
				header("location:./login.php");
				exit();
			}
		}

		public $PAGE_LIMIT 	= 10;
		public $PAGE 		= 0;
		public $pid			= 0;
		public $list		= array();
		public $clist		= array();
		public $allpage		= 0;
		public $userid 		= 0;
		public $start		= 0;
		public $llist		= array();
		public $rlist		= array();
	}

?>