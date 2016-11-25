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
	delay(500);
	serialHandler();
	String content = head + menulogedin;
	content += "<script>function syncTime(){var d=new Date();d.setHours(d.getHours()+2);var n=d.toISOString().split('.')[0];document.getElementById('time').value=n;}</script><h2>Nastavitve</h2><br/><form enctype='application/json'><table style='width:70%' align='center'><tr><th><h4>Nastavitev</h4></th><th><h4>Trenutna vrednost</h4></th><th><h4>Nova vrednost</h4></th></tr><tr><td><p>Min temp razlika:</p></td><td><p>" + arduinoSettings[0] + "°C</p></td><td><input type='text' name='TDiffMin' value='" + arduinoSettings[0] + "'></td></tr><tr><td><p>Maksimalna temperatura kolektorja:</p></td><td><p>" + arduinoSettings[1] + "°C</p></td><td><input type='text' name='TKMax' value='" + arduinoSettings[1] + "'></td></tr><tr><td><p>Minimalna temperatura kolektorja:</p></td><td><p>" + arduinoSettings[2] + "°C</p></td><td><input type='text' name='TKMin' value='" + arduinoSettings[2] + "'></td></tr><tr><td><p>Maksimalna temperatura bojlerja:</p></td><td><p>" + arduinoSettings[3] + "°C</p></td><td><input type='text' name='TBMax' value='" + arduinoSettings[3] + "'></td></tr><tr><td><p>Nadmorska višina:</p></td><td><p>" + arduinoSettings[4] + " m</p></td><td><input type='text' name='Altitude' value='" + arduinoSettings[4] + "'></td></tr><tr><td><p>Datum/ura</p></td><td><p>" + arduinoSettings[5] + "</p></td><td><input id='time' type='text' name='date' value='" + arduinoSettings[5] + "'></td></tr><tr><td colspan='2'><input class='shadow button' onclick='syncTime()' type='submit' value='Sinhroniziraj čas z računalnikom'></td><td colspan='1' ><input class='shadow button' type='submit' value='Shrani' style='float: right;'></td></tr></table><br></form></div></div>";
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
	String content = head + menulogedin;
	content += "<h2 style='text-align:center'>Upravljanje črpalk/senzorjev</h2><br/><script>function r1Controller(){if (document.getElementById('r1').checked) r1Enable(); else r1Disable();}function r2Controller(){if (document.getElementById('r2').checked) r2Enable(); else r2Disable();}function r3Controller(){if (document.getElementById('r3').checked) r3Enable(); else r3Disable();}function r4Controller(){if (document.getElementById('r4').checked) r4Enable(); else r4Disable();}function r1Disable(){document.getElementById('p1activate').disabled=true; document.getElementById('p1deactivate').disabled=true; document.getElementById('p1auto').disabled=true;}function r1Enable(){document.getElementById('p1activate').disabled=false; document.getElementById('p1deactivate').disabled=false; document.getElementById('p1auto').disabled=false;}function r2Disable(){document.getElementById('p2activate').disabled=true; document.getElementById('p2deactivate').disabled=true; document.getElementById('p2auto').disabled=true;}function r2Enable(){document.getElementById('p2activate').disabled=false; document.getElementById('p2deactivate').disabled=false; document.getElementById('p2auto').disabled=false;}function r3Disable(){document.getElementById('p3activate').disabled=true; document.getElementById('p3deactivate').disabled=true; document.getElementById('p3auto').disabled=true;}function r3Enable(){document.getElementById('p3activate').disabled=false; document.getElementById('p3deactivate').disabled=false; document.getElementById('p3auto').disabled=false;}function r4Disable(){document.getElementById('p4activate').disabled=true; document.getElementById('p4deactivate').disabled=true; document.getElementById('p4auto').disabled=true;}function r4Enable(){document.getElementById('p4activate').disabled=false; document.getElementById('p4deactivate').disabled=false; document.getElementById('p4auto').disabled=false;}</script><table style='width:50%' align='center'> <tr> <td> <h4></h4></td><td> <h4>Črpalka/rele</h4></td><td> <h4>Status</h4></td><td> <h4>VKLOP</h4></td><td> <h4>IZKLOP</h4></td><td> <h4>AUTO</h4></td></tr><tr> <td><input onclick='r1Controller()' type='checkbox' id='r1' name='rele1' checked='checked'> </td><td> <p>Crpalka 1</p></td><td> <p>" + arduinoData[1] + "</p></td><td> <a href='?cmd=Pump(1, On);'> <input class='shadow button' id='p1activate' type='button' value='Vklop'> </a> </td><td> <a href=' ?cmd=Pump(1, Off);'> <input class='shadow button' id='p1deactivate' type='button' value='Izklop'> </a> </td><td> <a href='?cmd=Pump(1, Auto);'> <input class='shadow button' id='p1auto' type='button' value='Auto'> </a> </td></tr><tr> <td><input onclick='r2Controller()' type='checkbox' id='r2' name='rele2' checked='checked'> </td><td> <p>Crpalka 2</p></td><td> <p>" + arduinoData[2] + "</p></td><td> <a href='?cmd=Pump(2, On);'> <input class='shadow button' id='p2activate' type='button' value='Vklop'> </a> </td><td> <a href='?cmd=Pump(2, Off);'> <input class='shadow button' id='p2deactivate' type='button' value='Izklop'> </a> </td><td> <a href='?cmd=Pump(2, Auto);'> <input class='shadow button' id='p2auto' type='button' value='Auto'> </a> </td></tr><tr> <td> <input onclick='r3Controller()' type='checkbox' id='r3' name='rele3' checked='checked'> </td><td> <p>Crpalka 3</p></td><td> <p>" + arduinoData[3] + "</p></td><td> <a href='?cmd=Pump(3, On);'> <input class='shadow button' id='p3activate' type='button' value='Vklop'> </a> </td><td> <a href='?cmd=Pump(3, Off); '> <input class='shadow button' id='p3deactivate' type='button' value='Izklop'> </a> </td><td> <a href='?cmd=Pump(3, Auto);'> <input class='shadow button' id='p3auto' type='button' value='Auto'> </a> </td></tr><tr> <td> <input onclick='r4Controller()' type='checkbox' id='r4' name='rele1' checked='checked'> </td><td> <p>Crpalka 4</p></td><td> <p>" + arduinoData[4] + "</p></td><td> <a href='?cmd=Pump(4, On);'> <input class='shadow button' id='p4activate' type='button' value='Vklop'> </a> </td><td> <a href='?cmd=Pump(4, Off);'> <input class='shadow button' id='p4deactivate' type='button' value='Izklop'> </a> </td><td> <a href='?cmd=Pump(4, Auto);'> <input class='shadow button' id='p4auto' type='button' value='Auto'> </a> </td></tr></table></div></div>";
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
