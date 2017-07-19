const char PAGE_head[] PROGMEM = R"=====(
<!DOCTYPE html><html> <head> <meta charset="utf-8"/> <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js"></script> <link href="https://fonts.googleapis.com/css?family=Montserrat" rel="stylesheet"> <link href="https://fonts.googleapis.com/css?family=Raleway:400,300,600,800,900" rel="stylesheet" type="text/css"> <script src="microajax.js"></script> <style>input{border: none;height: 25px;padding: 5px;}#menu li a,*{color:#999;text-decoration:none}#content,#wrapper{position:relative}footer,header{left:0;background-color:#FFF}h1,h4{color:#ff551d}#content,#copyright,#menu li,.text,h1,h4{text-align:center}#settingsTable tr:first-child td:first-child{text-align:right}#menu li a,.text,h1,h4{font-family:Montserrat,sans-serif;font-style:normal;font-weight:400;letter-spacing:.085em;text-transform:uppercase}*{font-family:Montserrat,sans-serif;font-style:normal;font-weight:400;letter-spacing:.085em}body,html{height:100%;width:100%;margin:0;padding:0;background-color:#34495e}header{position:fixed;top:0;width:300px;height:100%;box-shadow:4px 0 0 0 #ccc;z-index:100}#wrapper,body{min-height:100%}#wrapper{overflow:auto}#logo{margin:20px}#logo img{max-width:100%}#menu{list-style-type:none;padding:0}#menu li{width:100%;font-size:15px}#menu li a{line-height:40px;font-size:20px;padding:18px 7%}#menu .selected,#menu li:hover{box-shadow:4px 0 0 0 #ff551d;transition:.3s}#menu .selected a,#menu li:hover a{color:#ff551d;transition:.3s}#content{min-height:calc(100% - 300px);margin-left:304px;margin-top:100px;padding-bottom:150px}footer{position:absolute;bottom:0;height:100px;margin-left:304px;width: calc(100% - 304px);}.progressbar,.textValue{position:relative;display:inline-block}#copyright{margin:30px auto;width:300px;color:#999;font-family:Montserrat,sans-serif;font-style:normal;font-weight:400}h1{font-size:30px}.progressbar{width:200px;height:200px;padding:50px}.textValue{margin:20px}.progressbar .progressbarText{position:absolute;top:175px;width:200px}.progressbar svg{margin-top:10px}.underline{border-bottom:2px solid #ea6e23;margin-bottom:10px}.inline-block{display:inline-block;margin:20px}.button{border:none;height:40px;background-color:#ff551d;color:#FFF}@media screen and (max-width:850px){#logo,header{height:100px}#logo img,#menu{position:absolute}header{width:100%}#content{margin:150px 0 0}#logo,#menu{display:inline-block}#logo{width:200px;margin:0}#logo img{max-height:100px;max-width:200px;top:0;bottom:0;left:10px;margin:auto 0}#menu{height:100%;margin:0;right:0}#menu li{float:left;width:auto;height:100%;padding-left:10px;padding-right:10px}#menu li a{line-height:100px;font-size:15px}#menu .selected,#menu li:hover{box-shadow:0 4px 0 0 #ff551d;transition:.3s}}@media screen and (max-width:700px){#logo,#menu{display:block}header{height:150px;width:100%;text-align:center}#logo img{position:static;margin:0 auto}#logo{height:auto;margin:0 auto}#menu{height:100px}#content{margin-top:200px}}</style> <script src="https://rawgit.com/kimmobrunfeldt/progressbar.js/1.0.0/dist/progressbar.js" ></script> </head> <body> <div id="wrapper"> <header> <div id="logo"> <img src="http://sshp.dejavu.si/img/logo.png" alt="ASHP logo"/> </div>
}
)=====";

const char PAGE_foot[] PROGMEM = R"=====(
<footer><div id="copyright">&copy; Jakob Salmič 2017 - 2018</div></footer></div>
<script>
      var curr_url = window.location.pathname ;
      var curr_menu = $("a[href$='" + curr_url + "']");
      curr_menu.parent().addClass('selected');
  </script>
  </body></html>
)=====";

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
