class SerialHandler {
  public:
    static void handle() {
      String input = "";
      while (Serial1.available()) {
        input = Serial1.readStringUntil(0xFF);
        Serial.println(input);
        String cmd = input.substring(0, input.indexOf('('));
        String args = input.substring(input.indexOf('(') + 1, input.length() - 1);

        if (cmd == "getSettings") {
          LCDHandler::changeText("settingsPage.minTDiff", (String)SettingsValues.tdiffmin);
          LCDHandler::changeText("settingsPage.maxCT", (String)SettingsValues.tkmax);
          LCDHandler::changeText("settingsPage.minCT", (String)SettingsValues.tkmin);
          LCDHandler::changeText("settingsPage.maxBT", (String)SettingsValues.tbmax);
          LCDHandler::changeText("settingsPage.altitude", (String)SettingsValues.altitude);
        }

        if (cmd == "settingsSaved") {//testing
          //SettingsValues.tdiffmin =
          Serial.println(LCDHandler::getIntValue("settingsPage.minTDiff"));
          delay(500);
          //SettingsValues.tkmax =
          LCDHandler::getIntValue("settingsPage.maxCT");
          //SettingsValues.tkmin =
          LCDHandler::getIntValue("settingsPage.minCT");
          //SettingsValues.tbmax =
          LCDHandler::getIntValue("settingsPage.maxBT");
          //SettingsValues.altitude =
          LCDHandler::getIntValue("settingsPage.altitude");
        }
      }
    }

    static void response(){
      while(Serial1.available()){
        String input = "";
        input = Serial1.readString();
      }
    }
};
