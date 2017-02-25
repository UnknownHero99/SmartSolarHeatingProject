#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>

const char* ssid = ""; //change with wifi SSID
const char* password = "";//change with wifi password
String apiKey = "";//replace with thingspeak api
const char* loginUsername = "admin";
const char* loginPassword = "admin";

unsigned long updateInterval = 600000;
unsigned long lastUpdate = 0;


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
#include "API.h"

void setup(void) {
	Serial.begin(115200);
	while (ardData.tempcollector != 0) { //wait until get data;
		serialHandler();
	}
	WiFi.begin(ssid, password);
	// Wait for connection
	while (WiFi.status() != WL_CONNECTED) {
	delay(500);
	}
	if (mdns.begin("esp8266", WiFi.localIP())) {
	}
	server.on("/", handleStatus);
	server.on("/status/data", send_system_status_data);

	server.on("/api", send_system_api_data);
	server.on("/api/set", set_system_api_data);
	server.on("/api/login", api_handleLogin);

	server.on("/login", handleLogin);

	server.on("/settings", handleSettings);
	server.on("/settings/data", send_system_settings_data);

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
}
void loop(void) {
	if (millis() - lastUpdate >= updateInterval) update();
	serialHandler();
	server.handleClient();
}
