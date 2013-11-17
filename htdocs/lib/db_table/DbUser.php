<?php

	/**
	 *用户类
	 */

	class DbUser extends DbConfig {

		public function __construct($username, $password) {

			$this->loger = new Log($this->LOG_FILENAME);
			$this->username = $username;
			$this->password = $password;
			$this->user_query();
		}

		public function user_query() {

			if( $this->username == "" || $this->password == "" ) {
				$this->msg = "用户名或密码不能为空";
			} else {
				$sql = "SELECT userPass, userId, nickName FROM $this->TB_WEBUSER WHERE userName = '$this->username';";
				$row = $this->get_one_nocache($sql);

				if($row['userPass'] == $this->password) {
					$_SESSION[$this->SESSION_USERNAME] 	= $this->username;
					$_SESSION[$this->SESSION_USERID] 	= $row['userId'];
					$_SESSION[$this->SESSION_NICKNAME] 	= $row['nickName'];
					$_SESSION[$this->SESSION_REDIRECT]  = "/";

					header("location:" . $_SESSION[$this->SESSION_REDIRECT]);
					exit();
				} else {
					$this->msg = "用户名或密码错误";
				}
			}
		}

		public function isUserExsit($user) {

			$sql = "SELECT COUNT(*) FROM $this->TB_WEBUSER WHERE userName = '$user';";
			if($row = $this->get_one_nocache($sql)) {
				if($row[0] == 0) {
					return false;
				} else {
					return true;
				}
			} else {
				$this->loger->logDAOError(__FILE__, __CLASS__, __METHOD__, $this->error());
			}
		}

		public function user_register($username, $password, $email, $nickname) {

			$username = addslashes($username);
			$password = md5($password);
			$email 	  = addslashes($email);
			$nickname = addslashes($nickname);
			$time	  = time();

			$sql = "INSERT INTO $this->TB_WEBUSER (userName, userPass, nickName, userMail, regDate) VALUES('$username', '$password', '$nickname', '$email', $time);";
			if($this->query($sql)) {
				header("location:/");
				exit();
			} else {
				$this->loger->logDAOError(__FILE__, __CLASS__, __METHOD__, $this->error());
			}
		}

		public $username 	= "";
		public $password 	= "";
		public $msg			= "";

	}

?>