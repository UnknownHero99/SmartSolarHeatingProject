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

String head = "<!DOCTYPE html><html><head><title>SolarnoGretje</title><meta http-equiv='Content-Type' content='text/html; charset=UTF8'> <link rel='stylesheet' href='http://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.6.3/css/font-awesome.min.css'><link href='https://fonts.googleapis.com/css?family=Aclonica|Tangerine|Neuton|Noto+Serif|Paytone+One|Oswald|Sacramento|Roboto|Open+Sans|Montserrat' rel='stylesheet' type='text/css'><style>.clicked a,a{color:#4CAF50}a,p{font-family:Montserrat}.button,a,li a{font-weight:700;text-decoration:none}#wrapper,body{min-height:100%}#header{position:fixed;height:160px}#menu,#title{height:80px}body,p{margin:0}#title{width:250px;margin:0 auto;}.clicked{background-color:#5F5F5F!important}h1,h2,h3,h4,p{color:#fff}p{font-size:15px}a{font-size:20px}.button,h1,h2,h3,h4{font-family:Noto Serif,serif}body,html{height:100%;width:100%;margin:0;padding:0}body{background-image:url(http://www.technocrazed.com/wp-content/uploads/2015/12/Blue-Wallpaper-For-Background-1.jpg)}.button{background-color:#5F5F5F;border:none;color:#4CAF50;padding:15px 32px;text-align:center;display:inline-block;font-size:16px;margin:4px 2px;cursor:pointer}input,ul{margin:0}.button:hover{background-color:#fcd450;color:#626262}#footer,#header{background-color:#00b8ff;width:100%}input{color:#626262;font-family:Montserrat;font-size:15px}#wrapper{overflow:auto;position:relative}.shadow{box-shadow:0 2px 4px 0 rgba(0,0,0,.16),0 2px 10px 0 rgba(0,0,0,.12)!important}#menu{position:absolute;right:10%;bottom:0}ul{transform:rotate(180deg);list-style-type:none;padding:0;height:100%;overflow:hidden}li{float:right}li a{transform:rotate(-180deg);display:block;padding:30px 30px 20px;color:#5F5F5F;font-family:noto-serif;font-size:30px}li:hover{background-color:#5F5F5F}li:hover a{color:#4CAF50}#content_Wrapper{width:50%;min-width:600px;margin:0 auto;padding-top:160px;text-align:center;padding-bottom:80px}#content{padding-top:50px;padding-bottom:100px;font-size:20px;background-color:#009bd6;box-shadow:3px 2px 100px 20px rgba(0,0,0,.75)}#footer{position:absolute;bottom:0;left:0;height:100px;box-shadow:0 -2px 8px 0 rgba(0,0,0,.16),0 -2px 10px 0 rgba(0,0,0,.12)!important;font-family:Neuton}#copyright{padding-top:40px;padding-left:50px}#copyright p{color:#fff;font-family:Noto Serif,serif;font-size:20px}</style></head><body><div id='wrapper'><div id='header' class='shadow'><div id='title'><h1>Solarno Gretje</h1></div>";
String menulogedin = "<div id='menu'><ul><li><a href='/'>Status</a></li><li><a href='/pumps'>Pumpe</a></li><li><a href='/settings'>Nastavitve</a></li><li><a href='/graphs'>Grafi</a></li><li><a href='/login?DISCONNECT=YES'>Odjava</a></li></ul></div></div><div id='content_Wrapper'><div id='content'>";
String menunotlogedin = "<div id='menu'><ul><li><a href='/'>Status</a></li><li><a href='/login'>Prijava</a></li></ul></div></div><div id='content_Wrapper'><div id='content'>";
String footer = "<div id='footer'><div id='copyright'><p>&copyJakob Salmič</p></div></div></div></body></html>";

String arduinoData[14];
String arduinoSettings[5];
String Data = "";
String Settings = "";
String pumps = "";


void setup(void) {
	Serial.begin(115200);
	WiFi.begin(ssid, password);


	// Wait for connection
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
	}
	if (mdns.begin("esp8266", WiFi.localIP())) {

	}

	server.on("/", handleRoot);
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
