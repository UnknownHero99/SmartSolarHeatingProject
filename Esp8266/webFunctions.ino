//Check if header is present and correct
bool is_authentified() {
	if (server.hasHeader("Cookie")) {
		String cookie = server.header("Cookie");
		if (cookie.indexOf("ESPSESSIONID=1") != -1) {
			return true;
		}
	}
	return false;
}

//login page, also called for disconnect
void handleLogin() {
	if (server.hasHeader("Cookie")) {
		String cookie = server.header("Cookie");
	}
	if (server.hasArg("DISCONNECT")) {
		String header = "HTTP/1.1 301 OK\r\nSet-Cookie: ESPSESSIONID=0\r\nLocation: /login\r\nCache-Control: no-cache\r\n\r\n";
		server.sendContent(header);
		return;
	}
	if (server.hasArg("USERNAME") && server.hasArg("PASSWORD")) {
		if (server.arg("USERNAME") == loginUsername &&  server.arg("PASSWORD") == loginPassword) {
			String header = "HTTP/1.1 301 OK\r\nSet-Cookie: ESPSESSIONID=1\r\nLocation: /\r\nCache-Control: no-cache\r\n\r\n";
			server.sendContent(header);
			return;
		}
		String msg = "Napačno uporabniško ime/geslo! Poizkusi znova.";
	}
	String content = head + menunotlogedin;
	content += "<form action='/login' method='POST'><table align='center'><tr><td><input type='text' name='USERNAME' placeholder='Uporabnisko ime'></td></tr><tr><td><input type='password' name='PASSWORD' placeholder='Geslo'></td></tr><tr><td><input class='shadow button' type='submit' value='Prijava'></td></tr></div></div>";
	content += footer;
	server.send(200, "text/html", content);
}

//root page can be accessed only if authentification is ok
void handleRoot() {
	if (is_authentified() && server.hasArg("cmd")) {
		Serial.print(server.arg("cmd") + ";");
		server.sendHeader("Location", String("/"), true);
		server.send(302, "text/plain", "");
	}
	Serial.print("GetData();");
	char json[Data.length() + 1];
	Data.toCharArray(json, Data.length() + 1);
	StaticJsonBuffer<200> jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(json);
	String date = root["date"];
	String pumpOperating = root["pumpOperating"];
	String pumpAutoMode = root["pumpAutoMode"];
	int operatingTimeHours = root["operatingTimeHours"];
	int operatingTimeMinutes = root["operatingTimeMinutes"];
	double boilerTemp = root["boilerTemp"];
	double collectorTemp = root["collectorTemp"];
	double roomTemp = root["roomTemp"];
	double roomHumidity = root["roomHumidity"];
	double t1Temp = root["t1Temp"];
	double t2Temp = root["t2Temp"];
	int roomPressure = root["roomPressure"];
	String content;
	if (is_authentified())content = head + menulogedin;
	else content = head + menunotlogedin;
	content += "<h2>trenutno stanje</h2><br/><table style='width:50%' align='center'><tr><td><h4>Senzor</h4></td><td><h4>Vrednost</h4></td></tr><tr><td><p>Status crpalke:</p></td><td><p>" + pumpOperating + "</p></td></tr><tr><td><p>Skupen čas delovanja(danes):</p></td><td><p>" + operatingTimeHours + "h, " + operatingTimeMinutes + "m</p></td></tr><tr><td><p>Avtomatsko delovanje crpalke:</p></td><td><p>" + pumpAutoMode + "</p></td></tr><tr><td><p>TK:</p></td><td><p>" + collectorTemp + "°C</p></td></tr><tr><td><p>TB:</p></td><td><p>" + boilerTemp + "°C</p></td></tr><tr><td><p>T1:</p></td><td><p>" + t1Temp + "°C</p></td></tr><tr><td><p>T2:</p></td><td><p>" + t2Temp + "°C</p></td></tr><tr><td><p>Sobna temperatura:</p></td><td><p>" + roomTemp + "°C</p></td></tr><tr><td><p>Vlaga:</p></td><td><p>" + roomHumidity + "%</p></td></tr><tr><td><p>Zracni tlak:</p></td><td><p>" + roomPressure + "mBa</p></td></tr></table></div></div>";
	content += footer;
	server.send(200, "text/html", content);
}

void handleGraphs() {
	if (is_authentified() && server.hasArg("cmd")) {
		Serial.print(server.arg("cmd") + ";");
		server.sendHeader("Location", String("/graphs"), true);
		server.send(302, "text/plain", "");
	}

	if (!is_authentified()) {
		String header = "HTTP/1.1 301 OK\r\nLocation: /graphs\r\nCache-Control: no-cache\r\n\r\n";
		server.sendContent(header);
		return;
	}
	String content = head + menulogedin;
	content += "<h2>Grafi</h2><br/><iframe width='450' height='260' style='border: 1px solid #cccccc;' src='" + graph1 + "'></iframe><iframe width='450' height='260' style='border: 1px solid #cccccc;' src='" + graph2 + "'></iframe><iframe width='450' height='260' style='border: 1px solid #cccccc;' src='" + graph3 + "'></iframe><iframe width='450' height='260' style='border: 1px solid #cccccc;' src='" + graph4 + "'></iframe><iframe width='450' height='260' style='border: 1px solid #cccccc;' src='" + graph5 + "'></iframe><iframe width='450' height='260' style='border: 1px solid #cccccc;' src='"+ graph6 +"'></iframe></div></div>";
	content += footer;
	server.send(200, "text/html", content);
}

