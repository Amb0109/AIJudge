<?php

	/**
	 * 排名列表
	 */

	class Ranklist Extends Dbconfig {

		public function __construct($pid, $page) {

			$this->loger = new Log($this->LOG_FILENAME);
			$this->get_all_rank($pid);
			$this->get_ranklist($pid, $page);
		}

		public function get_ranklist($pid, $page) {

			$this->PAGE = $page;
			$page < 0 && $this->PAGE = 0;
			$page > $this->allpage && $this->PAGE = $this->allpage;
			$this->pid  = $pid;
			$start = $this->PAGE * $this->PAGE_LIMIT;
			$sql = "SELECT B.userName, B.nickName, A.Score FROM $this->TB_WEBSTATUS AS A, $this->TB_WEBUSER AS B WHERE A.userId = B.userId AND A.gameId = $this->pid ORDER BY A.Score DESC, A.userId ASC limit $start, $this->PAGE_LIMIT;";
			$rank = $start + 1;
			if($res = $this->query($sql)) {
				while($row = $this->fetch_array($res, MYSQL_ASSOC)) {
					array_unshift($row,$rank);
					array_push($this->list, $row);
					$rank ++;
				}
			} else {
				$this->loger->logDAOError(__FILE__, __CLASS__, __METHOD__, $this->error());
			}
		}

		public function get_all_rank($pid) {

			$sql = "SELECT COUNT(*) FROM $this->TB_WEBSTATUS WHERE gameId = $pid;";
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
				return sprintf($this->PREVPAGELINK, "ranklist", $this->pid, $this->PAGE - 1);
			} else if($this->PAGE == 0 && $this->PAGE < $this->allpage) {
				return sprintf($this->NEXTPAGELINK, "ranklist", $this->pid, $this->PAGE + 1);
			} else {
				return sprintf($this->PREVPAGELINK . "||" . $this->NEXTPAGELINK, "ranklist",$this->pid, $this->PAGE - 1, "ranklist", $this->pid, $this->PAGE + 1);
			}
		}

		public $PAGE_LIMIT 	= 15;
		public $PAGE 		= 0;
		public $pid			= 0;
		public $list		= array();
		public $allpage		= 0;
	}

?>