const char* thingspeak = "api.thingspeak.com";
MDNSResponder mdns;
ESP8266WebServer server(80);
WiFiClient client;
//replace with url to thingspeak graphs
String graph1 = "https://thingspeak.com/channels/132705/charts/1?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line";
String graph2 = "https://thingspeak.com/channels/132705/charts/2?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line";
String graph3 = "https://thingspeak.com/channels/132705/charts/3?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line";
String graph4 = "https://thingspeak.com/channels/132705/charts/4?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line";
String graph5 = "https://thingspeak.com/channels/132705/charts/5?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line";
String graph6 = "https://thingspeak.com/channels/132705/maps/channel_show"; //map

struct arduinoData {
	bool pump1operating = false;
	String pump1Status = "";
	String pump2Status = "";
	String pump3Status = "";
  String pump4Status = "";
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

struct arduinoSettings {
	int tdiffmin = 0;
	int tdiffmininput = 0;
	int tkmax = 0;
	int tkmaxinput = 0;
	int tkmin = 0;
	int tkmininput = 0;
	int tbmax = 0;
	int tbmaxinput = 0;
	int altitude = 0;
	int altitudeinput = 0;
}   ardSettings;

bool settingsUpdate(String settings) {
  char json[settings.length() + 1];
  settings.toCharArray(json, settings.length() + 1);
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  if (!root.success()) return false;

  ardSettings.tdiffmin = root["mTD"];
  ardSettings.tdiffmininput = ardSettings.tdiffmin;

  ardSettings.tkmax = root["maxTC"];
  ardSettings.tkmaxinput = ardSettings.tkmax;

  ardSettings.tkmin = root["minTC"];
  ardSettings.tkmininput = ardSettings.tkmin;

  ardSettings.tbmax = root["mTB"];
  ardSettings.tbmaxinput = ardSettings.tbmax;

  ardSettings.altitude = root["a"];
  ardSettings.altitudeinput = ardSettings.altitude;

  return true;
}

bool pumpUpdate(String pumps) {
  char json[pumps.length() + 1];
  pumps.toCharArray(json, pumps.length() + 1);
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  if (!root.success()) return false;

  ardData.pump1operating = root["p1O"];
  String pump1Status = root["p1S"];
  ardData.pump1Status = pump1Status;
  String pump2Status = root["p2S"];
  ardData.pump2Status = pump2Status;
  String pump3Status = root["p3S"];
  ardData.pump3Status = pump3Status;
  String pump4Status = root["p4S"];
  ardData.pump4Status = pump4Status;
  return true;
}

bool dataUpdate(String data) {
  char json[data.length() + 1];
  data.toCharArray(json, data.length() + 1);
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  if (!root.success()) return false;
  
  ardData.pump1operating = root["p1O"];
  String pump1Status = root["p1S"];
  ardData.pump1Status = pump1Status;
  ardData.operatinghours = root["oTH"];
  ardData.operatingminutes = root["oTM"];
  ardData.tempboiler = root["bT"];
  ardData.tempcollector = root["cT"];
  ardData.temproom = root["rT"];
  ardData.humidityroom = root["rH"];
  ardData.tempt1 = root["t1T"];
  ardData.tempt2 = root["t2T"];
  ardData.pressureroom = root["rP"];
  return true;
}

void serialHandler() {
	String input = "";
	while (Serial.available()) {
		input = Serial.readStringUntil(';');;
		String cmd = input.substring(0, input.indexOf('('));
		String args = input.substring(input.indexOf('(') + 1, input.length() - 1);

		if (cmd == "GetIP") {
			Serial.println("IP(" + IP + ");");
		}

		else if (cmd == "ThingSpeak") {
				if (client.connect(thingspeak, 80)) {  //   "184.106.153.149" or api.thingspeak.com
					String postStr = apiKey;
					postStr += "&field1=";
					postStr += String(ardData.tempboiler);
					postStr += "&field2=";
					postStr += String(ardData.tempcollector);
					postStr += "&field3=";
					postStr += String(ardData.temproom);
					postStr += "&field4=";
					postStr += String(ardData.humidityroom);
					postStr += "&field5=";
					postStr += String(ardData.pressureroom);
					postStr += "\r\n\r\n";
					client.print("POST /update HTTP/1.1\n");
					client.print("Host: api.thingspeak.com\n");
					client.print("Connection: close\n");
					client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
					client.print("Content-Type: application/x-www-form-urlencoded\n");
					client.print("Content-Length: ");
					client.print(postStr.length());
					client.print("\n\n");
					client.print(postStr);
					client.stop();
				}
				
		}
		else if (cmd == "Settings") {
			settingsUpdate(args);
		}
		else if (cmd == "Data") {
			dataUpdate(args);
		}
		else if (cmd == "PumpStatus") {
			pumpUpdate(args);
		}
	}
}

void requestData(){
  Serial.print("GetData();");
}

void requestSettings(){
  Serial.print("GetSettings();");
}
void requestPumps(){
  Serial.print("GetPumps();");
}

void requestAll() {
	requestData();
  requestSettings();
  requestPumps();
	lastUpdate = millis();
}

