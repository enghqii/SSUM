<?php
	// 클라이언트는 http request 를 통해 서버에게 정보를 준다.
	require 'dbconfig.php';
	//require 'logger.php';

	//CLog::init('test.txt');
	//CLog::write(array('begin'),"test.txt");

	// mysql connection
	$conn = mysqli_connect($mysql_host, $mysql_user, $mysql_password);
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
			//CLog::write(array('LOGIN'),"test.txt");
			
			// values from post setting
			$id = $_POST['id'];
			$password = $_POST['password'];
			
			// query
			$query = "SELECT * FROM member WHERE id = '{$id}';";
			$res = mysqli_query($conn,$query);

			// check valid
			$success = false;
			$name = "";
			while($row = mysqli_fetch_array($res)){
				//print_r($row);
				if($row['password'] == md5($password)){
					$success = true;
					$name = $row['name'];
					break;
				}
			}

			if($success){
				$response = array("tag" => "LOGIN","name" => $name, "success" => 1);
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

		}else if($tag == 'GET_FRIEND_LIST'){

			//echo("why!");	
			//CLog::write(array('GETFRIENDLIST'),"test.txt");

			$id = $_POST['id'];
			$query = "SELECT name FROM member WHERE id != '{$id}';";
			$res = mysqli_query($conn,$query);

			$friend_list = array();
			$i = 0;

			while($row = mysqli_fetch_array($res)){
				//echo("i is {$i} and {$row['name']}");
				$friend_list[$i] = $row['name'];
				$i++;
			}

			$response = array("tag" => "GET_FRIEND_LIST", "numberOfFriends" => $i, "friends"=> $friend_list);
			echo json_encode($response);

		}else if($tag == 'SEND_MSG'){
			//CLog::write(array('send msg'),"test.txt");
			$sender = $_POST['sender'];
			$receiver = $_POST['receiver'];
			$message = $_POST['message'];
			$is_binary = false;

			$query = "INSERT INTO messages (sender, receiver, message, is_binary) VALUES ('{$sender}', '{$receiver}', '{$message}', '{$is_binary}');";

			$res = mysqli_query($conn,$query);

			if($res == true){
				$response = array("tag" => "SEND_MSG", "success" => 1);
			}else{
				$response = array("tag" => "SEND_MSG", "success" => 0, "error_msg" => "query failed");
			}
			echo json_encode($response);

		}else if($tag == 'UPDATE_MSG'){
			//CLog::write(array('update msg'),"test.txt");

			$sender = $_POST['sender'];
			$receiver = $_POST['receiver'];
			$lastTime = $_POST['lastTime'];

			$query = "SELECT * FROM messages WHERE time > '{$lastTime}'";
			$res = mysqli_query($conn,$query);

			$msg_list = array();
			$i = 0;

			while($row = mysqli_fetch_array($res)){
				$msg_list[$i] = $row;
				$i++;
			}
			$response = array("tag" => "UPDATE_MSG", "numberOfTalk" => $i, "talk"=> $msg_list);
			echo json_encode($response);

		}else{
			//CLog::write(array('super else'),"test.txt");

			// 태그값은 있는데 규약이랑 다름
			$response = array("tag" => $tag, "success" => 0, "error_msg" => "I can't understand what the tag means.");
			echo json_encode($response);
		}

	}else{
		//CLog::write(array('Invalid tag value.'),"test.txt");
		echo ("Invalid tag value.");
	}

	mysqli_close($conn);
?>