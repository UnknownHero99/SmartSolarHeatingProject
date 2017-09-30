
const char* thingspeak = "api.thingspeak.com";

WiFiClient client; //needed for sending data to thingspeak

String apiKey;
String thingspeakChannelID;

const char* loginUsername = "admin";
String loginPassword;

const unsigned long noDataRecivedInterval = 2000;
unsigned long lastUpdate = 0;

String IP;

void wifiConnect () {
  WiFiManager wifiManager;
  wifiManager.autoConnect("SmartSolarHeatingProject");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  IP = WiFi.localIP().toString();
}

void SPIFFSInitReadData() {
  SPIFFS.begin();
  File f = SPIFFS.open("/data.txt", "r");
  loginPassword = f.readStringUntil('|');
  if (loginPassword == "") loginPassword = "admin";
  thingspeakChannelID = f.readStringUntil('|');
  apiKey = f.readStringUntil('|');
  f.close();
}

String eepromReadString(int offset, int bytes) {
  char c = 0;
  String string = "";
  EEPROM.begin(32);
  EEPROM.write(1, 233);
  for (int i = 0; i < (bytes); i++) {
    int addr = i;
    addr += EEPROM.get(addr, c);
    EEPROM.commit();
    string += c;
  }

  EEPROM.end();
  return string;
}

void eepromWriteString(int offset, int bytes, String buf) {
  char c = 0;
  //int len = (strlen(buf) < bytes) ? strlen(buf) : bytes;
  EEPROM.begin(512);
  for (int i = 0; i < bytes; i++) {
    c = buf[i];
    i += EEPROM.put(i, (int)c);
    EEPROM.write(offset + i, (int)c);
  }
  EEPROM.end();
}

