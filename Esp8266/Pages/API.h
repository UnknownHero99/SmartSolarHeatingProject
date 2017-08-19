void send_system_api_data()
{
  unsigned long lastRequest = millis();
  SerialHandler::requestAll();
  while (!Serial.available()) {
    if (millis() - lastRequest >= noDataRecivedInterval) {
      SerialHandler::requestAll();
      lastRequest = millis();
    }
  }
  SerialHandler::handle();
  String ApiContent = "{\n\t\"data\": {";
  ApiContent += "\n\t\t\"pump1operating\":" + String(ardData.pump1operating);
  ApiContent += ",\n\t\t\"pump1Status\":" + String(ardData.pump1Status);
  ApiContent += ",\n\t\t\"pump2Status\":" + String(ardData.pump2Status);
  ApiContent += ",\n\t\t\"pump3Status\":" + String(ardData.pump3Status);
  ApiContent += ",\n\t\t\"pump4Status\":" + String(ardData.pump4Status);
  ApiContent += ",\n\t\t\"pumpautomode\":";
  if (ardData.pump1Status != "A") ApiContent += "OFF";
  else ApiContent += "ON";
  ApiContent += ",\n\t\t\"operatinghours\":" + String(ardData.operatinghours);
  ApiContent += ",\n\t\t\"operatingminutes\":" + String(ardData.operatingminutes);
  ApiContent += ",\n\t\t\"tempcollector\":" + String(ardData.tempcollector);
  ApiContent += ",\n\t\t\"tempboiler\":" + String(ardData.tempboiler);
  ApiContent += ",\n\t\t\"tempt1\":" + String(ardData.tempt1);
  ApiContent += ",\n\t\t\"tempt2\":" + String(ardData.tempt2);
  ApiContent += ",\n\t\t\"temproom\":" + String(ardData.temproom);
  ApiContent += ",\n\t\t\"humidityroom\":" + String(ardData.humidityroom);
  ApiContent += ",\n\t\t\"pressureroom\":" + String(ardData.pressureroom);
  ApiContent += "\n\t},";
  ApiContent += "\n\t\"settings\": {";
  ApiContent += "\n\t\t\"tdiffmin\":" + String(ardSettings.tdiffmin);
  ApiContent += ",\n\t\t\"tkmax\":" + String(ardSettings.tkmax);
  ApiContent += ",\n\t\t\"tkmin\":" + String(ardSettings.tkmin);
  ApiContent += ",\n\t\t\"tbmax\":" + String(ardSettings.tbmax);
  ApiContent += ",\n\t\t\"altitude\":" + String(ardSettings.altitude);
  ApiContent += "\n\t}";
  ApiContent += "\n}";
  server.send(200, "application/json", ApiContent);
}

void set_system_api_data()
{
  handleSettings();
}

void api_handleLogin() {
  if (server.hasArg("USERNAME") && server.hasArg("PASSWORD")) {
    if (server.arg("USERNAME") == loginUsername &&  server.arg("PASSWORD") == loginPassword) {
      String header = "HTTP/1.1 200 OK\r\nSet-Cookie: ESPSESSIONID=1\r\nLocation: /\r\nCache-Control: no-cache\r\n\r\n";
      server.sendContent(header);
    }
  }
}
