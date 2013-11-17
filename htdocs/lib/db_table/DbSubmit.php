<?php

	/**
	 * 提交代码
	 */

	class DbSubmit Extends Dbconfig {

		public function __construct() {
			$this->loger = new Log($this->LOG_FILENAME);
		}

		public function gs_submit($pid, $language, $code) {

			$length = strlen($code);
			$this->Islogin($pid);
			$this->get_members($pid, $language, $code);
			if(!$this->msg) {

				$user_id = $_SESSION[$this->SESSION_USERID];
				$sql = "CALL $this->PROCEDURE_SUBMIT ($user_id, $this->pid," . time() . ", $this->language, $length, '$this->code');";
				if($this->query($sql)) {
					header("location:/view/games/status.php?pid=". $pid);
					exit();
				} else {
					$this->loger->logDAOError(__FILE__, __CLASS__, __METHOD__, $this->error());
				}
			} else {
				return;
			}
		}

		public function get_members($pid, $language, $code) {

			$this->pid = $pid;
			if($language >= 0 && $language <= 1)
				$this->language = $language;
			else
				$this->language = 0;
			if(strlen($code) <= 50 || strlen($code) >= 65536) {

				$this->code = $this->HTMLize(stripcslashes($code));
				$this->msg 	= "Code length is improper ! Make sure your code length is longer than 50 and not exceed 65536 Bytes.";
			}
			else
				$this->code = addslashes(addslashes($code));
		}

		public function Islogin($pid) {

			if(!isset($_SESSION[$this->SESSION_USERNAME])) {
				$_SESSION[$this->SESSION_REDIRECT] = "/view/games/submit.php?pid=" . $pid;
				header("location:./login.php");
				exit();
			}
		}

		public function HTMLize($string) {

			$string = str_replace('&', '&amp;', $string);
			$string = str_replace('"', '&quot;', $string);
			$string = str_replace("'", '&apos;', $string);
			$string = str_replace('<', '&lt;', $string);
			$string = str_replace('>', '&gt;', $string);
			$string = str_replace(' ', '&nbsp;', $string);
			$string = str_replace("\n", '<br>', $string);
			return $string;
		}

		public $language 	= 0;
		public $pid			= 0;
		public $code		= "";
		public $msg			= "";
	}

?>