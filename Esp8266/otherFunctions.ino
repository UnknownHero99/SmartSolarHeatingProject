void serialHandler() {
	String input = "";
	if (Serial.available()) {
		input = Serial.readStringUntil(';');;
		String cmd = input.substring(0, input.indexOf('('));
		String args = input.substring(input.indexOf('(') + 1, input.length() - 1);

		if (cmd == "GetIP") {
			Serial.println("IP(NA);");
		}

		else if (cmd == "ThingSpeak") {
			String thingSpeakData = "&field2=";
			thingSpeakData += arduinoData[8];
			thingSpeakData += "&field2=";
			thingSpeakData += arduinoData[9];
			thingSpeakData += "&field3=";
			thingSpeakData += arduinoData[10];
			thingSpeakData += "&field4=";
			thingSpeakData += arduinoData[11];
			thingSpeakData += "&field5=";
			thingSpeakData += arduinoData[14];
			thingSpeakData += "\r\n\r\n";
			String postStr = apiKey + thingSpeakData;

			server.sendContent(postStr);

			if (client.connect(thingspeak, 80)) { // "184.106.153.149" or api.thingspeak.com
				client.print("POST /update HTTP/1.1\n");
				client.print("Host: api.thingspeak.com\n");
				client.print("Connection: close\n");
				client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
				client.print("Content-Type: application/x-www-form-urlencoded\n");
				client.print("Content-Length: ");
				client.print(postStr.length());
				client.print("\n\n");
				client.print(postStr);
			}
			client.stop();
		}
		else if (cmd == "Settings") {
			settingsUpdate(args);
		}
		else if (cmd == "Data") {
			dataUpdate(args);
		}
		else if (cmd == "PumpStatus") {
			pumpUpdate(args);
		}
	}
}

bool pumpUpdate(String pumps) {
	char json[pumps.length() + 1];
	pumps.toCharArray(json, pumps.length() + 1);
	StaticJsonBuffer<200> jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(json);
	if (!root.success()) return false;

	String pump1Operating = root["pump1Operating"];
	String pump2Operating = root["pump2Operating"];
	String pump3Operating = root["pump3Operating"];
	String pump4Operating = root["pump4Operating"];

	arduinoData[1] = pump1Operating;
	arduinoData[2] = pump2Operating;
	arduinoData[3] = pump3Operating;
	arduinoData[4] = pump4Operating;
	return true;
}

bool dataUpdate(String data) {
	char json[data.length() + 1];
	data.toCharArray(json, data.length() + 1);
	StaticJsonBuffer<200> jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(json);
	if (!root.success()) return false;

	ardData.pumpautomode = root["pumpAutoMode"];
	ardData.operatinghours = root["operatingTimeHours"];
	ardData.operatingminutes = root["operatingTimeMinutes"];
	ardData.tempboiler = root["boilerTemp"];
	ardData.tempcollector = root["collectorTemp"];
	ardData.temproom = root["roomTemp"];
	ardData.humidityroom = root["roomHumidity"];
	ardData.tempt1 = root["t1Temp"];
	ardData.tempt2 = root["t2Temp"];
	ardData.pressureroom = root["roomPressure"];
	return true;
}

bool settingsUpdate(String settings) {
	char json[settings.length() + 1];
	settings.toCharArray(json, settings.length() + 1);
	StaticJsonBuffer<200> jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(json);
	if (!root.success()) return false;

	String minTempDiff = root["minTempDiff"];
	String maxTempCollector = root["maxTempCollector"];
	String minTempCollector = root["minTempCollector"];
	String maxTempBoiler = root["maxTempBoiler"];
	String altitude = root["altitude"];
	String time = root["time"];

	arduinoSettings[0] = minTempDiff;
	arduinoSettings[1] = maxTempCollector;
	arduinoSettings[2] = minTempCollector;
	arduinoSettings[3] = maxTempBoiler;
	arduinoSettings[4] = altitude;
	arduinoSettings[5] = time;
	return true;
}

void update() {
	Serial.print("GetSettings();GetData();GetPumps();");
	lastUpdate = millis();
}

