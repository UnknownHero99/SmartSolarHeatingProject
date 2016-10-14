
void TempHandler() {
  if (autoMode) {
    if ((collectorSensor.tempDouble() - boilerSensor.tempDouble() >= settingsMinTempDifference && collectorSensor.tempDouble() >= settingsMinTempCollector && boilerSensor.tempDouble() < settingsMaxTempBoiler && !mainPump.isOperating()) || (collectorSensor.tempDouble() > settingsMaxTempCollector && boilerSensor.tempDouble() < settingsMaxTempBoiler && !mainPump.isOperating()))
    {
      mainPump.on();
    }

    if ((collectorSensor.tempDouble() - boilerSensor.tempDouble() < settingsMinTempDifference && collectorSensor.tempDouble() < settingsMaxTempCollector && mainPump.isOperating()) || (boilerSensor.tempDouble() >= settingsMaxTempBoiler && mainPump.isOperating()) || (collectorSensor.tempDouble() < settingsMinTempCollector && mainPump.isOperating())) {
      mainPump.off();
    }
  }
  else {
    if ((collectorSensor.tempDouble() > settingsMaxTempCollector && boilerSensor.tempDouble() < settingsMaxTempBoiler && !mainPump.isOperating())) {

      mainPump.on();
    }

    if ((boilerSensor.tempDouble() >= settingsMaxTempBoiler && mainPump.isOperating())) {
      mainPump.off();
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
    logfile.println(String(now.hour()) + ":" + String(now.minute()) + ";" + String(collectorSensor.tempDouble()) + ";" + String(collectorSensor.statistics("%Max")) + ";" + String(collectorSensor.statistics("%Min")) + ";" + String(boilerSensor.tempDouble()) + ";" + String(boilerSensor.statistics("%Max")) + ";" + String(boilerSensor.statistics("%Min")) + ";" + String(roomMaxTemp) + ";" + String(roomMinTemp) + ";" + String(roomMaxPressure) + ";" + String(roomMinPressure) + ";" + String(roomMaxHumidity) + ";" + String(roomMinHumidity) + ";" + String(mainPump.isOperating()));
    logfile.close();
  }
  esphandler();
  lastLog = millis();
}

void setColor(int red = 0, int green = 0, int blue = 0)
{

  analogWrite(redPin, map(red, 0, 255, 255, 0));
  analogWrite(greenPin, map(green, 0, 255, 255, 0));
  analogWrite(bluePin, map(blue, 0, 255, 255, 0));
}

void ledHandler() {
  if (mainPump.isOperating()) setColor(0, 255, 0);

  if (!mainPump.isOperating()) setColor(153, 0, 76);

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
  String Espsend = "&field1=";
  Espsend += boilerSensor.temp();
  Espsend += "&field2=";
  Espsend += collectorSensor.temp();
  Espsend += "&field3=";
  Espsend += roomTemp;
  Espsend += "&field4=";
  Espsend += roomHumidity;
  Espsend += "&field5=";
  Espsend += roomPressure;
  Espsend += "\r\n\r\n";
  Serial2.print(">");
  delay(200);
  Serial2.print(Espsend);
}

String firstxchars(String input, int chars) {
  String output = "";
  for (int i = 0; i < chars; i++) output += input[i];
  return output;
}

void serialhandler() {
  String input = "";
  if (Serial2.available()) {
    input = Serial2.readStringUntil(';');

    if (input == "PumpOn") {
      autoMode = false;
      mainPump.on();
      return;
    }

    if (input == "PumpOff") {
      autoMode = false;
      mainPump.off();
      return;
    }

    if (input == "PumpAuto") {
      autoMode = true;
      sensorUpdate();
      TempHandler();
      ledHandler();
      return;
    }

    if (input == "reqAllData") {
      String date = "\"" + String(now.day()) + "." + String(now.month()) + "." + String(now.year()) + " " + String(now.hour()) + ":" + String(now.minute()) + "\"";
      String data = "{\"date\": " + date + ",\"pumpOperating\": ";
      if (mainPump.isOperating()) data += "\"On\"";
      else data += "\"Off\"";
      data += ",\"pumpAutoMode\": ";
      if (autoMode) data += "\"On\"";
      else data += "\"Off\"";
      data += ",\"operatingTimeHours\": " + String(mainPump.operatingTime("%H")) + ",\"operatingTimeMinutes\": " + String(mainPump.operatingTime("%M")) + ",\"boilerTemp\": " + String(boilerSensor.temp()) + ",\"collectorTemp\": " + String(collectorSensor.temp()) + ",\"t1Temp\": " + String(t1Sensor.temp()) + ",\"t2Temp\": " + String(t2Sensor.temp()) + ",\"roomTemp\": " + String(roomTemp) + ",\"roomHumidity\": " + String(roomHumidity) + ",\"roomPressure\": " + String(roomPressure) + "}";
      Serial2.println(data);
      Serial.print(data);
      return;
    }

    if (input == "reqSettings") {
      String data = "{\"minTempDiff\": " + String(settingsMinTempDifference) + ",\"maxTempCollector\": " + String(settingsMaxTempCollector) + ",\"minTempCollector\": " + String(settingsMinTempCollector) + ",\"maxTempBoiler\": " + String(settingsMaxTempBoiler) + ",\"altitude\": " + String(altitude) + "}";
      Serial2.println(data);
      Serial.println(data);
      return;
    }

    if (firstxchars(input, 3) == "SET") {
      String jsonSettings = input.substring(3);
      Serial.println(jsonSettings);
      char json[jsonSettings.length()];
      jsonSettings.toCharArray(json, jsonSettings.length());
      StaticJsonBuffer<200> jsonBuffer;
      JsonObject& root = jsonBuffer.parseObject(json);
      settingsMinTempDifference = root["minTempDiff"];
      settingsMinTempCollector = root["minTempCollector"];
      settingsMaxTempCollector = root["maxTempCollector"];
      settingsMaxTempBoiler = root["maxTempBoiler"];
      altitude = root["altitude"];
      EEPROM.update(0, settingsMinTempDifference);
      EEPROM.update(1, settingsMinTempCollector);
      EEPROM.update(2, settingsMaxTempCollector);
      EEPROM.update(3, settingsMaxTempBoiler);
      Serial.println(altitude);
      int value = altitude % 10;
      EEPROM.update(6, value);
      value = (altitude / 10) % 10;
      EEPROM.update(7, value);
      value = (altitude / 100) % 10;
      EEPROM.update(8, value);
      value = (altitude / 1000) % 10;
      EEPROM.update(9, value);
      serialhandler();
    }
  }
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


