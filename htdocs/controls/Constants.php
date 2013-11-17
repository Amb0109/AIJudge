<?php
	
	/**
	 * 链接常量
	 */
	class Constants {

		public $LOG_FILENAME 	= 'gs_log.log';
		public $PROBLEMLINK 	= '<a href="/view/games/index.php?pid=%d">%s</a>';
		public $PREVPAGELINK	= '<a href="/view/games/%s.php?pid=%d&page=%d"><< Prev Page </a>';
		public $NEXTPAGELINK	= '<a href="/view/games/%s.php?pid=%d&page=%d"> Next Page >></a>';
		public $CODEVIEWLINK	= '<a href="javascript:void(0)" onclick="show(%d, 1);">%s</a>';
		public $CERRORLINK		= '<a style="color:#b94a48;" href="javascript:void(0)" onclick="show(%d, 0);">%s</a>';
		public $BUTTON			= '<button class="btn" onclick=insert(%d,\'%s\',%d) id="judge%d%s">%s</button>';
		public $REPLAYLINK		= '<button class="btn" onclick=location.href="%s">%s</button>';
		public $RESCOLOR		= '<font color="%s">%s</font>';
		public $WINNER			= '<span style="position:relative;top:8px;">%s</span> <span class="label label-success">Winner</span>';
	}

?>