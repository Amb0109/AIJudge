<?php

	function num_rand($lenth){

		$randval = "";
		for($i=0;$i<$lenth;$i++){
			
			$randval .= chr(65+mt_rand(0,25));
		}
		return $randval;
	}

	function AddError($msg) {

		global $register_error;
		global $isError;
		$isError = false;
		$register_error .= "<li>$msg</li>";
	}

	function IsUserName($string) {

		return preg_match("/^[a-zA-Z0-9_]{1,12}$/", $string);
	}

	function IsPassword($string) {

		return preg_match("/^[a-zA-Z0-9+=-_]{1,18}$/", $string);
	}

	function IsEmailAddr($string) {

		$string = strtolower($string);
		return preg_match("/^[_.0-9a-z-]+@([0-9a-z][0-9a-z-]+.)+[a-z]{2,3}$/", $string);
	}

	function HTMLize($string) {
		
		$string = str_replace('&', '&amp;', $string);
		$string = str_replace('"', '&quot;', $string);
		$string = str_replace('<', '&lt;', $string);
		$string = str_replace('>', '&gt;', $string);
		$string = str_replace(' ', '&nbsp;', $string);
		$string = str_replace("\n", '<br>', $string);
		return $string;
	}

?>