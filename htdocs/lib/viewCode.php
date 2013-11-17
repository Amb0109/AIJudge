<?php

	/**
	 * 查看代码
	 */
	class viewCode extends Dbconfig {

		public function __construct($runid) {

			$this->loger = new Log($this->LOG_FILENAME);
			$this->runid 	= intval($runid);
			$this->userid 	= $_SESSION[$this->SESSION_USERID];
			$this->author 	= $_SESSION[$this->SESSION_USERNAME];

		}

		public function get_info() {

			$this->check();
			$sql = "SELECT A.srcCode, B.language, B.gameId FROM $this->TB_WEBSOURCE AS A, $this->TB_WEBSUBMITION AS B WHERE A.runId = $this->runid AND A.runId = B.runId;";
			if($row = $this->get_one_nocache($sql)) {
				$this->code 	= $row['srcCode'];
				$this->lang 	= $this->language[$row['language']];
				$this->gameid   = $row['gameId'];
			} else {
				$this->loger->logDAOError(__FILE__, __CLASS__, __METHOD__, $this->error());
			}
		}

		public function get_ce() {
			$sql = "SELECT A.complieInfo FROM $this->TB_WEBCERROR AS A, $this->TB_WEBSUBMITION AS B WHERE A.runId = $this->runid AND A.runId = B.runId AND B.fetched = -1;";
			if($row = $this->get_one_nocache($sql)) {
				$this->ce = $row['complieInfo'];
			}
		}

		public function check() {

			$sql = "SELECT COUNT(*) FROM $this->TB_WEBSUBMITION WHERE runId = $this->runid AND userId = $this->userid;";
			if($row = $this->get_one_nocache($sql)) {

				if($row[0] == 0) {

					$sql = "SELECT MAX(run_id) FROM $this->TB_WEBSUBMITION WHERE userId = $this->userid;";
					if($row = $this->get_one_nocache($sql)) {

						$this->runid = $row[0];
					} else {
						$this->loger->logDAOError(__FILE__, __CLASS__, __METHOD__, $this->error());
					}
				}
			} else {
				$this->loger->logDAOError(__FILE__, __CLASS__, __METHOD__, $this->error());
			}
		}

		public $runid  		= 0;
		public $userid 		= 0;
		public $code 		= "";
		public $gameid  	= 0;
		public $author		= "";
		public $language	= array(0 => "G++", 1 => "GCC", 2 => "C++", 3 => "C", 4 => "Java");
		public $lang        = 0;
		public $ce 			= "";
	}

?>