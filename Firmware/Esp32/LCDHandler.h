class LCDHandler {
  private:
    static void sendCommand(String command) {

      Serial1.print(command);
      /*    Serial.print(command);
          Serial.write(0xFF);
          Serial.write(0xFF);
          Serial.write(0xFF);
          Serial.flush();*/
    }
    static void endCommand() {
      Serial1.write(0xFF);
      Serial1.write(0xFF);
      Serial1.write(0xFF);
      Serial1.flush();
    }
  public:
    static void switchPage(int id) {
      String command = "page " + String(id);
      sendCommand(command);
      endCommand();
    }

    static void changeText(String element, String text) {
      String command = element + ".txt=\"" + text + "\"";
      sendCommand(command);
      endCommand();
    }

    static void wake() {
      sendCommand("sleep=0");
      endCommand();
    }

    static void setVisibility(String element, bool visible) {
      String command = "vis " + element + "," + String(visible);
      Serial.println(command);
      sendCommand(command);
      endCommand();
    }

    static int getIntValue(String element) {
      sendCommand("print " + element + ".txt");
      endCommand();
      while (!Serial1.available());
      String value = Serial1.readString();
      return value.toInt();
    }

    static void loadWIFIAPS() {
      int numberOfAPs =  WiFi.scanNetworks();
      int numberIfSameSSIDs = 0;
      for (int i = 0; i < numberOfAPs && i < 5; i++) {
        boolean InList = false;
        for (int j = 0; j < i; j++) {
          if ((String(WiFi.SSID(j)) == String(WiFi.SSID(i)))) {
            InList = true;
            numberIfSameSSIDs++;
          }
        }
        if (!InList) {
          LCDHandler::changeText("WifiConn.b" + String(i - numberIfSameSSIDs), String(WiFi.SSID(i)));
          LCDHandler::setVisibility("WifiConn.b" + String(i - numberIfSameSSIDs), true);
        }
      }
    }
};
