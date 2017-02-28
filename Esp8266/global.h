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

  ardSettings.tdiffmin = root["minTempDiff"];
  ardSettings.tdiffmininput = ardSettings.tdiffmin;

  ardSettings.tkmax = root["maxTempCollector"];
  ardSettings.tkmaxinput = ardSettings.tkmax;

  ardSettings.tkmin = root["minTempCollector"];
  ardSettings.tkmininput = ardSettings.tkmin;

  ardSettings.tbmax = root["maxTempBoiler"];
  ardSettings.tbmaxinput = ardSettings.tbmax;

  ardSettings.altitude = root["altitude"];
  ardSettings.altitudeinput = ardSettings.altitude;

  return true;
}

bool pumpUpdate(String pumps) {
  char json[pumps.length() + 1];
  pumps.toCharArray(json, pumps.length() + 1);
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  if (!root.success()) return false;

  ardData.pump1operating = root["pump1Operating"];
  String pump1Status = root["pump1Status"];
  ardData.pump1Status = pump1Status;
  String pump2Status = root["pump2Status"];
  ardData.pump2Status = pump2Status;
  String pump3Status = root["pump3Status"];
  ardData.pump3Status = pump3Status;
  String pump4Status = root["pump4Status"];
  ardData.pump4Status = pump4Status;
  return true;
}

bool dataUpdate(String data) {
  char json[data.length() + 1];
  data.toCharArray(json, data.length() + 1);
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  if (!root.success()) return false;

  ardData.operatinghours = root["operatingTimeHours"];
  ardData.operatingminutes = root["operatingTimeMinutes"];
  ardData.tempboiler = root["boilerTemp"];
  ardData.tempcollector = root["collectorTemp"];
  ardData.temproom = root["roomTemp"];
  ardData.humidityroom = root["roomHumidity"];
  ardData.tempt1 = root["t1Temp"];
  ardData.tempt2 = root["t2Temp"];
  ardData.pressureroom = root["roomPressure"];
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

void update() {
	Serial.print("GetSettings();GetData();GetPumps();");
	lastUpdate = millis();
}

