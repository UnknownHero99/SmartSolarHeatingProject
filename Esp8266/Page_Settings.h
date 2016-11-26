const char PAGE_settings[] PROGMEM = R"=====(
		<script>
			function syncTime() {
				var d = new Date();
				d.setHours(d.getHours() + 2);
				var n = d.toISOString().split(".")[0];
				document.getElementById("time").value = n;
			}
		</script>
		<h2>Nastavitve</h2>
		<br/>
		<form enctype="application/json" method="post">
			<table style="width:70%" align="center">
				<tr>
					<th>
						<h4>Nastavitev</h4></th>
					<th>
						<h4>Trenutna vrednost</h4></th>
					<th>
						<h4>Nova vrednost</h4></th>
				</tr>
				<tr>
					<td>
						<p>Min temp razlika:</p>
					</td>
					<td>
						<span id="tdiffmin" name="tdiffmin"></span><span>°C</span>
					</td>
					<td>
						<input type="text" id="tdiffmininput" name="tdiffmininput" value="0" />
					</td>
				</tr>
				<tr>
					<td>
						<p>Maksimalna temperatura kolektorja:</p>
					</td>
					<td>
						<span id="tkmax" name="tkmax"></span><span>°C</span>
					</td>
					<td>
						<input type="text" id="tkmaxinput" name="tkmaxinput" value="0" />
					</td>
				</tr>
				<tr>
					<td>
						<p>Minimalna temperatura kolektorja:</p>
					</td>
					<td>
						<span id="tkmin" name="tkmin"></span><span>°C</span>
					</td>
					<td>
						<input type="text" id="tkmininput" name="tkmininput" value="0" />
					</td>
				</tr>
				<tr>
					<td>
						<p>Maksimalna temperatura bojlerja:</p>
					</td>
					<td>
						<span id="tbmax" name="tbmax"></span><span>°C</span>
					</td>
					<td>
						<input type="text" id="tbmaxinput" name="tbmaxinput" value="0">
					</td>
				</tr>
				<tr>
					<td>
						<p>Nadmorska višina:</p>
					</td>
					<td>
						<span id="altitude" name="altitude"></span><span>m</span>
					</td>
					<td>
						<input type="text" id="altitudeinput" name="altitudeinput" value="0">
					</td>
				</tr>
				<tr>
					<td>
						<p>Datum/ura</p>
					</td>
					<td>
						<span id="time" name="time"></span>
					</td>
					<td>
						<input id="timeinput" type="text" name="date" value="0">
					</td>
				</tr>
				<tr>
					<td colspan="2">
						<button type="button" onclick="GetSettingsData()" class="shadow button">Osveži</button>
					</td>
					<td colspan="1">
						<input class="shadow button" type="submit" value="Shrani" style="float: right;">
					</td>
				</tr>
			</table>
			<br>
		</form>
	</div>
</div>
<script>
function GetSettingsData()
{
	setValues("/settings/data");
}

window.onload = function ()
{
	load("microajax.js","js", function() 
	{
			GetSettingsData();
	});
}
function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}
</script>
)=====";

void send_system_settings_data()
{
	Serial.print("GetSettings();GetData();GetPumps();");
  lastUpdate = millis();
	String values = "";
	values += "tdiffmin|" + (String)ardSettings.tdiffmin + "|span\n";
	values += "tkmax|" + (String)ardSettings.tkmax + "|span\n";
	values += "tkmin|" + (String)ardSettings.tkmin + "|span\n";
	values += "tbmax|" + (String)ardSettings.tbmax + "|span\n";
	values += "altitude|" + (String)ardSettings.altitude + "|span\n";
	server.send(200, "text/plain", values);
}

void handleSettings() {
	if (!is_authentified()) {
		String header = "HTTP/1.1 301 OK\r\nLocation: /\r\nCache-Control: no-cache\r\n\r\n";
		server.sendContent(header);
		return;
	}

	if (server.args() > 0)  // Save Settings
	{
		if (server.hasArg("tdiffmininput")) {
			ardSettings.tdiffmin = server.arg("tdiffmininput").toInt();
			ardSettings.tdiffmininput = ardSettings.tdiffmin;
		}
		if (server.hasArg("tkmaxinput")) {
			ardSettings.tkmax = server.arg("tkmaxinput").toInt();
			ardSettings.tkmaxinput = ardSettings.tkmax;
		}
		if (server.hasArg("tkmininput")) {
			ardSettings.tkmin = server.arg("tkmininput").toInt();
			ardSettings.tkmininput = ardSettings.tkmin;
		}
		if (server.hasArg("tbmaxinput")) {
			ardSettings.tbmax = server.arg("tbmaxinput").toInt();
			ardSettings.tbmaxinput = ardSettings.tbmax;
		}
		if (server.hasArg("altitudeinput")) {
			ardSettings.altitude = server.arg("altitudeinput").toInt();
			ardSettings.altitudeinput = ardSettings.altitude;
		}
		String Settings = "Set(";
		Settings += "{\"minTempDiff\": " + String(ardSettings.tdiffmin) + ",\"maxTempCollector\": " + String(ardSettings.tkmax) + ",\"minTempCollector\": " + String(ardSettings.tkmin) + ",\"maxTempBoiler\": " + String(ardSettings.tbmax) + ",\"altitude\": " + String(ardSettings.altitude) + "});";
		Serial.print(Settings);
	}
	Serial.print("GetSettings();");
	String content = String(PAGE_HEAD);
	content += String(PAGE_MENU_LOGEDIN);
	content += String(PAGE_settings);
	content += String(PAGE_FOOT);
	server.send(200, "text/html", content);
}

