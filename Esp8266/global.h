bool settingsUpdate(String settings) {
  char json[settings.length() + 1];
  settings.toCharArray(json, settings.length() + 1);
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  if (!root.success()) return false;

  ardSettings.tdiffmin = root["minTempDiff"];
  ardSettings.tdiffmininput = ardSettings.tdiffmin;

  ardSettings.tkmax = root["maxTempCollector"];
  ardSettings.tkmaxinput = ardSettings.tkmax;

  ardSettings.tkmin = root["minTempCollector"];
  ardSettings.tkmininput = ardSettings.tkmin;

  ardSettings.tbmax = root["maxTempBoiler"];
  ardSettings.tbmaxinput = ardSettings.tbmax;

  ardSettings.altitude = root["altitude"];
  ardSettings.altitudeinput = ardSettings.altitude;

  return true;
}

bool pumpUpdate(String pumps) {
  char json[pumps.length() + 1];
  pumps.toCharArray(json, pumps.length() + 1);
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  if (!root.success()) return false;

  ardData.pump1operating = root["pump1Operating"];
  ardData.pump2operating = root["pump2Operating"];
  ardData.pump3operating = root["pump3Operating"];
  ardData.pump4operating = root["pump4Operating"];
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

void update() {
	Serial.print("GetSettings();GetData();GetPumps();");
	lastUpdate = millis();
}

