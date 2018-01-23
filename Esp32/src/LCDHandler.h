class LCDHandler {
private:
  static void sendCommand(String command){

    Serial1.print(command);
/*    Serial.print(command);
    Serial.write(0xFF);
    Serial.write(0xFF);
    Serial.write(0xFF);
    Serial.flush();*/
  }
  static void endCommand(){
    Serial1.write(0xFF);
    Serial1.write(0xFF);
    Serial1.write(0xFF);
    Serial1.flush();
  }
public:
  static void switchPage(int id){
    String command = "page " + String(id);
    sendCommand(command);
    endCommand();
  }

  static void changeText(String element, String text){
    String command = element + ".txt=\"" + text + "\"";
    sendCommand(command);
    endCommand();
  }

  static void updateStatusPage(){
    changeText("boilerTemp", (boilerSensor.tempDouble() != -127.0) ? String(boilerSensor.tempDouble())+(char)176+"C" : "N/A" );
    changeText("collectorTemp", (collectorSensor.tempDouble() != -127.0) ? String(collectorSensor.tempDouble())+(char)176+"C" : "N/A" );
    changeText("t1Temp", (t1Sensor.tempDouble() != -127.0) ? String(t1Sensor.tempDouble())+(char)176+"C" : "N/A" );
    changeText("t2Temp", (t2Sensor.tempDouble() != -127.0) ? String(t2Sensor.tempDouble())+(char)176+"C" : "N/A" );
    changeText("operatingTime", String(pumps[0].operatingTime("%Hh %Mm")));
  }

  static String getIntValue(String element){
    String command = "print \"" + element + "(\n"+element+".txt\n);\"";
    Serial.println(command);
    sendCommand("print \"" + element + "(\"");
    sendCommand(element+".txt");
    sendCommand("\");\"");
    endCommand();
    while(!Serial1.available());
    String value = Serial1.readString();
    Serial.println(value.substring(4,value.length()-3));
    return value;
  }
};
