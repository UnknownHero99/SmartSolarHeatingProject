#include <ESPmDNS.h>

#include <WiFiClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include "Pump.h"
#include "TempSensors.h"

WiFiClient client; // needed for sending data to thingspeak
const char *thingspeak = "api.thingspeak.com";
AsyncWebServer server(80); // The Webserver



// Pin settings
#define boilerSensorPin 32
#define collectorSensorPin 33
#define t1SensorPin 25
#define t2SensorPin 26
// rgb pins
int redPin = 27;
int greenPin = 14;
int bluePin = 12;

TempSensor boilerSensor(boilerSensorPin);
TempSensor collectorSensor(collectorSensorPin);
TempSensor t1Sensor(t1SensorPin);
TempSensor t2Sensor(t2SensorPin);

Pump pumps[4] = {Pump("1", 4), Pump("2", 16), Pump("3", 17),
                 Pump("4", 5)
                };

DateTime now;

unsigned long lastUpdate = 0;
unsigned long updateInterval = 5L * 1000L; // 5s
unsigned long lastSensorsUpdate = 0;
unsigned long lastThingspeak = 0;
unsigned long thingspeakInterval = 5L * 60L * 1000L; // 5 mins
bool antiFreezeActivated = false;
int antiFreezeTemperature = -4;

float roomTemp, roomHumidity = 0;
float roomPressure = 1111;
bool autoMode = true;

int problemID = 0; // 0-No problem, 1-BME280 Problem

void SPIFFSInitReadData() {
  SPIFFS.begin(true);
  File f = SPIFFS.open("/data.txt", "r");
  loginPassword = f.readStringUntil('|');
  if (loginPassword == "")
    loginPassword = "admin";
  thingspeakChannelID = f.readStringUntil('|');
  apiKey = f.readStringUntil('|');
  SettingsValues.tdiffmin = f.readStringUntil('|').toInt();
  SettingsValues.tkmax = f.readStringUntil('|').toInt();
  SettingsValues.tkmin = f.readStringUntil('|').toInt();
  SettingsValues.tbmax = f.readStringUntil('|').toInt();
  SettingsValues.altitude = f.readStringUntil('|').toInt();
  SettingsValues.ssid = f.readStringUntil('|');
  SettingsValues.password = f.readStringUntil('|');
  f.close();
}

void wifiConnect() {
  LCDHandler::loadWIFIAPS();
  LCDHandler::switchPage(1);
  static unsigned int start = millis();
  while (WiFi.status() != WL_CONNECTED) {
    if(millis()-start > 60*1000 && SettingsValues.ssid != ""){
      Serial.println("connecting to : " + SettingsValues.ssid);
      WiFi.begin(SettingsValues.ssid.c_str(), SettingsValues.password.c_str());
      while(WiFi.status() != WL_CONNECTED);
    }
    SerialHandler::handle();
    Serial.print(".");
    delay(500);
  }

  delay(500);
  WiFi.setHostname("SSHProject");
  if (!MDNS.begin("SSHProject")) {
    while (1) {
      delay(1000);
    }
  }
  // Add service to MDNS-SD
  //MDNS.addService("http", "tcp", 80);
  SettingsValues.IP = WiFi.localIP().toString();
  LCDHandler::switchPage(2);
}



void TempHandler() {

  if(boilerSensor.tempDouble() == -127.0 || collectorSensor.tempDouble() == -127.0){
    pumps[0].off();
    return;
  }

  if (autoMode) {
    if (boilerSensor.tempDouble() < SettingsValues.tbmax &&
        ((collectorSensor.tempDouble() - boilerSensor.tempDouble() >=
          SettingsValues.tdiffmin &&
          collectorSensor.tempDouble() >= SettingsValues.tkmin) ||
         collectorSensor.tempDouble() > SettingsValues.tkmax)) {
      if (!pumps[0].isOperating())
        pumps[0].on();
    }

    else if (pumps[0].isOperating())
      pumps[0].off();
  } else {
    if (collectorSensor.tempDouble() > SettingsValues.tkmax &&
        boilerSensor.tempDouble() < SettingsValues.tbmax) {
      if (!pumps[0].isOperating())
        pumps[0].on();
    }

    // else if (pumps[0].isOperating()) pumps[0].off();
  }
  if (antiFreezeActivated) {
    antiFreezeActivated = !antiFreezeActivated;
    pumps[0].off();
  }
  if (collectorSensor.tempDouble() <= antiFreezeTemperature &&
      collectorSensor.tempDouble() != -127.0 &&
      !antiFreezeActivated) {
    antiFreezeActivated = true;
    pumps[0].on();
  }
}

