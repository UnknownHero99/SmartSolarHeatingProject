#include <Arduino.h>

/*Libraries*/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <WiFiManager.h>
#include <ESP8266httpUpdate.h>
#include <EEPROM.h>
#include "FS.h"

#define releaseVersion "1.0"

#include "global.h"
#include "SerialHandler.h"
#include "WebServerHandler.h"


void setup(void) {
  Serial.begin(115200);

  //begin SPIFFS and read data from it
  SPIFFSInitReadData();
  
  //connect to WIFI
  wifiConnect();
  //init webserver
  WebServerHandler::initWebserver();
}

void loop(void) {
  SerialHandler::handle();
  WebServerHandler::handleClient();
}

