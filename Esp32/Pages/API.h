
String getApiData(){
  String ApiContent = "{\n\t\"data\": {";
  ApiContent += "\n\t\t\"pump1operating\":" + String(pumps[0].isOperating());

  ApiContent += ",\n\t\t\"pump1Status\":";
  if(autoMode) ApiContent += "A";
  else if(pumps[0].isOperating())ApiContent += "On";
  else ApiContent += "Off";

  ApiContent += ",\n\t\t\"pump2Status\":";
  if(pumps[1].isOperating())ApiContent += "On";
  else ApiContent += "Off";

  ApiContent += ",\n\t\t\"pump3Status\":";
  if(pumps[2].isOperating())ApiContent += "On";
  else ApiContent += "Off";

  ApiContent += ",\n\t\t\"pump4Status\":";
  if(pumps[3].isOperating())ApiContent += "On";
  else ApiContent += "Off";

  ApiContent += ",\n\t\t\"pumpautomode\":";
  if (!autoMode) ApiContent += "OFF";
  else ApiContent += "ON";

  ApiContent += ",\n\t\t\"operatinghours\":" + String(pumps[0].operatingTime("%H"));
  ApiContent += ",\n\t\t\"operatingminutes\":" + String(pumps[0].operatingTime("%M"));
  ApiContent += ",\n\t\t\"tempcollector\":" + String(collectorSensor.tempDouble());
  ApiContent += ",\n\t\t\"tempboiler\":" + String(boilerSensor.tempDouble());
  ApiContent += ",\n\t\t\"tempt1\":" + String(t1Sensor.tempDouble());
  ApiContent += ",\n\t\t\"tempt2\":" + String(t2Sensor.tempDouble());
  ApiContent += ",\n\t\t\"temproom\":" + String(roomTemp);
  ApiContent += ",\n\t\t\"humidityroom\":" + String(roomHumidity);
  ApiContent += ",\n\t\t\"pressureroom\":" + String(roomPressure);
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
