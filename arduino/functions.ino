
void TempHandler() {
	if (autoMode) {
		if ((collectorSensor.tempDouble() - boilerSensor.tempDouble() >= settingsMinTempDifference && collectorSensor.tempDouble() >= settingsMinTempCollector && boilerSensor.tempDouble() < settingsMaxTempBoiler && !pumps[0].isOperating()) || (collectorSensor.tempDouble() > settingsMaxTempCollector && boilerSensor.tempDouble() < settingsMaxTempBoiler && !pumps[0].isOperating()))
		{
			pumps[0].on();
		}

		if ((collectorSensor.tempDouble() - boilerSensor.tempDouble() < settingsMinTempDifference && collectorSensor.tempDouble() < settingsMaxTempCollector && pumps[0].isOperating()) || (boilerSensor.tempDouble() >= settingsMaxTempBoiler && pumps[0].isOperating()) || (collectorSensor.tempDouble() < settingsMinTempCollector && pumps[0].isOperating())) {
			pumps[0].off();
		}
	}
	else {
		if ((collectorSensor.tempDouble() > settingsMaxTempCollector && boilerSensor.tempDouble() < settingsMaxTempBoiler && !pumps[0].isOperating())) {

			pumps[0].on();
		}

		if ((boilerSensor.tempDouble() >= settingsMaxTempBoiler && pumps[0].isOperating())) {
			pumps[0].off();
		}
	}
}

void TempUpdate() {
	boilerSensor.updateTemp();
	collectorSensor.updateTemp();
	t1Sensor.updateTemp();
	t2Sensor.updateTemp();
}

void DhtUpdate() {
	roomTemp = dht.readTemperature();
	roomHumidity = dht.readHumidity();
}

void PressureUpdate() {
	char status;
	double T, P, p0, a;
	status = pressure.startTemperature();
	if (status != 0)
	{
		delay(status);

		status = pressure.getTemperature(T);
		if (status != 0)
		{

			status = pressure.startPressure(3);
			if (status != 0)
			{
				delay(status);

				status = pressure.getPressure(P, T);
				if (status != 0)
				{
					roomPressure = pressure.sealevel(P, altitude); // smo na nadmorski viďż˝ini 290m
				}
				else pressureProblem = true;
			}
			else pressureProblem = true;
		}
		else pressureProblem = true;
	}
	else pressureProblem = true;

}

void sensorUpdate() {
	now = rtc.now();
	DhtUpdate();
	TempUpdate();
	PressureUpdate();
	lastUpdate = millis();
	statisticshandler();
	if (!sleeping) lcdUpdate();
}

int eeprommultiplace(int epl1, int epl2, int epl3, int epl4) {
	int number;
	number = EEPROM.read(epl4) * 10;
	number = (number + EEPROM.read(epl3)) * 10;
	number = (number + EEPROM.read(epl2)) * 10;
	number = (number + EEPROM.read(epl1));
	return number;
}

void eeprom_read_config() {
	settingsMinTempDifference = EEPROM.read(0);
	settingsMinTempCollector = EEPROM.read(1);
	settingsMaxTempCollector = EEPROM.read(2);
	settingsMaxTempBoiler = EEPROM.read(3);
	contrast = EEPROM.read(4);
	brightness = EEPROM.read(5);
	altitude = eeprommultiplace(6, 7, 8, 9);
}

void statisticshandler() {
	if (roomPressure > roomMaxPressure) roomMaxPressure = roomPressure;
	if (roomPressure < roomMinPressure) roomMinPressure = roomPressure;
	if (roomHumidity > roomMaxHumidity) roomMaxHumidity = roomHumidity;
	if (roomHumidity < roomMinHumidity) roomMinHumidity = roomHumidity;
	if (roomTemp > roomMaxTemp) roomMaxTemp = roomTemp;
	if (roomTemp < roomMinTemp) roomMinTemp = roomTemp;
}

