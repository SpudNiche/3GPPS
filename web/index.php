<!doctype html>
<html>
	<head>
		<title>Ping Pong Superstars</title>
		<link rel="stylesheet" type="text/css" href="style.css">
		<link rel="shortcut icon" href="icon.ico" type="image/x-icon" />
	</head>
	<body>
		<h1>Ping Pong Superstars</h1>

		<?php

		include "database_manager.php"; 
		$db = new DatabaseManager(); 

		if (!$db) {
			echo $db->lastErrorMsg(); 
		}

		?>

		<table class="container">
			<thead>
				<tr>
				<th><h1>ID</h1></th>
				<th><h1>Name</h1></th>
				<th><h1>Wins</h1></th>
				<th><h1>Losses</h1></th>
			</tr>
		</thead>

		<?php

		$ret = $db->dumpUsers(); 
		while ($row = $ret->fetchArray(SQLITE3_ASSOC)) {
				echo "<tr>"; 
				echo "<td>" . $row["user_id"] . "</td>"; 
				echo "<td>" . $row["name"] . "</td>"; 
				echo "<td>" . $row["win_count"] . "</td>"; 
				echo "<td>" . $row["loss_count"] . "</td>"; 
				echo "</tr>";
		}
		echo "</table>"; 

		$db->close(); 

		?>

		<div class="inputs">
			<div class="entry">
				<center>
				<h2>New Entry</h2>
				<form action="update.php" method="post">
					<p>Winner ID:</p>
					<input type="number" name="wid">
					<br>
					<p>Loser ID:</p>
					<input type="number" name="lid">
					<br><br>
					<input type="submit" value="Submit" class="button">
					<br><br>
				</form>
				</center>
			</div>

			<div class="player">
				<center>
				<h2>New Player</h2>
				<form action="update.php" method="post">
					<p>Name:</p>
					<input type="text" name="name">
					<br><br>
					<input type="submit" value="Submit" class="button">
					<br><br>
				</form>
				</center>
			</div>
		</div>

	<body>
</html>
