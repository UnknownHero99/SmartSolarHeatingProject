void send_system_api_data()
{
	Serial.print("GetPumps();GetPumps();GetSettings();");
	serialHandler();
	String ApiContent = "{\n\t\"data\": {";
	ApiContent += "\n\t\t\"pump1operating\":" + String(ardData.pump1operating);
	ApiContent += ",\n\t\t\"pump2operating\":" + String(ardData.pump2operating);
	ApiContent += ",\n\t\t\"pump3operating\":" + String(ardData.pump3operating);
	ApiContent += ",\n\t\t\"pump4operating\":" + String(ardData.pump4operating);
	ApiContent += ",\n\t\t\"pumpautomode\":" + String(ardData.pumpautomode);
	ApiContent += ",\n\t\t\"operatinghours\":" + String(ardData.operatinghours);
	ApiContent += ",\n\t\t\"operatingminutes\":" + String(ardData.operatingminutes);
	ApiContent += ",\n\t\t\"tempcollector\":" + String(ardData.tempcollector);
	ApiContent += ",\n\t\t\"tempboiler\":" + String(ardData.tempboiler);
	ApiContent += ",\n\t\t\"tempt1\":" + String(ardData.tempt1);
	ApiContent += ",\n\t\t\"tempt2\":" + String(ardData.tempt2);
	ApiContent += ",\n\t\t\"temproom\":" + String(ardData.temproom);
	ApiContent += ",\n\t\t\"humidityroom\":" + String(ardData.humidityroom);
	ApiContent += ",\n\t\t\"pressureroom\":" + String(ardData.pressureroom);
	ApiContent += "\n\t},";
	ApiContent += "\n\t\"settings\": {";
	ApiContent += "\n\t\t\"tdiffmin\":" + String(ardSettings.tdiffmin);
	ApiContent += "\n\t\t\"tkmax\":" + String(ardSettings.tkmax);
	ApiContent += "\n\t\t\"tkmin\":" + String(ardSettings.tkmin);
	ApiContent += "\n\t\t\"tbmax\":" + String(ardSettings.tbmax);
	ApiContent += "\n\t\t\"altitude\":" + String(ardSettings.altitude);
	ApiContent += "\n\t}";
	ApiContent += "\n}";
	server.send(200, "text/plain", ApiContent);
}

void set_system_api_data()
{
	handleSettings();
}

void api_handleLogin() {
	if (server.hasArg("USERNAME") && server.hasArg("PASSWORD")) {
		if (server.arg("USERNAME") == loginUsername &&  server.arg("PASSWORD") == loginPassword) {
			String header = "HTTP/1.1 200 OK\r\nSet-Cookie: ESPSESSIONID=1\r\nLocation: /\r\nCache-Control: no-cache\r\n\r\n";
			server.sendContent(header);
		}
	}
}