void sdcardwrite() {
	esphandler();
	if (!SD.begin(ChipSelect)) sdProblem = true;
	else sdProblem = false;
	String filename = "LOG-" + String(now.day()) + "." + String(now.month()) + "." + String(now.year()) + ".csv";
	Serial.println(SD.exists(filename.c_str()));
	if (!SD.exists(filename.c_str())) {
		Serial.println(filename);
		logfile = SD.open(filename.c_str(), FILE_WRITE);
		if (logfile)logfile.println("Time;Collector Temp;Max Collector Temp;Min Collector Temp;Boiler Temp;Max Boiler Temp;Min Boiler Temp;Max room Temp;Min room Temp;Max room Temp;Min room Temp;Max room Humidity;Min room Humidity;Pump");
		logfile.close();
	}
	else logfile = SD.open(filename.c_str(), FILE_WRITE);
	if (logfile) {
		logfile.println(String(now.hour()) + ":" + String(now.minute()) + ";" + String(collectorSensor.tempDouble()) + ";" + String(collectorSensor.statistics("%Max")) + ";" + String(collectorSensor.statistics("%Min")) + ";" + String(boilerSensor.tempDouble()) + ";" + String(boilerSensor.statistics("%Max")) + ";" + String(boilerSensor.statistics("%Min")) + ";" + String(roomMaxTemp) + ";" + String(roomMinTemp) + ";" + String(roomMaxPressure) + ";" + String(roomMinPressure) + ";" + String(roomMaxHumidity) + ";" + String(roomMinHumidity) + ";" + String(pumps[0].isOperating()));
		logfile.close();
	}
	lastLog = millis();
}

void setColor(int red = 0, int green = 0, int blue = 0)
{
	analogWrite(redPin, map(red, 0, 255, 255, 0));
	analogWrite(greenPin, map(green, 0, 255, 255, 0));
	analogWrite(bluePin, map(blue, 0, 255, 255, 0));
}

void ledHandler() {
	if (pumps[0].isOperating()) setColor(0, 255, 0);

	if (!pumps[0].isOperating()) setColor(153, 0, 76);

	if (sdProblem) {
		setColor(153, 76, 0);
		Serial.println("Problem with SD card");
	}

	if (isnan(roomTemp) || isnan(roomHumidity)) {
		setColor(0, 0, 255);
		Serial.println("Problem with DHT22 sensor");
	}

	if (collectorSensor.tempDouble() == -127) {
		setColor(0, 0, 255);
		Serial.println("Problem with collector temp sensor");
	}

	if (boilerSensor.tempDouble() == -127) {
		setColor(0, 0, 255);
		Serial.println("Problem with boiler temp sensor");
	}

	if (pressureProblem) {
		setColor(0, 0, 255);
		Serial.println("Problem with room pressure sensor");
	}

	if (boilerSensor.tempDouble() > settingsMaxTempBoiler || collectorSensor.tempDouble() > settingsMaxTempCollector) {
		setColor(255, 0, 0);
		Serial.println("OVERHEATED!!");
	}
}

void esphandler() {
	Serial2.print("ThingSpeak();");
}

