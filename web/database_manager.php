<?php

class DatabaseManager extends SQLite3 {
	function __construct() {
		$this->open("users.db"); 
	}

	function init() {
		$sql = "
			CREATE TABLE USERS(
			id INTEGER PRIMARY KEY AUTOINCREMENT,
			user_id INTEGER DEFAULT 1, 
			name TEXT NOT NULL,
			win_count INTEGER DEFAULT 0,
			loss_count INTEGER DEFAULT 0
			);
			
			CREATE TABLE ENTRIES(
			id INTEGER PRIMARY KEY AUTOINCREMENT, 
			winner INTEGER DEFAULT 0,
			loser INTEGER DEFAULT 0, 
			timestamp INTEGER NOT NULL
			); 

			";

		return $this->exec($sql); 
	}

	function dumpUsers() {
		$sql = "SELECT * FROM USERS ORDER BY user_id"; 
		return $this->query($sql); 
	}

	function dumpEntries() {
		$sql = "SELECT * FROM ENTRIES"; 
		return $this->query($sql); 
	}

	function updateBoard($winner_id, $loser_id) {
		$sql = "UPDATE USERS SET win_count = win_count + 1 WHERE user_id = :wid"; 
		$stmt = $this->prepare($sql); 
		$stmt->bindValue(":wid", $winner_id, SQLITE3_INTEGER); 
		$stmt->execute(); 

		$sql = "UPDATE USERS SET loss_count = loss_count + 1 WHERE user_id = :lid"; 
		$stmt = $this->prepare($sql); 
		$stmt->bindValue(":lid", $loser_id, SQLITE3_INTEGER); 
		$stmt->execute(); 

		$timestamp = time(); 
		$sql = "INSERT INTO ENTRIES VALUES (null, :wid, :lid, $timestamp)"; 
		$stmt = $this->prepare($sql); 
		$stmt->bindValue(":wid", $winner_id, SQLITE3_INTEGER); 
		$stmt->bindValue(":lid", $loser_id, SQLITE3_INTEGER); 
		$stmt->execute(); 
	}

	function insertUser($name) {
		// Query to find the smallest available user id
		$query = "
				SELECT MIN(user_id + 1)
				FROM USERS a
				WHERE NOT EXISTS
					(
					SELECT NULL
					FROM USERS b
					WHERE b.user_id = a.user_id + 1
					)
				"; 
		$new_user_id = $this->querySingle($query); 

		$sql = "INSERT INTO USERS VALUES (null, $new_user_id, :name, 0, 0)"; 
		$stmt = $this->prepare($sql); 
		$stmt->bindValue(":name", $name, SQLITE3_TEXT); 
		$stmt->execute(); 
	}

	function insertUserWithVal($user_id, $name, $win_count, $loss_count) {
		$sql = "INSERT INTO USERS VALUES (null, :user_id, :name, :win_count, :loss_count)"; 
		$stmt = $this->prepare($sql); 
		$stmt->bindValue(":user_id", $user_id, SQLITE3_INTEGER); 
		$stmt->bindValue(":name", $name, SQLITE3_TEXT); 
		$stmt->bindValue(":win_count", $win_count, SQLITE3_INTEGER); 
		$stmt->bindValue(":loss_count", $loss_count, SQLITE3_INTEGER); 
		$stmt->execute(); 
	}

	function updateUserName($id, $new_name) {
		$sql = "UPDATE USERS SET name = '$new_name' WHERE user_id = $id"; 
		return $this->exec($sql); 
	}

	function updateUserWinCount($id, $new_win) {
		$sql = "UPDATE USERS SET win_count = '$new_win' WHERE user_id = $id"; 
		return $this->exec($sql); 
	}

	function updateUserLossCount($id, $new_loss) {
		$sql = "UPDATE USERS SET loss_count = '$new_loss' WHERE user_id = $id"; 
		return $this->exec($sql); 
	}

	function deleteUser($id) {
		$sql = "DELETE FROM USERS WHERE user_id = $id"; 
		return $this->exec($sql); 
	}
}

/*
$db = new DatabaseManager(); 
if (!$db) {
	echo $db->lastErrorMsg(); 
} else {
	echo "Opened database successfully\n"; 
}

if (!$db->init()) {
	echo $db->lastErrorMsg(); 
} else {
	echo "Table created successfully\n"; 
}

$db->insertUserWithVal(1, "Zhu", 10, 3); 
$db->insertUserWithVal(2, "Nic", 43, 16); 
$db->insertUserWithVal(3, "Zac", 38, 105); 
$db->insertUserWithVal(4, "Bennie", 68, 59); 
$db->insertUserWithVal(5, "Kent", 34, 18); 
$db->insertUserWithVal(6, "Bennie (L)", 11, 2); 

$db->close(); 
*/

?>
