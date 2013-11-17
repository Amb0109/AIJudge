<?php

	/**
	 * 获取所有游戏列表
	 */
	class DbProblem extends Dbconfig {

		public $problemlist = array();

		public function __construct() {

			$this->loger = new Log($this->LOG_FILENAME);
			$this->get_problem_list();
		}

		public function get_problem_list() {

			$sql = "SELECT gameId, gameName, tried FROM $this->TB_WEBGAME ORDER BY gameId;";
			if($res = $this->query($sql)) {
				while($row = $this->fetch_array($res, MYSQL_ASSOC)) {
					array_push($this->problemlist, $row);
				}
			} else {
				$this->loger->logDAOError(__FILE__, __CLASS__, __METHOD__, $this->error());
			}
		} 

	}

?>