<?php

	/**
	 * �ύ״̬
	 */

	class DbStatus Extends Dbconfig {

		public function __construct($pid, $page) {
			
			$this->loger = new Log($this->LOG_FILENAME);
			$this->Islogin($pid);
			$this->userid = $_SESSION[$this->SESSION_USERID];
			$this->username = $_SESSION[$this->SESSION_USERNAME];
			$this->get_all_status($pid);
			$this->get_statuslist($pid, $page);
		}

		public function get_statuslist($pid, $page) {

			$this->PAGE = $page;
			$page < 0 && $this->PAGE = 0;
			$page > $this->allpage && $this->PAGE = $this->allpage;
			$this->pid  = $pid;
			$start = $this->PAGE * $this->PAGE_LIMIT;

			$sql = "SELECT runId, submitDate, codeLength, language, fetched FROM $this->TB_WEBSUBMITION WHERE gameId = $this->pid AND userId = $this->userid ORDER BY runId DESC limit $start, $this->PAGE_LIMIT;";

			if($res = $this->query($sql)) {

				while($row = $this->fetch_array($res, MYSQL_ASSOC)) {
					array_push($this->list, $row);
				}
			} else {
				$this->loger->logDAOError(__FILE__, __CLASS__, __METHOD__, $this->error());
			}
		}

		public function get_all_status($pid) {

			$sql = "SELECT COUNT(*) FROM $this->TB_WEBSUBMITION WHERE userId = $this->userid AND gameId = $pid;";
			if($row = $this->get_one_nocache($sql)) {
				$this->allpage = $row[0] % $this->PAGE_LIMIT == 0 ? intval($row[0] / $this->PAGE_LIMIT) - 1 : intval($row[0] / $this->PAGE_LIMIT);
				$this->allpage < 0 && $this->allpage = 0;
			} else {
				$this->loger->logDAOError(__FILE__, __CLASS__, __METHOD__, $this->error());
			}
		}

		public function get_page_link() {

			if($this->PAGE == 0 && $this->PAGE == $this->allpage) {
				return ;
			} else if($this->PAGE > 0 && $this->PAGE == $this->allpage) {
				return sprintf($this->PREVPAGELINK, "status", $this->pid, $this->PAGE - 1);
			} else if($this->PAGE == 0 && $this->PAGE < $this->allpage) {
				return sprintf($this->NEXTPAGELINK, "status", $this->pid, $this->PAGE + 1);
			} else {	
				return sprintf($this->PREVPAGELINK . "&nbsp;&nbsp;|&nbsp;|&nbsp;&nbsp;" . $this->NEXTPAGELINK, "status",$this->pid, $this->PAGE - 1, "status", $this->pid, $this->PAGE + 1);
			}
		}

		public function Islogin($pid) {

			if(!isset($_SESSION[$this->SESSION_USERNAME])) {
				$_SESSION[$this->SESSION_REDIRECT] = "/view/games/status.php?pid=" . $pid;
				header("location:./login.php");
				exit();
			}
		}

		public function toChange() {

			foreach ($this->list as $value) {
				$value['codeLength']	= $value['codeLength'] . "B";
				$value['submitDate'] 	= gmdate("Y-m-d H:i:s", $value['submitDate'] + 28800);
				$value['language']		= sprintf($this->CODEVIEWLINK, $value['runId'] , $this->language[$value['language']]);	
				if($value['fetched'] == -1)
					$value['fetched']	= sprintf($this->CERRORLINK, $value['runId'],$this->state[3]);
				else
					$value['fetched']	= sprintf($this->RESCOLOR, $this->color[$value['fetched']], $this->state[$value['fetched']]);
				array_push($this->clist, $value);
			}
		}

		public $PAGE_LIMIT 	= 15;
		public $PAGE 		= 0;
		public $pid			= 0;
		public $list		= array();
		public $allpage		= 0;
		public $userid 		= null;
		public $username 	= null;
		public $clist		= array();
		public $language	= array(0 => "G++", 1 => "GCC", 2 => "C++", 3 => "C", 4 => "Java");
		public $state		= array(0 => "Queuing", 1 => "Running", 2 => "Success", 3 => "Compile Error");
		public $color		= array(0 => "#3a87ad", 1 => "#3a87ad", 2 => "#468847");
	}

?>