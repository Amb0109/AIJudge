<?php
	
	session_start();
	$SESSION_CHECKNO = 'checkno';
	$x_size = 74;
	$y_size = 28;

	if(!isset($_SESSION[$SESSION_CHECKNO]))
		exit();
	$nmsg =  strval($_SESSION[$SESSION_CHECKNO] );

	if(function_exists('imagecreate') && function_exists('imagecolorallocate') && function_exists('imagepng') &&
	function_exists('imagesetpixel') && function_exists('imageString') && function_exists('imagedestroy') && function_exists('imagefilledrectangle') && function_exists('imagerectangle')){
		$aimg = imagecreate($x_size,$y_size);
		$back = imagecolorallocate($aimg,255,255,255);
		$border = imagecolorallocate($aimg,221,221,221);
		imagefilledrectangle($aimg,0,0,$x_size - 1,$y_size - 1,$back);
		imagerectangle($aimg,0,0,$x_size - 1,$y_size - 1,$border);
	  
	    for($i=1; $i<=20;$i++){
			$dot = imagecolorallocate($aimg,mt_rand(50,255),mt_rand(50,255),mt_rand(50,255));
			imagesetpixel($aimg,mt_rand(2,$x_size-2), mt_rand(2,$y_size-2),$dot);
	    }  
		for($i=1; $i<=10;$i++){
			imageString($aimg,1,$i*$x_size/12+mt_rand(1,3),mt_rand(1,13),'*',imageColorAllocate($aimg,mt_rand(150,255),mt_rand(150,255),mt_rand(150,255)));
		}
	    for ($i=0; $i<strlen($nmsg); $i++){
			imageString($aimg,mt_rand(3,5),$i*$x_size/4+mt_rand(1,8),mt_rand(1,8), $nmsg[$i], imageColorAllocate($aimg,mt_rand(50,255),mt_rand(0,120),mt_rand(50,255)));
	    }

		header("Pragma:no-cache");
		header("Cache-control:no-cache");
		header("Content-type: image/png");
	    imagepng($aimg);
	    imagedestroy($aimg);
	}

?>