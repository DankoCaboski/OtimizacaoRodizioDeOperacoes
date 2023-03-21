<?php
	try {
		$HOST = "localhost";
		$BANCO = "bdnodemcu";
		$USUARIO = "root";
		$SENHA = "";

		$PDO = new PDO("mysql:host=" . $HOST . ";dbname=" . $BANCO . ";charset=utf8", $USUARIO, $SENHA);

	} catch (PDOException $erro){

		//echo "Erro de conexao, detalhes: " . $erro->getMessage();	
		echo "conexao_erro";
	}
?>