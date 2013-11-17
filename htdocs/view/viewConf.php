<?php

	ob_start();
	require_once(preg_replace("/view.*?php$/", "", __FILE__) . '/config.php');
	require_once(INCLUDES_PATH . 'xtemplate/xtemplate.class.php');
	ob_clean();
	session_start();


	$DB_SERVER 	= 'localhost';
	$DB_USER	= 'root';
	$DB_PASS	= 'admin';
	$DB_NAME 	= 'ai_oj';

	!isset($gsdb) && $gsdb = new DbConfig($DB_SERVER, $DB_USER, $DB_PASS, $DB_NAME);
	
	$SESSION_USERNAME 	= 'username';
	$SESSION_USERID 	= 'userid';
	$SESSION_NICKNAME 	= 'nickname';
	$SESSION_REDIRECT	= 'redirect';
	$SESSION_CHECKNO	= 'checkno';

?>