void serialhandler() {
	String input = "";
	while (Serial2.available()) {
		input = Serial2.readStringUntil(';');
		String cmd = input.substring(0, input.indexOf('('));
		String args = input.substring(input.indexOf('(') + 1, input.length() - 1);
		if (cmd == "Pump") {
			String pumpName = args.substring(0, args.indexOf(','));
			String state = args.substring(args.indexOf(',') + 2);
			for (int i = 0; i < sizeof(pumps) / sizeof(pumps[0]); i++) {
				if (pumps[i].getName() == pumpName) {
					if (state == "On") {
						autoMode = false;
						pumps[i].on();
					}
					else if (state == "Off") {
						autoMode = false;
						pumps[i].off();
					}
					else if (state == "Auto")autoMode = true;
					else if (state == "Enable")pumps[i].enable();
					else if (state == "Disable")pumps[i].disable();
					else if (state == "Reset")pumps[i].resetTime();
				}
			}
		}
		else if (cmd == "GetData") {
			sendData();
		}

		else if (cmd == "GetSettings") {
			sendSettings();
		}

		else if (cmd == "GetPumps") {
			sendPumps();
		}

		else if (cmd == "Set") {
			char json[args.length() + 1];
			args.toCharArray(json, args.length() + 1);
			StaticJsonBuffer<200> jsonBuffer;
			JsonObject& root = jsonBuffer.parseObject(json);
			if (!root.success()) return;
			settingsMinTempDifference = root["minTempDiff"];
			settingsMinTempCollector = root["minTempCollector"];
			settingsMaxTempCollector = root["maxTempCollector"];
			settingsMaxTempBoiler = root["maxTempBoiler"];
			altitude = root["altitude"];
			EEPROM.update(0, settingsMinTempDifference);
			EEPROM.update(1, settingsMinTempCollector);
			EEPROM.update(2, settingsMaxTempCollector);
			EEPROM.update(3, settingsMaxTempBoiler);
			int value = altitude % 10;
			EEPROM.update(6, value);
			value = (altitude / 10) % 10;
			EEPROM.update(7, value);
			value = (altitude / 100) % 10;
			EEPROM.update(8, value);
			value = (altitude / 1000) % 10;
			EEPROM.update(9, value);
		}

		else if (cmd == "IP") {
			IP = args;
		}

	}
}

void sendData() {
	String date = "\"" + String(now.day()) + "." + String(now.month()) + "." + String(now.year()) + " " + String(now.hour()) + ":" + String(now.minute()) + "\"";
	String data = "{\"date\": " + date + ",\"pumpOperating\": ";
	if (pumps[0].isOperating()) data += "\"On\"";
	else data += "\"Off\"";
	data += ",\"operatingTimeHours\": " + String(pumps[0].operatingTime("%H")) + ",\"operatingTimeMinutes\": " + String(pumps[0].operatingTime("%M")) + ",\"boilerTemp\": " + String(boilerSensor.temp()) + ",\"collectorTemp\": " + String(collectorSensor.temp()) + ",\"t1Temp\": " + String(t1Sensor.temp()) + ",\"t2Temp\": " + String(t2Sensor.temp()) + ",\"roomTemp\": " + String(roomTemp) + ",\"roomHumidity\": " + String(roomHumidity) + ",\"roomPressure\": " + String(roomPressure) + "}";
	Serial2.print("Data(" + data + ");");
}

void sendSettings() {
	String data = "{\"minTempDiff\": " + String(settingsMinTempDifference) + ",\"maxTempCollector\": " + String(settingsMaxTempCollector) + ",\"minTempCollector\": " + String(settingsMinTempCollector) + ",\"maxTempBoiler\": " + String(settingsMaxTempBoiler) + ",\"altitude\": " + String(altitude) + "}";
	Serial2.print("Settings(" + data + ");");
}

void sendPumps() {
	String data = "{\"pump1Operating\": \"" + String(pumps[0].isOperating());
 
  data += "\",\"pump1Status\": \"";
	if(autoMode) data += "A";
  else if(pumps[0].isOperating())data += "On";
  else data += "Off";
  data += "\",\"pump2Status\": \"";
  if(pumps[1].isOperating())data += "On";
  else data += "Off";
  data += "\",\"pump3Status\": \"";
  if(pumps[2].isOperating())data += "On";
  else data += "Off";
  data += "\",\"pump4Status\": \"";
  if(pumps[3].isOperating())data += "On";
  else data += "Off";
  data += "\"}";
	Serial2.print("PumpStatus(" + data + ");");
}

void resetStatistics() {
	collectorSensor.resetStatistics();
	boilerSensor.resetStatistics();
	t1Sensor.resetStatistics();
	t2Sensor.resetStatistics();
	roomMinPressure = roomPressure;
	roomMaxPressure = roomPressure;
	roomMinHumidity = roomHumidity;
	roomMaxHumidity = roomHumidity;
	roomMinTemp = roomTemp;
	roomMaxTemp = roomTemp;
}

void getIP(){
  Serial2.print("GetIP();");
}


