#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>



const char* ssid = "";
const char* password = "";
String apiKey = "";//replace with thingspeak api
const char* loginUsername = "admin";
const char* loginPassword = "admin";

unsigned long updateInterval = 600000;
unsigned long lastUpdate = 0;

const char* thingspeak = "api.thingspeak.com";

MDNSResponder mdns;
ESP8266WebServer server(80);
WiFiClient client;



//replace with url to thingspeak graphs
String graph1 = "https://thingspeak.com/channels/132705/charts/1?bgcolor=%23ffffff&color=%23d62020&days=1&dynamic=true&max=120&min=-10&results=60&title=Temperatura+bojlerja&type=line&xaxis=%C4%8Cas&yaxis=Temperatura+%28%C2%B0C%29";
String graph2 = "https://thingspeak.com/channels/132705/charts/2?bgcolor=%23ffffff&color=%23d62020&days=1&dynamic=true&max=120&min=-10&results=60&title=Temperatura+kolektorja&type=line&xaxis=%C4%8Cas&yaxis=Temperatura+%28%C2%B0C%29";
String graph3 = "https://thingspeak.com/channels/132705/charts/3?bgcolor=%23ffffff&color=%23d62020&days=7&dynamic=true&max=50&min=-20&results=60&title=Temperatura+prostora&type=line&xaxis=%C4%8Cas&yaxis=Temperatura+%28%C2%B0C%29";
String graph4 = "https://thingspeak.com/channels/132705/charts/4?bgcolor=%23ffffff&color=%23d62020&dynamic=true&max=100&min=10&results=60&title=Vlaga+prostora&type=line&xaxis=%C4%8Cas&yaxis=Vlaga+%28%25%29";
String graph5 = "https://thingspeak.com/channels/132705/charts/5?bgcolor=%23ffffff&color=%23d62020&days=7&dynamic=true&max=1100&min=900&results=60&title=Barometer&type=line&xaxis=%C4%8Cas&yaxis=Zra%C4%8Dni+tlak+%28mBa%29";
String graph6 = "https://thingspeak.com/channels/132705/maps/channel_show"; //map

String head = "";
String menulogedin = "<div id='menu'><ul><li><a href='/'>Status</a></li><li><a href='/pumps'>Pumpe</a></li><li><a href='/settings'>Nastavitve</a></li><li><a href='/graphs'>Grafi</a></li><li><a href='/login?DISCONNECT=YES'>Odjava</a></li></ul></div></div><div id='content_Wrapper'><div id='content'>";
String menunotlogedin = "<div id='menu'><ul><li><a href='/'>Status</a></li><li><a href='/login'>Prijava</a></li></ul></div></div><div id='content_Wrapper'><div id='content'>";
String footer = "<div id='footer'><div id='copyright'><p>&copyJakob Salmič</p></div></div></div></body></html>";

String arduinoData[15];
String arduinoSettings[5];
String Data = "";
String Settings = "";
String pumps = "";


struct arduinoData {
	bool pumpstatus = false;
	bool  pumpautomode = false;
	int operatinghours = 0;
	int  operatingminutes = 0;
	double tempcollector = 0;
	double tempboiler = 0;
	double tempt1 = 0;
	double tempt2 = 0;
	double temproom = 0;
	double humidityroom = 0;
	double pressureroom = 0;
}   ardData;

#include "Page_Constants.h"
#include "Page_MicroAjax.js.h"
#include "Page_Status.h"

void setup(void) {
	Serial.begin(115200);
	WiFi.begin(ssid, password);


	// Wait for connection
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
	}
	if (mdns.begin("esp8266", WiFi.localIP())) {

	}

	server.on("/", handleStatus);
	server.on("/status/data", send_system_status_data);
	server.on("/microajax.js", []() { server.send(200, "text/plain", PAGE_microajax_js);  });
	server.on("/pumps", handlePumps);
	server.on("/graphs", handleGraphs);
	server.on("/settings", handleSettings);
	server.on("/login", handleLogin);

	server.onNotFound(handleNotFound);
	//here the list of headers to be recorded
	const char * headerkeys[] = { "User-Agent", "Cookie" };
	size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);
	//ask server to track these headers
	server.collectHeaders(headerkeys, headerkeyssize);
	server.begin();
	update();
}

void loop(void) {
	if (millis() - lastUpdate >= updateInterval) update();
	serialHandler();
	server.handleClient();
}
