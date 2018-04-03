const char PAGE_head[] PROGMEM = R"=====(
<!DOCTYPE html><html> <head><meta name="viewport" content="width=device-width, initial-scale=0.9"><meta charset="utf-8"/><link rel="icon" type="image/png" href="http://sshp.dejavu.si/img/favicon.png"><title>SSHP Web Management</title>
 <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js"></script> <link href="https://fonts.googleapis.com/css?family=Montserrat:400,300,500,600,800,900" rel="stylesheet"> <link href="https://fonts.googleapis.com/css?family=Raleway:400,300,600,800,900" rel="stylesheet" type="text/css"> <script src="microajax.js"></script> <style>input{border: none;height: 25px;padding: 5px;}#menu li a,*{color:#999;text-decoration:none}#content,#wrapper{position:relative}footer,header{left:0;background-color:#FFF}h1,h4{color:#ff551d}#content,#copyright,#menu li,.text,h1,h4{text-align:center}#settingsTable tr:first-child td:first-child{text-align:right}#menu li a,.text,h1,h4{font-family:Montserrat,sans-serif;font-style:normal;font-weight:500;letter-spacing:.085em;text-transform:uppercase}*{font-family:Montserrat,sans-serif;font-style:normal;font-weight:400;letter-spacing:.085em}body,html{height:100%;width:100%;margin:0;padding:0;background-color:#34495e}header{position:fixed;top:0;width:300px;height:100%;box-shadow:4px 0 0 0 #ccc;z-index:100}#wrapper,body{min-height:100%}#logo{margin:20px}#logo img{max-width:100%}#menu{list-style-type:none;padding:0}#menu li{width:100%;font-size:15px}#menu li a{line-height:40px;font-size:20px;padding:18px 7%}#menu .selected,#menu li:hover{border: 0;border-right: solid 4px #ff551d;transition:.3s}#menu .selected a,#menu li:hover a{color:#ff551d;transition:.3s}#content{min-height:calc(100% - 300px);margin-left:304px;padding-top:50px;padding-bottom:125px}footer{position:absolute;bottom:0;height:100px;margin-left:304px;width: calc(100% - 304px);}.progressbar,.textValue{position:relative;display:inline-block}#copyright{margin:30px auto;width:300px;color:#999;font-family:Montserrat,sans-serif;font-style:normal;font-weight:400}h1{font-size:30px}.progressbar{width:200px;height:200px;padding:30px}.textValue{margin:20px}.progressbar .progressbarText{position: absolute;left: 50%;top: 65%;padding: 0px;margin: 0px;transform: translate(-50%, -50%)}.progressbar svg{margin-top:10px}.underline{border-bottom:2px solid #ea6e23;margin-bottom:10px}.inline-block{display:inline-block;margin:20px}.button{border:none;height:40px;background-color:#ff551d;color:#FFF}@media screen and (max-width:850px){#logo,header{height:100px}header{box-shadow:0 4px 0 0 #ccc;position:absolute;}footer{width:100% !important;margin:0 auto !important;}#logo img,#menu{position:absolute;bottom: 0}header{width:100%}#content{margin:150px 0 0}#logo,#menu{display:inline-block}#logo{width:200px;margin:0}#logo img{max-height:100px;max-width:200px;top:0;bottom:0;left:10px;margin:auto 0}#menu{height:100%;margin:0;right:0}#menu li{float:left;width:auto;height:100%;padding-left:5px;padding-right:5px}#menu li a{line-height:100px;font-size:12px;padding:0;}#menu .selected,#menu li:hover{border: 0;border-bottom: 4px solid #ff551d;transition:.3s}}@media screen and (max-width:700px){#logo,#menu{display:block}header{height:150px;width:100%;text-align:center}#logo img{position:static;margin:10px auto}#logo{height:auto;margin:0 auto}#menu{height:75px}#content{margin-top:200px}}</style> <script src="https://rawgit.com/kimmobrunfeldt/progressbar.js/1.0.0/dist/progressbar.js" ></script> </head> <body> <div id="wrapper"> <header> <div id="logo"> <img src="http://sshp.dejavu.si/img/logo.png" alt="ASHP logo"/> </div>
)=====";


const char PAGE_menu_script[] PROGMEM = R"=====(
<script>var curr_url = window.location.pathname ;
      var curr_menu = $("a[href$='" + curr_url + "']");
      curr_menu.parent().addClass('selected');
</script>
)=====";

const String PAGE_foot = "<footer><div id='copyright'>&copy; Jakob Salmič 2017<br />Version: " + String(releaseVersion) + "</div></footer></div>" + PAGE_menu_script +"</body></html>";

const char PAGE_menu_logedin[] PROGMEM = R"=====(
  <ul id="menu">
    <li><a href="/">status</a></li>
    <li><a href="/pumps">pumpe</a></li>
    <li><a href="/settings">nastavitve</a></li>
    <li><a href="/graphs">grafi</a></li>
    <li><a href="/login?DISCONNECT=YES">odjava</a></li>
  </ul>
</header>

)=====";

const char PAGE_menu_normal[] PROGMEM = R"=====(
  <ul id="menu">
    <li><a href="/">status</a></li>
    <li><a href="/login">prijava</a></li>
  </ul>
</header>
)=====";