void statisticshandler() {
  if (roomPressure > statisticsValues.roomMaxPressure)
    statisticsValues.roomMaxPressure = roomPressure;
  if (roomPressure < statisticsValues.roomMinPressure)
    statisticsValues.roomMinPressure = roomPressure;
  if (roomHumidity > statisticsValues.roomMaxHumidity)
    statisticsValues.roomMaxHumidity = roomHumidity;
  if (roomHumidity < statisticsValues.roomMinHumidity)
    statisticsValues.roomMinHumidity = roomHumidity;
  if (roomTemp > statisticsValues.roomMaxTemp)
    statisticsValues.roomMaxTemp = roomTemp;
  if (roomTemp < statisticsValues.roomMinTemp)
    statisticsValues.roomMinTemp = roomTemp;
}

void resetStatistics() {
  collectorSensor.resetStatistics();
  boilerSensor.resetStatistics();
  t1Sensor.resetStatistics();
  t2Sensor.resetStatistics();
  statisticsValues.roomMinPressure = roomPressure;
  statisticsValues.roomMaxPressure = roomPressure;
  statisticsValues.roomMinHumidity = roomHumidity;
  statisticsValues.roomMaxHumidity = roomHumidity;
  statisticsValues.roomMinTemp = roomTemp;
  statisticsValues.roomMaxTemp = roomTemp;
}

double bmeAtSealevel(double pressure) {
  return pressure / pow((1.0 - (float)SettingsValues.altitude / 44330.0), 5.255);
}

void BMEUpdate() {
  BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
  BME280::PresUnit presUnit(BME280::PresUnit_Pa);

  // bme.read(roomPressure, roomTemp, roomHumidity, tempUnit, presUnit);
  roomPressure = bme.pres();
  roomTemp = bme.temp();
  roomHumidity = bme.hum();
  roomPressure = bmeAtSealevel(roomPressure);
}

void TempUpdate() {
  boilerSensor.updateTemp();
  collectorSensor.updateTemp();
  t1Sensor.updateTemp();
  t2Sensor.updateTemp();
  BMEUpdate();
}



void sensorUpdate() {

  //disable interupts
  //portDISABLE_INTERRUPTS();

  now = rtc.now();
  TempUpdate();
  lastSensorsUpdate = millis();
  statisticshandler();

  //reenable interupts
  //portENABLE_INTERRUPTS();
}


void ledSetup() {
}

void setColor(int red = 0, int green = 0, int blue = 0) {
  // analogWrite(redPin, map(red, 0, 255, 255, 0));
  // analogWrite(greenPin, map(green, 0, 255, 255, 0));
  // analogWrite(bluePin, map(blue, 0, 255, 255, 0));
}

void ledHandler() {
  switch (problemID) {
    case 0:
      setColor(0, 255); // green
      break;
    case 1:
      setColor(255);
      break;
  }
}


void sendToThingspeak() {
  if (client.connect(thingspeak, 80)) { //   "184.106.153.149" or api.thingspeak.com
    Serial.println(apiKey);
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(boilerSensor.tempDouble());
    postStr += "&field2=";
    postStr += String(collectorSensor.tempDouble());
    postStr += "&field3=";
    postStr += String(roomTemp);
    postStr += "&field4=";
    postStr += String(roomHumidity);
    postStr += "&field5=";
    postStr += String(roomPressure);
    postStr += "\r\n\r\n";
    client.print("POST /update HTTP/1.1\n");
     client.print("Host: api.thingspeak.com\n");
     client.print("Connection: close\n");
     client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
    Serial.println(client.readString());
    client.stop();
  }
}

static void updateStatusPage(){
  LCDHandler::changeText("statusPage.boilerTemp", (boilerSensor.tempDouble() != -127.0) ? String(boilerSensor.tempDouble())+(char)176+"C" : "N/A" );
  LCDHandler::changeText("statusPage.collectorTemp", (collectorSensor.tempDouble() != -127.0) ? String(collectorSensor.tempDouble())+(char)176+"C" : "N/A" );
  LCDHandler::changeText("statusPage.t1Temp", (t1Sensor.tempDouble() != -127.0) ? String(t1Sensor.tempDouble())+(char)176+"C" : "N/A" );
  LCDHandler::changeText("statusPage.t2Temp", (t2Sensor.tempDouble() != -127.0) ? String(t2Sensor.tempDouble())+(char)176+"C" : "N/A" );
  LCDHandler::changeText("statusPage.operatingTime", String(pumps[0].operatingTime("%Hh %Mm")));
  LCDHandler::changeText("statusPage.IP", WiFi.localIP().toString());
  LCDHandler::changeText("statusPage2.roomTemp", String(roomTemp)+(char)176+"C");
  LCDHandler::changeText("statusPage2.roomHum",  String(roomHumidity) + " %");
  LCDHandler::changeText("statusPage2.roomPress", String(roomPressure) + " mBar");
}