void handleSettings() {
	if (is_authentified() && server.hasArg("cmd")) {
		Serial.print(server.arg("cmd") + ";");
		server.sendHeader("Location", String("/settings"), true);
		server.send(302, "text/plain", "");
	}
	if (!is_authentified()) {
		String header = "HTTP/1.1 301 OK\r\nLocation: /settings\r\nCache-Control: no-cache\r\n\r\n";
		server.sendContent(header);
		return;
	}
	if (server.hasArg("TDiffMin")) {
		String Settings = "Set(";
		Settings += "{\"minTempDiff\": " + String(server.arg("TDiffMin")) + ",\"maxTempCollector\": " + String(server.arg("TKMax")) + ",\"minTempCollector\": " + String(server.arg("TKMin")) + ",\"maxTempBoiler\": " + String(server.arg("TBMax")) + ",\"altitude\": " + String(server.arg("Altitude")) + "});";
		Serial.print(Settings);
		server.sendHeader("Location", String("/settings"), true);
		server.send(302, "text/plain", "");
		return;
	}
	Serial.print("GetSettings();");
	serialHandler();
	char json[Settings.length() + 1];
	Settings.toCharArray(json, Settings.length() + 1);
	StaticJsonBuffer<200> jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(json);
	int minTempDiff = root["minTempDiff"];
	int maxTempCollector = root["maxTempCollector"];
	int minTempCollector = root["minTempCollector"];
	int maxTempBoiler = root["maxTempBoiler"];
	int altitude = root["altitude"];
	String time = root["time"];
	String content = head + menulogedin;
	content += "<script>function syncTime(){var d=new Date();d.setHours(d.getHours()+2);var n=d.toISOString().split('.')[0];document.getElementById('time').value=n;}</script><h2>Nastavitve</h2><br/><form enctype='application/json'><table style='width:70%' align='center'><tr><th><h4>Nastavitev</h4></th><th><h4>Trenutna vrednost</h4></th><th><h4>Nova vrednost</h4></th></tr><tr><td><p>Min temp razlika:</p></td><td><p>" + String(minTempDiff) + "°C</p></td><td><input type='text' name='TDiffMin' value='" + String(minTempDiff) + "'></td></tr><tr><td><p>Maksimalna temperatura kolektorja:</p></td><td><p>" + String(maxTempCollector) + "°C</p></td><td><input type='text' name='TKMax' value='" + String(maxTempCollector) + "'></td></tr><tr><td><p>Minimalna temperatura kolektorja:</p></td><td><p>" + String(minTempCollector) + "°C</p></td><td><input type='text' name='TKMin' value='" + String(minTempCollector) + "'></td></tr><tr><td><p>Maksimalna temperatura bojlerja:</p></td><td><p>" + String(maxTempBoiler) + "°C</p></td><td><input type='text' name='TBMax' value='" + String(maxTempBoiler) + "'></td></tr><tr><td><p>Nadmorska višina:</p></td><td><p>" + String(altitude) + " m</p></td><td><input type='text' name='Altitude' value='" + String(altitude) + "'></td></tr><tr><td><p>Datum/ura</p></td><td><p>" + time + "</p></td><td><input id='time' type='text' name='date' value='" + time + "'></td></tr><tr><td colspan='2'><input class='shadow button' onclick='syncTime()' type='submit' value='Sinhroniziraj čas z računalnikom'></td><td colspan='1' ><input class='shadow button' type='submit' value='Shrani' style='float: right;'></td></tr></table><br></form></div></div>";
	content += footer;
	server.send(200, "text/html", content);
}

