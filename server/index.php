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
		// mysql setup here
		mysqli_select_db($conn,$mysql_db);
	}

	// post 에 tag값이 있는지 없는지
	if( isset($_POST['tag']) && $_POST['tag'] != ''){

		$tag = $_POST['tag'];

		if($tag == 'LOGIN'){

			// values from post setting
			$id = $_POST['id'];
			$password = $_POST['password'];
			
			// query
			$query = "SELECT id, password FROM member WHERE id = '{$id}';";
			$res = mysqli_query($conn,$query);

			// check valid
			$success = false;
			while($row = mysqli_fetch_array($res)){
				//print_r($row);
				if($row['password'] == md5($password)){
					$success = true;
					break;
				}
			}

			if($success){
				$response = array("tag" => "LOGIN", "success" => 1);
			}else{
				$response = array("tag" => "LOGIN", "success" => 0, "error_msg" => "login failed");
			}
			echo json_encode($response);

		}else if($tag == 'REGISTER'){

			// values from post setting
			$id = $_POST['id'];
			$password = md5($_POST['password']);
			$name = $_POST['name'];

			// sql query
			$query = "INSERT INTO member (id, password, name) VALUES ('{$id}', '{$password}', '{$name}');";
			//echo $query;
			$res = mysqli_query($conn,$query);

			if($res == true){
				$response = array("tag" => "REGISTER", "success" => 1);
			}else{
				$response = array("tag" => "REGISTER", "success" => 0, "error_msg" => "query failed");
			}
			echo json_encode($response);

		}else if($tag == 'SEND_MSG'){

		}else{
			// 태그값은 있는데 정해지지 않은 것임.
		}

	}else{
		echo ("Invalid tag value.");
	}

	mysqli_close($conn);
?>