#include <Arduino.h>

/*Libraries*/
#include <WiFi.h>          
#include <WebServer.h>
#include <WiFiManager.h> 
#include <SPIFFS.h>  
    
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include "FS.h"

const String releaseVersion = "1.0";

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
