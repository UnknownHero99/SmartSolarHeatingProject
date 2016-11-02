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
			String postStr = apiKey + args;
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
			char json[args.length() + 1];
			args.toCharArray(json, args.length() + 1);
			StaticJsonBuffer<200> jsonBuffer;
			JsonObject& root = jsonBuffer.parseObject(json);
			if (!root.success()) return;
			Settings = args;
		}
		else if (cmd == "Data") {
			char json[args.length() + 1];
			args.toCharArray(json, args.length() + 1);
			StaticJsonBuffer<200> jsonBuffer;
			JsonObject& root = jsonBuffer.parseObject(json);
			if (!root.success()) return;
			Data = args;
		}
	}
}

void update() {
	Serial.print("GetSettings();GetData();");
	lastUpdate = millis();
}

