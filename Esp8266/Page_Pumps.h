const char PAGE_pumps[] PROGMEM = R"=====(
<div id="content">
        <article>
          <h1>Upravljanje črpalk</h1>
          <style>
            input[type=radio]{position:absolute;visibility:hidden;display:none}.radio-group,label{display:inline-block}label{color:#FFF;cursor:pointer;font-weight:700;padding:5px 20px}.option-one:checked+label{background-color:#5cb85c}.option-two:checked+label{background-color:#ff551d}.option-three:checked+label{background-color:#d9534f}label+input[type=radio]+label{border-left:solid 4px #999}.radio-group{border:4px solid #999;border-radius:10px;overflow:hidden}
          </style>
            
            <form class="inline-block">
              <div class="text underline">Pumpa 1</div>
              <div class="radio-group">
                <input type="radio" class="option-one" id="pump1-on" name="selector" onclick="check1()" /><label class="text" for="pump1-on" >On</label><input type="radio" class="option-two" id="pump1-auto" name="selector" onclick="check1()" /><label class="text" for="pump1-auto">Auto</label><input type="radio" class="option-three" id="pump1-off" name="selector" onclick="check1()" /><label class="text" for="pump1-off">Off</label>
              </div>
            </form>
            <form class="inline-block">
              <div class="text underline">Pumpa 2</div>
              <div class="radio-group">
                <input type="radio" class="option-one" id="pump2-on" name="selector" onclick="check2()" /><label class="text" for="pump2-on">On</label><input type="radio" class="option-two" id="pump2-auto" name="selector" onclick="check2()" /><label class="text" for="pump2-auto">Auto</label><input type="radio" class="option-three" id="pump2-off" name="selector" onclick="check2()" /><label class="text" for="pump2-off">Off</label>
              </div>
            </form>
            <form class="inline-block">
              <div class="text underline">Pumpa 3</div>
              <div class="radio-group">
                <input type="radio" class="option-one" id="pump3-on" name="selector" onclick="check3()" /><label class="text" for="pump3-on">On</label><input type="radio" class="option-two" id="pump3-auto" name="selector" onclick="check3()" /><label class="text" for="pump3-auto">Auto</label><input type="radio" class="option-three" id="pump3-off" name="selector" onclick="check3()" /><label class="text" for="pump3-off">Off</label>
              </div>
            </form>
            <form class="inline-block">
              <div class="text underline">Pumpa 4</div>
              <div class="radio-group">
                <input type="radio" class="option-one" id="pump4-on" name="selector" onclick="check4()" /><label class="text" for="pump4-on">On</label><input type="radio" class="option-two" id="pump4-auto" name="selector" onclick="check4()" /><label class="text" for="pump4-auto">Auto</label><input type="radio" class="option-three" id="pump4-off" name="selector" onclick="check4()" /><label class="text" for="pump4-off">Off</label>
              </div>
            </form>
            <script>
              function check1() {
                if(document.getElementById('pump1-on').checked) window.location.replace('?cmd=Pump(1, On);');
                else if(document.getElementById('pump1-auto').checked) window.location.replace('?cmd=Pump(1, Auto);');
                else if(document.getElementById('pump1-off').checked) window.location.replace('?cmd=Pump(1, Off);');
              }
              function check2() {
                if(document.getElementById('pump2-on').checked) window.location.replace('?cmd=Pump(2, On);');
                else if(document.getElementById('pump2-auto').checked) window.location.replace('?cmd=Pump(2, Auto);');
                else if(document.getElementById('pump2-off').checked) window.location.replace('?cmd=Pump(2, Off);');
              }
              function check3() {
                if(document.getElementById('pump3-on').checked) window.location.replace('?cmd=Pump(3, On);');
                else if(document.getElementById('pump3-auto').checked) window.location.replace('?cmd=Pump(3, Auto);');
                else if(document.getElementById('pump3-off').checked) window.location.replace('?cmd=Pump(3, Off);');
              }
              function check4() {
                if(document.getElementById('pump4-on').checked) window.location.replace('?cmd=Pump(4, On);');
                else if(document.getElementById('pump4-auto').checked) window.location.replace('?cmd=Pump(4, Auto);');
                else if(document.getElementById('pump4-off').checked) window.location.replace('?cmd=Pump(4, Off);');
              }
            </script>
            
        </article>
      </div>
      <script>
        function GetStatusData()
        {
          setValues("/pumps/data");
        }

        window.onload = function ()
        {
          load("microajax.js","js", function() 
          {
              GetStatusData();
          });
        }
        function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}
      </script>
)=====";

void send_system_pumps_data()
{
  unsigned long lastRequest = millis();
  requestPumps();
  while(!Serial.available()){
    if(millis() - lastRequest >= noDataRecivedInterval){
      requestPumps();
      lastRequest = millis();
    }
  }
  serialHandler();
	String values = "";
	values += "pump1status|" + (String)ardData.pump1Status + "|span\n";
	values += "pump2status|" + (String)ardData.pump2Status + "|span\n";
	values += "pump3status|" + (String)ardData.pump3Status + "|span\n";
	values += "pump4status|" + (String)ardData.pump4Status + "|span\n";
	server.send(200, "text/plain", values);
}

void handlePumps() {
	if (!is_authentified()) {
		String header = "HTTP/1.1 301 OK\r\nLocation: /\r\nCache-Control: no-cache\r\n\r\n";
		server.sendContent(header);
		return;
	}
	if (server.hasArg("cmd")) {
    String cmd = server.arg("cmd");
		Serial.print(cmd + ";");
    int pump = cmd.substring(cmd.indexOf('(')+1, cmd.indexOf(',')).toInt();
    String status = cmd.substring(cmd.indexOf(',')+2, cmd.indexOf(')'));
    if(status == "Auto") status = "A";
    switch (pump){
      case 1:
        ardData.pump1Status = status;
        if (status == "On") ardData.pump1operating = true;
        break;
      case 2:
        ardData.pump2Status = status;
        break;
      case 3:
        ardData.pump3Status = status;
        break;
      case 4:
        ardData.pump4Status = status;
        break;
       
    }
    serialHandler();
		server.sendHeader("Location", String("/pumps"), true);
		server.send(302, "text/plain", "");
	}
  String content = String(PAGE_head) + String(PAGE_menu_logedin)+ String(PAGE_pumps)+ String(PAGE_foot);
	server.sendContent(content);
}
