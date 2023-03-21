<!DOCTYPE html>
<html>
<head>
	<meta charset="utf-8">
	<title>NodeMCU e mySQL</title>

	<style type="text/css">

		/* ESTILOS GERAIS */
		.container{
			width: 50%;
			margin: 0 auto;
		}

		/* ESTILOS FORMULARIO */
		.areaPesquisa {
			border-radius: 5px;
			background-color: #B5B5B5;
			padding: 10px;
		}

		input{
			padding: 10px;
			margin: 8px 0;
			border: 1px solid #000;
			border-radius: 4px;
		}

		input[type=text]{
			width: 15%;
		}

		input[type=submit]{
			width: 20%;
			background-color: #006400;
			color: #fff;
			cursor: pointer;
		}

		/* ESTILOS TABELA */
		table{
			border-collapse: collapse;
			width: 100%;
			margin-top:10px;
		}

		table th{
			background-color: #006400;
			color: #fff;
			height: 30px;	
		}

	</style>

</head>

<body>

	<div class="container">

		<div class="areaPesquisa">
			<form action="" method="post">
				<input type="text" name="data" placeholder="dia/mês/ano">
				<input type="submit" name="submit" value="Buscar">
			</form>
		</div>

<?php
  include('conexao.php');
  
  if($_SERVER['REQUEST_METHOD'] == "POST") {
    $dataPesquisa =  $_POST['data'];

    $dataArray = explode("/", $dataPesquisa);
    $dataPesquisa = $dataArray[2] . "-" . $dataArray[1] . "-" . $dataArray[0];

    $sql = "SELECT * FROM tbdados WHERE data_hora LIKE '%" . $dataPesquisa . "%'";

  } else {
    $dataAtual = date('Y-m-d');

  	$sql = "SELECT * FROM tbdados WHERE data_hora LIKE '%" . $dataAtual . "%'";
  }

  $stmt = $PDO->prepare($sql);
  $stmt->execute();

  echo "<table border=\"1\">";

	echo "<tr> <th>Sensor 1</th> <th>Sensor 2</th> <th>Sensor 3</th> <th>Data/Hora</th> </tr>";

	while ($linha = $stmt->fetch(PDO::FETCH_OBJ)) {

  $timestamp = strtotime($linha->data_hora);
  $dataTabela = date('d/m/Y H:i:s', $timestamp);

		echo "<tr>";
	  echo "<td>" . $linha->sensor1 . "</td>";
	  echo "<td>" . $linha->sensor2 . "</td>";
	  echo "<td>" . $linha->sensor3 . "</td>";
		echo "<td>" . $dataTabela . "</td>";
		echo "</tr>";
	}

  echo "</table>";
?>

	</div>

</body>
</html>