void handlePumps() {
	if (is_authentified() && server.hasArg("cmd")) {
		Serial.print(server.arg("cmd") + ";");
		server.sendHeader("Location", String("/pumps"), true);
		server.send(302, "text/plain", "");
	}
	if (!is_authentified()) {
		String header = "HTTP/1.1 301 OK\r\nLocation: /pumps\r\nCache-Control: no-cache\r\n\r\n";
		server.sendContent(header);
		return;
	}
	Serial.print("GetPumps();");
	serialHandler();
	char json[pumps.length() + 1];
	pumps.toCharArray(json, pumps.length() + 1);
	StaticJsonBuffer<200> jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(json);
	String pump1Operating = root["pump1Operating"];
	String pump2Operating = root["pump2Operating"];
	String pump3Operating = root["pump3Operating"];
	String pump4Operating = root["pump4Operating"];
	String content = head + menulogedin;
	content += "<h2 style='text-align:center'>Upravljanje črpalk/senzorjev</h2><br/><script>function r1Controller(){if (document.getElementById('r1').checked) r1Enable(); else r1Disable();}function r2Controller(){if (document.getElementById('r2').checked) r2Enable(); else r2Disable();}function r3Controller(){if (document.getElementById('r3').checked) r3Enable(); else r3Disable();}function r4Controller(){if (document.getElementById('r4').checked) r4Enable(); else r4Disable();}function r1Disable(){document.getElementById('p1activate').disabled=true; document.getElementById('p1deactivate').disabled=true; document.getElementById('p1auto').disabled=true;}function r1Enable(){document.getElementById('p1activate').disabled=false; document.getElementById('p1deactivate').disabled=false; document.getElementById('p1auto').disabled=false;}function r2Disable(){document.getElementById('p2activate').disabled=true; document.getElementById('p2deactivate').disabled=true; document.getElementById('p2auto').disabled=true;}function r2Enable(){document.getElementById('p2activate').disabled=false; document.getElementById('p2deactivate').disabled=false; document.getElementById('p2auto').disabled=false;}function r3Disable(){document.getElementById('p3activate').disabled=true; document.getElementById('p3deactivate').disabled=true; document.getElementById('p3auto').disabled=true;}function r3Enable(){document.getElementById('p3activate').disabled=false; document.getElementById('p3deactivate').disabled=false; document.getElementById('p3auto').disabled=false;}function r4Disable(){document.getElementById('p4activate').disabled=true; document.getElementById('p4deactivate').disabled=true; document.getElementById('p4auto').disabled=true;}function r4Enable(){document.getElementById('p4activate').disabled=false; document.getElementById('p4deactivate').disabled=false; document.getElementById('p4auto').disabled=false;}</script><table style='width:50%' align='center'> <tr> <td> <h4></h4></td><td> <h4>Črpalka/rele</h4></td><td> <h4>Status</h4></td><td> <h4>VKLOP</h4></td><td> <h4>IZKLOP</h4></td><td> <h4>AUTO</h4></td></tr><tr> <td><input onclick='r1Controller()' type='checkbox' id='r1' name='rele1' checked='checked'> </td><td> <p>Crpalka 1</p></td><td> <p>" + pump1Operating + "</p></td><td> <a href='?cmd=Pump(1, On);'> <input class='shadow button' id='p1activate' type='button' value='Vklop'> </a> </td><td> <a href=' ?cmd=Pump(1, Off);'> <input class='shadow button' id='p1deactivate' type='button' value='Izklop'> </a> </td><td> <a href='?cmd=Pump(1, Auto);'> <input class='shadow button' id='p1auto' type='button' value='Auto'> </a> </td></tr><tr> <td><input onclick='r2Controller()' type='checkbox' id='r2' name='rele2' checked='checked'> </td><td> <p>Crpalka 2</p></td><td> <p>" + pump2Operating + "</p></td><td> <a href='?cmd=Pump(2, On);'> <input class='shadow button' id='p2activate' type='button' value='Vklop'> </a> </td><td> <a href='?cmd=Pump(2, Off);'> <input class='shadow button' id='p2deactivate' type='button' value='Izklop'> </a> </td><td> <a href='?cmd=Pump(2, Auto);'> <input class='shadow button' id='p2auto' type='button' value='Auto'> </a> </td></tr><tr> <td> <input onclick='r3Controller()' type='checkbox' id='r3' name='rele3' checked='checked'> </td><td> <p>Crpalka 3</p></td><td> <p>" + pump3Operating + "</p></td><td> <a href='?cmd=Pump(3, On);'> <input class='shadow button' id='p3activate' type='button' value='Vklop'> </a> </td><td> <a href='?cmd=Pump(3, Off); '> <input class='shadow button' id='p3deactivate' type='button' value='Izklop'> </a> </td><td> <a href='?cmd=Pump(3, Auto);'> <input class='shadow button' id='p3auto' type='button' value='Auto'> </a> </td></tr><tr> <td> <input onclick='r4Controller()' type='checkbox' id='r4' name='rele1' checked='checked'> </td><td> <p>Crpalka 4</p></td><td> <p>" + pump4Operating + "</p></td><td> <a href='?cmd=Pump(4, On);'> <input class='shadow button' id='p4activate' type='button' value='Vklop'> </a> </td><td> <a href='?cmd=Pump(4, Off);'> <input class='shadow button' id='p4deactivate' type='button' value='Izklop'> </a> </td><td> <a href='?cmd=Pump(4, Auto);'> <input class='shadow button' id='p4auto' type='button' value='Auto'> </a> </td></tr></table></div></div>";
	content += footer;
	server.send(200, "text/html", content);
}

//no need authentification
void handleNotFound() {
	String message = "File Not Found\n\n";
	message += "URI: ";
	message += server.uri();
	message += "\nMethod: ";
	message += (server.method() == HTTP_GET) ? "GET" : "POST";
	message += "\nArguments: ";
	message += server.args();
	message += "\n";
	for (uint8_t i = 0; i < server.args(); i++) {
		message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
	}
	server.send(404, "text/plain", message);
}
