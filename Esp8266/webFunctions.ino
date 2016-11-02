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
	String msg;
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
		msg = "Napačno uporabniško ime/geslo! Poizkusi znova.";
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
	String header;
	if (is_authentified() && server.hasArg("cmd")) {
		Serial.print(server.arg("cmd") + ";");
		server.sendHeader("Location", String("/graphs"), true);
		server.send(302, "text/plain", "");
	}

	if (!is_authentified()) {
		String header = "HTTP/1.1 301 OK\r\nLocation: /login\r\nCache-Control: no-cache\r\n\r\n";
		server.sendContent(header);
		return;
	}
	String content = head + menulogedin;
	content += "<h2>Grafi</h2><br/><iframe width='450' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/132705/charts/1?bgcolor=%23ffffff&color=%23d62020&days=1&dynamic=true&max=120&min=-10&results=60&title=Temperatura+bojlerja&type=line&xaxis=%C4%8Cas&yaxis=Temperatura+%28%C2%B0C%29'></iframe><iframe width='450' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/132705/charts/2?bgcolor=%23ffffff&color=%23d62020&days=1&dynamic=true&max=120&min=-10&results=60&title=Temperatura+kolektorja&type=line&xaxis=%C4%8Cas&yaxis=Temperatura+%28%C2%B0C%29'></iframe><iframe width='450' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/132705/charts/3?bgcolor=%23ffffff&color=%23d62020&days=7&dynamic=true&max=50&min=-20&results=60&title=Temperatura+prostora&type=line&xaxis=%C4%8Cas&yaxis=Temperatura+%28%C2%B0C%29'></iframe><iframe width='450' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/132705/charts/4?bgcolor=%23ffffff&color=%23d62020&dynamic=true&max=100&min=10&results=60&title=Vlaga+prostora&type=line&xaxis=%C4%8Cas&yaxis=Vlaga+%28%25%29'></iframe><iframe width='450' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/132705/charts/5?bgcolor=%23ffffff&color=%23d62020&days=7&dynamic=true&max=1100&min=900&results=60&title=Barometer&type=line&xaxis=%C4%8Cas&yaxis=Zra%C4%8Dni+tlak+%28mBa%29'></iframe><iframe width='450' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/132705/maps/channel_show'></iframe></div></div>";
	content += footer;
	server.send(200, "text/html", content);
}

void handleSettings() {
	String header;
	if (is_authentified() && server.hasArg("cmd")) {
		Serial.print(server.arg("cmd") + ";");
		server.sendHeader("Location", String("/settings"), true);
		server.send(302, "text/plain", "");
	}
	if (!is_authentified()) {
		String header = "HTTP/1.1 301 OK\r\nLocation: /login\r\nCache-Control: no-cache\r\n\r\n";
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
