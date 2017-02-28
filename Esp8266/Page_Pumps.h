const char PAGE_pumps[] PROGMEM = R"=====(
<h2 style="text-align:center">Upravljanje črpalk/senzorjev</h2>
<br/>
<script>
    function r1Controller() {
        if (document.getElementById("r1").checked) r1Enable();
        else r1Disable();
    }

    function r2Controller() {
        if (document.getElementById("r2").checked) r2Enable();
        else r2Disable();
    }

    function r3Controller() {
        if (document.getElementById("r3").checked) r3Enable();
        else r3Disable();
    }

    function r4Controller() {
        if (document.getElementById("r4").checked) r4Enable();
        else r4Disable();
    }

    function r1Disable() {
        document.getElementById("p1activate").disabled = true;
        document.getElementById("p1deactivate").disabled = true;
        document.getElementById("p1auto").disabled = true;
    }

    function r1Enable() {
        document.getElementById("p1activate").disabled = false;
        document.getElementById("p1deactivate").disabled = false;
        document.getElementById("p1auto").disabled = false;
    }

    function r2Disable() {
        document.getElementById("p2activate").disabled = true;
        document.getElementById("p2deactivate").disabled = true;
        document.getElementById("p2auto").disabled = true;
    }

    function r2Enable() {
        document.getElementById("p2activate").disabled = false;
        document.getElementById("p2deactivate").disabled = false;
        document.getElementById("p2auto").disabled = false;
    }

    function r3Disable() {
        document.getElementById("p3activate").disabled = true;
        document.getElementById("p3deactivate").disabled = true;
        document.getElementById("p3auto").disabled = true;
    }

    function r3Enable() {
        document.getElementById("p3activate").disabled = false;
        document.getElementById("p3deactivate").disabled = false;
        document.getElementById("p3auto").disabled = false;
    }

    function r4Disable() {
        document.getElementById("p4activate").disabled = true;
        document.getElementById("p4deactivate").disabled = true;
        document.getElementById("p4auto").disabled = true;
    }

    function r4Enable() {
        document.getElementById("p4activate").disabled = false;
        document.getElementById("p4deactivate").disabled = false;
        document.getElementById("p4auto").disabled = false;
    }
</script>
		<table style="width:50%" align="center">
			<tr>
				<td>
					<h4></h4></td>
				<td>
					<h4>črpalka/rele</h4></td>
				<td>
					<h4>Status</h4></td>
				<td>
					<h4>VKLOP</h4></td>
				<td>
					<h4>IZKLOP</h4></td>
				<td>
					<h4>AUTO</h4></td>
			</tr>
			<tr>
				<td>
					<input onclick="r1Controller()" type="checkbox" id="r1" name="rele1" checked="checked"> </td>
				<td>
					<p>Crpalka 1</p>
				</td>
				<td>
					<span id="pump1status" name="pump1status"></span>
				</td>
				<td>
					<a href="?cmd=Pump(1, On);">
						<input class="shadow button" id="p1activate" type="button" value="Vklop"> </a>
				</td>
				<td>
					<a href=" ?cmd=Pump(1, Off);">
						<input class="shadow button" id="p1deactivate" type="button" value="Izklop"> </a>
				</td>
				<td>
					<a href="?cmd=Pump(1, Auto);">
						<input class="shadow button" id="p1auto" type="button" value="Auto"> </a>
				</td>
			</tr>
			<tr>
				<td>
					<input onclick="r2Controller()" type="checkbox" id="r2" name="rele2" checked="checked"> </td>
				<td>
					<p>Crpalka 2</p>
				</td>
				<td>
					<span id="pump2status" name="pump2status"></span>
				</td>
				<td>
					<a href="?cmd=Pump(2, On);">
						<input class="shadow button" id="p2activate" type="button" value="Vklop"> </a>
				</td>
				<td>
					<a href="?cmd=Pump(2, Off);">
						<input class="shadow button" id="p2deactivate" type="button" value="Izklop"> </a>
				</td>
				<td>
					<a href="?cmd=Pump(2, Auto);">
						<input class="shadow button" id="p2auto" type="button" value="Auto"> </a>
				</td>
			</tr>
			<tr>
				<td>
					<input onclick="r3Controller()" type="checkbox" id="r3" name="rele3" checked="checked"> </td>
				<td>
					<p>Crpalka 3</p>
				</td>
				<td>
					<span id="pump3status" name="pump3status"></span>
				</td>
				<td>
					<a href="?cmd=Pump(3, On);">
						<input class="shadow button" id="p3activate" type="button" value="Vklop"> </a>
				</td>
				<td>
					<a href="?cmd=Pump(3, Off); ">
						<input class="shadow button" id="p3deactivate" type="button" value="Izklop"> </a>
				</td>
				<td>
					<a href="?cmd=Pump(3, Auto);">
						<input class="shadow button" id="p3auto" type="button" value="Auto"> </a>
				</td>
			</tr>
			<tr>
				<td>
					<input onclick="r4Controller()" type="checkbox" id="r4" name="rele1" checked="checked"> </td>
				<td>
					<p>Crpalka 4</p>
				</td>
				<td>
					<span id="pump4status" name="pump4status"></span>
				</td>
				<td>
					<a href="?cmd=Pump(4, On);">
						<input class="shadow button" id="p4activate" type="button" value="Vklop"> </a>
				</td>
				<td>
					<a href="?cmd=Pump(4, Off);">
						<input class="shadow button" id="p4deactivate" type="button" value="Izklop"> </a>
				</td>
				<td>
					<a href="?cmd=Pump(4, Auto);">
						<input class="shadow button" id="p4auto" type="button" value="Auto"> </a>
				</td>
			</tr>
		</table>
	</div>
</div>
<script>
function GetPumpsData()
{
	setValues("/pumps/data");
}

window.onload = function ()
{
	load("microajax.js","js", function() 
	{
			GetPumpsData();
	});
}
function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}
</script>
)=====";

void send_system_pumps_data()
{
	Serial.print("GetPumps();");
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

	String content = String(PAGE_head);
	content += String(PAGE_menu_logedin);
	content += String(PAGE_pumps);
	content += String(PAGE_foot);
	server.send(200, "text/html", content);
}
