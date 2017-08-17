#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>
#include <WiFiManager.h>
#include <ESP8266httpUpdate.h>
#include <EEPROM.h>
#include "FS.h"

String apiKey;//replace with thingspeak api
String thingspeakChannelID ;
const char* loginUsername = "admin";
String loginPassword = "admin";

const int bufferSize = 6000;
uint8_t _buffer[6000];

unsigned long updateInterval = 60000;
const unsigned long noDataRecivedInterval = 2000;
unsigned long lastUpdate = 0;
String IP = "";//leave this there will be saved IP of esp

#include "global.h"
#include "Page_Functions.h"
#include "Page_Constants.h"
#include "Page_MicroAjax.js.h"
#include "Page_Status.h"
#include "Page_Settings.h"
#include "Page_Login.h"
#include "Page_Pumps.h"
#include "Page_Graphs.h"
#include "Page_404NotFound.h"
#include "Page_Update.h"
#include "API.h"
void setup(void) {
	Serial.begin(115200);
  SPIFFS.begin();
  WiFiManager wifiManager;
  wifiManager.autoConnect("SmartSolarHeatingProject");

	// Wait for connection
	while (WiFi.status() != WL_CONNECTED) {
	delay(500);
	}
	if (mdns.begin("esp8266", WiFi.localIP())) {
	}
  unsigned long lastRequest = millis();

	IP = WiFi.localIP().toString();
	server.on("/", handleStatus);
	server.on("/status/data", send_system_status_data);
 
	server.on("/api", send_system_api_data);
	server.on("/api/set", set_system_api_data);
	server.on("/api/login", api_handleLogin);

	server.on("/login", handleLogin);

	server.on("/settings", handleSettings);
	server.on("/settings/data", send_system_settings_data);
  server.on("/settings/ota", handleSystemUpdate);
  
	server.on("/microajax.js", []() { server.send(200, "text/plain", PAGE_microajax_js);  });

	server.on("/pumps", handlePumps);
	server.on("/pumps/data", send_system_pumps_data);

	server.on("/graphs", handleGraphs);
	server.on("/graphs/data", send_system_graphs_data);
  
	
	server.onNotFound(handleNotFound);

	//here the list of headers to be recorded
	const char * headerkeys[] = { "User-Agent", "Cookie" };
	size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);
	//ask server to track these headers
	server.collectHeaders(headerkeys, headerkeyssize);
	server.begin();
  File f = SPIFFS.open("/data.txt", "r");
  loginPassword = f.readStringUntil('|');
  if (loginPassword == "") loginPassword = "admin";
  thingspeakChannelID = f.readStringUntil('|');
  apiKey = f.readStringUntil('|');
  f.close();
}
void loop(void) {
	if (millis() - lastUpdate >= updateInterval) requestAll();
	serialHandler();
	server.handleClient();
}
