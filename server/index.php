<?php
	// 클라이언트는 http request 를 통해 서버에게 정보를 준다.
	require 'dbconfig.php';

	//echo "begin";

	// mysql connection
	$conn = mysqli_connect($mysql_host,$mysql_user,$mysql_password);
	if(mysqli_connect_errno($conn)){
		echo "DB connection fail " . mysqli_connect_error();
		exit;
	}else{
		//echo "DB conncted";
	}

	// post 에 tag값이 있는지 없는지
	if( isset($_POST['tag']) && $_POST['tag'] != ''){

		$tag = $_POST['tag'];

		if($tag == 'LOGIN'){

			$id = $_POST['id'];
			$password = $_POST['password'];
			
			echo ("login process here");

		}else if($tag == 'REGISTER'){

		}else if($tag == 'SEND_MSG'){

		}else{

		}

	}else{
		echo ("Invalid tag value.");
	}
?>