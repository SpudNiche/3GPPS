<?php

	if ($_POST["data"] || $_POST["wid"] || $_POST["lid"] || $_POST["name"]) {

		include 'database_manager.php';

		$db = new DatabaseManager(); 
		if (!$db) {
			echo $db->lastErrorMsg(); 
			return; 
		}
		$redirect = false; 

		if ($_POST["data"]) {
			// The expected data is in the format of W0L0W1L1...WnLn
			// Where Wi and Li for 0 <= i <= n are the winner and loser ids
			$ids = str_split($_POST["data"]); 

			// Go through each win-loss pair and update the database
			for ($i = 0; $i < sizeof($ids); $i += 2) {
				$db->updateBoard($ids[$i], $ids[$i + 1]);
			}
		} 
		else if ($_POST["wid"] && $_POST["lid"]) {
			$db->updateBoard($_POST["wid"], $_POST["lid"]);
			$redirect = true; 
		}
		else if ($_POST["name"]) {
			$db->insertUser($_POST["name"]);
			$redirect = true; 
		}

		$db->close();

		if ($redirect) {
			header("location: index.php");
		}
	}

?>
