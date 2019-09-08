<!DOCTYPE html>

<html>
  	<head>
    	<title>Beego</title>
    	<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
        <link rel="stylesheet" type="text/css" href="static/css/style.css">
	</head>

  	<body>
	{{range $msg := .messages}}
		<div class="message">
    		{{ $msg }}
		</div>
	{{ end }}
	</body>
</html>
