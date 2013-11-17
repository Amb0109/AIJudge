<?php
	
	/**
	 * Êý¾Ý¿âÀà
	 */
	class DbConfig extends Constants {

		public function __construct($host, $user, $password, $dbname, $pconnect = 0) {

			$this->loger = new Log($this->LOG_FILENAME);
			return $this->connect($host, $user, $password, $dbname, $pconnect);
		}

		public function connect($host, $user, $password, $dbname, $pconnect) {

			if(!$pconnect)
				$conn = @mysql_connect($host, $user, $password, 1, 131072);
			else
				$conn = @mysql_pconnect($host, $user, $password);
			
			mysql_errno() && $this->halt("Connect($pconnect) to MySQL Server failed.");
			$dbname && $this->select_db($dbname);

			return $conn;
		}

		public function query($query, $type = '') {

			if($type == "QUERY_UNBUFFERED" && function_exists('mysql_unbuffered_query'))
				$res = mysql_unbuffered_query($query);
			else
				$res = mysql_query($query);
			return $res;
		}

		public function get_one_nocache($query) {

			$res = $this->query($query, "QUERY_UNBUFFERED");
			$row = mysql_fetch_array($res);

			return $row;
		}

		public function fetch_array($res, $result_type = MYSQL_BOTH) {
			
			return mysql_fetch_array($res, $result_type);
		}

		public function fetch_row($res) {
			
			return mysql_fetch_row($res);
		}

		public function affected_rows() {
			
			return mysql_affected_rows();
		}

		public function num_rows($res) {
			
			return mysql_num_rows($res);
		}

		public function free_result($res) {

			return mysql_free_result($res);
		}

		public function insert_id() {

			return mysql_insert_id();
		}

		public function error() {

			return mysql_error();
		}

		public function halt($msg) {

			$this->loger->logDAOError(__FILE__, __CLASS__, __METHOD__, $msg);
			die();
		}

		public function close() {

			return mysql_close();
		}

		public function select_db($dbname) {

			!@mysql_select_db($dbname) && $this->halt('Cannot use specified database.');
		}

		public    $loger			= null;
		protected $TB_FILE_JUDGER	= 'file_judger_exec';
		protected $TB_FILE_USER 	= 'file_user_exec';
		protected $TB_WEBGAME 		= 'web_game';
		protected $TB_WEBMATCH 		= 'web_match';
		protected $TB_WEBMATCH_D	= 'web_match_d';
		protected $TB_WEBMATCH_S	= 'web_match_s';
		protected $TB_WEBPROJECT 	= 'web_project';
		protected $TB_WEBSOURCE 	= 'web_source';
		protected $TB_WEBSTATUS 	= 'web_status';
		protected $TB_WEBCERROR		= 'web_cerror';
		protected $TB_WEBSUBMITION	= 'web_submition';
		protected $TB_WEBUSER		= 'web_user';
		protected $SESSION_USERNAME = 'username';
		protected $SESSION_USERID 	= 'userid';
		protected $SESSION_NICKNAME = 'nickname';
		protected $SESSION_REDIRECT	= 'redirect';
		protected $SESSION_CHECKNO	= 'checkno';
		protected $PROCEDURE_SUBMIT = 'gs_submit';
	}

?>