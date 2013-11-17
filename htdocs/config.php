<?php
	
	define(GS_ROOTPATH, preg_replace("/\\\c.*?php$/", "\\", __FILE__));
	define(XTPL_PATH, GS_ROOTPATH . "xtpl/");
	define(R_P, "/");
	define(RESOURCES_PATH, GS_ROOTPATH . "resources/");
	define(INCLUDES_PATH, GS_ROOTPATH . "includes/");
	define(VIEW_PATH, GS_ROOTPATH . "view/");
	define(TEMPLATE_PATH, VIEW_PATH . "template/");
	define(LIB_PATH, GS_ROOTPATH . "lib/");
	define(DB_TABLE_PATH, LIB_PATH . "db_table/");
	define(LOG_PATH, GS_ROOTPATH . "log/");
	define(CONTROLS_PATH, GS_ROOTPATH . "controls/");

	require_once(LOG_PATH . "Log.php");

	require_once(INCLUDES_PATH . "Functions.php");

	require_once(CONTROLS_PATH . "Constants.php");

	require_once(DB_TABLE_PATH . "DbConfig.php");
	require_once(DB_TABLE_PATH . "DbProblem.php");
	require_once(DB_TABLE_PATH . "DbSubmit.php");
	require_once(DB_TABLE_PATH . "DbStatus.php");
	require_once(DB_TABLE_PATH . "DbUser.php");

	require_once(LIB_PATH . "Check.php");
	require_once(LIB_PATH . "Ranklist.php");
	require_once(LIB_PATH . "viewCode.php");
	require_once(LIB_PATH . "Matchlist.php");
	require_once(LIB_PATH . "InsertMatch.php");

?>