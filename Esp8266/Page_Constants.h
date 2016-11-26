const char PAGE_head[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>

<head>
    <title>SolarnoGretje</title>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF8">
    <link rel="stylesheet" href="http://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.6.3/css/font-awesome.min.css">
    <link href="https://fonts.googleapis.com/css?family=Aclonica|Tangerine|Neuton|Noto+Serif|Paytone+One|Oswald|Sacramento|Roboto|Open+Sans|Montserrat" rel="stylesheet" type="text/css">
	<script src="/microajax.js"></script> 
	<style>
		.clicked a,
		a {
			color: #4CAF50
		}
        
		a,
		p,
		span {
			font-family: Montserrat
		}
        
		.button,
		a,
		li a {
			font-weight: 700;
			text-decoration: none
		}
        
		#wrapper,
		body {
			min-height: 100%
		}
        
		#header {
			position: fixed;
			height: 160px
		}
        
		#menu,
		#title {
			height: 80px
		}
        
		body,
		p,
		span {
			margin: 0
		}
        
		#title {
			width: 300px;
			margin: 0 auto;
		}
        
		.clicked {
			background-color: #5F5F5F!important
		}
        
		h1,
		h2,
		h3,
		h4,
		p,
		span {
			color: #fff
		}
        
		p,
		span {
			font-size: 15px
		}
        
		a {
			font-size: 20px
		}
        
		.button,
		h1,
		h2,
		h3,
		h4 {
			font-family: Noto Serif, serif
		}
        
		body,
		html {
			height: 100%;
			width: 100%;
			margin: 0;
			padding: 0
		}
        
		body {
			background-image: url(http://www.technocrazed.com/wp-content/uploads/2015/12/Blue-Wallpaper-For-Background-1.jpg)
		}
        
		.button {
			background-color: #5F5F5F;
			border: none;
			color: #4CAF50;
			padding: 15px 32px;
			text-align: center;
			display: inline-block;
			font-size: 16px;
			margin: 4px 2px;
			cursor: pointer
		}
        
		input,
		ul {
			margin: 0
		}
        
		.button:hover {
			background-color: #fcd450;
			color: #626262
		}
        
		#footer,
		#header {
			background-color: #00b8ff;
			width: 100%
		}
        
		input {
			color: #626262;
			font-family: Montserrat;
			font-size: 15px
		}
        
		#wrapper {
			overflow: auto;
			position: relative
		}
        
		.shadow {
			box-shadow: 0 2px 4px 0 rgba(0, 0, 0, .16), 0 2px 10px 0 rgba(0, 0, 0, .12)!important
		}
        
		#menu {
			position: absolute;
			right: 10%;
			bottom: 0
		}
        
		ul {
			transform: rotate(180deg);
			list-style-type: none;
			padding: 0;
			height: 100%;
			overflow: hidden
		}
        
		li {
			float: right
		}
        
		li a {
			transform: rotate(-180deg);
			display: block;
			padding: 30px 30px 20px;
			color: #5F5F5F;
			font-family: noto-serif;
			font-size: 30px
		}
        
		li:hover {
			background-color: #5F5F5F
		}
        
		li:hover a {
			color: #4CAF50
		}
        
		#content_Wrapper {
			width: 50%;
			min-width: 600px;
			margin: 0 auto;
			padding-top: 160px;
			text-align: center;
			padding-bottom: 80px
		}
        
		#content {
			padding-top: 50px;
			padding-bottom: 100px;
			font-size: 20px;
			background-color: #009bd6;
			box-shadow: 3px 2px 100px 20px rgba(0, 0, 0, .75)
		}
        
		#footer {
			position: absolute;
			bottom: 0;
			left: 0;
			height: 100px;
			box-shadow: 0 -2px 8px 0 rgba(0, 0, 0, .16), 0 -2px 10px 0 rgba(0, 0, 0, .12)!important;
			font-family: Neuton
		}
        
		#copyright {
			padding-top: 40px;
			padding-left: 50px
		}
        
		#copyright p {
			color: #fff;
			font-family: Noto Serif, serif;
			font-size: 20px
		}
	</style>
</head>

<body>
    <div id="wrapper">
        <div id="header" class="shadow">
			<a href="/">
				<div id="title">
					<h1>Solarno Gretje</h1>
				</div>
			</a>
)=====";

const char PAGE_foot[] PROGMEM = R"=====(
		<div id="footer">
			<div id="copyright">
				<p>&copyJakob Salmič</p>
			</div>
		</div>
	</div>
</body>
</html>
)=====";

const char PAGE_menu_logedin[] PROGMEM = R"=====(
	<div id="menu">
		<ul>
			<li><a href="/">Status</a></li>
			<li><a href="/pumps">Pumpe</a></li>
			<li><a href="/settings">Nastavitve</a></li>
			<li><a href="/graphs">Grafi</a></li>
			<li><a href="/login?DISCONNECT=YES">Odjava</a></li>
		</ul>
	</div>
</div>
<div id="content_Wrapper">
    <div id="content">
)=====";

const char PAGE_menu_normal[] PROGMEM = R"=====(
	<div id="menu">
		<ul>
			<li><a href="/">Status</a></li>
			<li><a href="/login">Prijava</a></li>
		</ul>
	</div>
</div>
<div id="content_Wrapper">
    <div id="content">
)=====";