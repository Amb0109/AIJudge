<?php

	/**
	 * 错误日志
	 * @author zjqilu
	 * @since 2013/3/28
	 * @version 1.0
	 */
	class Log {

		private $LogFile = "";
		protected $DAO_LOGGER_FORMATTER = "%s System Database Error;File:%s;Class:%s;Method:%s;ErrorInfo:%s;\n\n";

		public function __construct($Filename) {

			$this->LogFile = preg_replace("/log.*?php$/", "", __FILE__) . $Filename;
		}

		public function logDAOError($file, $class, $method, $info) {

			$fp = fopen($this->LogFile, "a+");
			fwrite($fp, sprintf($this->DAO_LOGGER_FORMATTER, date("Y-m-d H:i:s", time()),
			   		$file, $class, $method, $info));
			fclose($fp);
		}
	}

?>