<?php

class DatabaseManager extends SQLite3 {
	function __construct() {
		$this->open("users.db"); 
	}

	function init() {
		$sql = "CREATE TABLE USERS(
			id INTEGER PRIMARY KEY AUTOINCREMENT,
			name TEXT NOT NULL,
			win_count INTEGER DEFAULT 0,
			loss_count INTEGER DEFAULT 0
			);";

		return $this->exec($sql); 
	}

	function dumpUsers() {
		$sql = "SELECT * FROM USERS"; 
		return $this->query($sql); 
	}

	function updateBoard($winner_id, $loser_id) {
		$sql = "UPDATE USERS SET win_count = win_count + 1 WHERE id = :wid"; 
		$stmt = $this->prepare($sql); 
		$stmt->bindValue(":wid", $winner_id, SQLITE3_INTEGER); 
		$stmt->execute(); 

		$sql = "UPDATE USERS SET loss_count = loss_count + 1 WHERE id = :lid"; 
		$stmt = $this->prepare($sql); 
		$stmt->bindValue(":lid", $loser_id, SQLITE3_INTEGER); 
		$stmt->execute(); 
	}

	function insertUser($name) {
		$sql = "INSERT INTO USERS VALUES (null, :name, 0, 0)"; 
		$stmt = $this->prepare($sql); 
		$stmt->bindValue(":name", $name, SQLITE3_TEXT); 
		$stmt->execute(); 
	}

	function updateUserName($id, $new_name) {
		$sql = "UPDATE USERS SET name = '$new_name' WHERE id = $id"; 
		return $this->exec($sql); 
	}

	function updateUserWinCount($id, $new_win) {
		$sql = "UPDATE USERS SET win_count = '$new_win' WHERE id = $id"; 
		return $this->exec($sql); 
	}

	function updateUserLossCount($id, $new_loss) {
		$sql = "UPDATE USERS SET loss_count = '$new_loss' WHERE id = $id"; 
		return $this->exec($sql); 
	}

	function deleteUser($id) {
		$sql = "DELETE FROM USERS WHERE id = $id"; 
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

$db->close(); 
*/


?>
