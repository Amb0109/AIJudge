<?php
	
	require_once("../view/viewConf.php");
	
	if(isset($_GET['_t']))
		$type = trim($_GET['_t']);
	else
		$type = "code";

	$view = new viewCode($_GET['runid']);

	if($type == "ce") {
		$view->get_ce();
		echo '<html><head>';
		echo '<meta http-equiv="Content-Type" content="text/html;charset=utf-8">';
		echo '</head><body>';
		echo '<div style="width:828px;height:452px;word-wrap:break-word;">';
		echo '<pre style="font-family:Courier New;text-align:left;font-size:12px;">';
		echo $view->ce;
		echo '</pre></div>';
		echo '</body></html>';
	} else {
		$view->get_info();
		echo '<html><head>';
		echo '<script src="/resources/gs.js"></script>';
		echo '<meta http-equiv="Content-Type" content="text/html;charset=utf-8">';
		echo '</head><body>';
		echo '<textarea id=usercode style="display:none;text-align:left;">';
		echo htmlspecialchars($view->code);
		echo '</textarea><div style="padding:10px;word-wrap:break-word;"><pre style="font-family:Courier New;text-align:left;font-size:12px;">';
		if($view->lang != "Java")
			echo "<script language='javascript'>document.write(cpprenderCode(document.getElementById('usercode').value));</script></pre></div>";
		else
			echo "<script language='javascript'>document.write(javarenderCode(document.getElementById('usercode').value));</script></pre></div>";
		echo '</body></html>';
	}
	
?>