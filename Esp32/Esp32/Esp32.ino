#include <Arduino.h>

/*Libraries*/
#include <WiFi.h>

#include "FS.h"
#include <SPIFFS.h>

//#define ASYNC_TCP_SSL_ENABLED 1
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>

#include <ArduinoJson.h>

#include <RTClib.h>
#include <BME280I2C.h>
#include <Wire.h>

#include "PCF8574.h"


PCF8574 PCF_01(0x38);
RTC_DS3231 rtc;
BME280I2C bme;

const String releaseVersion = "1.0";
const char *ssid = "dejavumasaze";
const char *password = "dejavu12";

#include "global.h"
#include "WebServerHandler.h"


void setup(void) {
  Serial.begin(115200);

  latestVersion = releaseVersion;
  Wire.begin(21, 22);
  PCF_01.begin();
  if (!rtc.begin()) {
    //RTC
    Serial.println("Problem with RTC");
  }
  now = rtc.now();

  while (!bme.begin())
  {
    Serial.println("Could not find BME280 sensor!");
    delay(1000);
  }

  // begin SPIFFS and read data from it
  SPIFFSInitReadData();

  // connect to WIFI
  wifiConnect();

  // init webserver
  WebServerHandler::initWebserver();
  sensorUpdate(); //update sensors data
  resetStatistics(); //reset pump operating time statistics
  ledSetup();
  ledHandler(); //run ledhandler
}

void loop(void) {

  //do it every minute
  unsigned long currentMillis = millis();
  if (currentMillis - lastUpdate >= updateInterval) {
    sensorUpdate(); //update sensors data
    TempHandler();
    ledHandler();

    //update pumps operating time
    for (int i = 0; i < 4; i++) {
      pumps[i].updateTime();
    }
    //if disconnected from wifi reconnect
    if (WiFi.status() != WL_CONNECTED) wifiConnect(); // reconnect to WIFI

    //check if statistics needs to be cleared
    if (now.hour() == 0 && now.minute() == 0) {
      pumps[0].resetTime();
      resetStatistics();
    }
    lastUpdate = millis();
  }
  if (currentMillis - lastThingspeak >= thingspeakInterval) {
    sendToThingspeak();
    lastThingspeak = millis();
  }
  /*delay(500);
    setColor(255);
    delay(500);
    setColor(0,255);
    delay(500);
    setColor(0,0,255);
    delay(500);
    setColor(255,255,255);
    delay(500);
    setColor(150,73,150);
    delay(500);
    PCF_01.write(3, LOW);
    delay(500);
    PCF_01.write(3, HIGH);*/
}
