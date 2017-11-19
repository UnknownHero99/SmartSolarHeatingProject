
String getApiData(){
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
  ApiContent += "\n\t\t\"tdiffmin\":" + String(SettingsValues.tdiffmin);
  ApiContent += ",\n\t\t\"tkmax\":" + String(SettingsValues.tkmax);
  ApiContent += ",\n\t\t\"tkmin\":" + String(SettingsValues.tkmin);
  ApiContent += ",\n\t\t\"tbmax\":" + String(SettingsValues.tbmax);
  ApiContent += ",\n\t\t\"altitude\":" + String(SettingsValues.altitude);
  ApiContent += "\n\t}";
  ApiContent += "\n}";
  return ApiContent;
}
