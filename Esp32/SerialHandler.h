class SerialHandler {
  public:
    static void handle() {
      String input = "";
      while (Serial1.available()) {
        input = Serial1.readStringUntil(';');
        Serial.println(input);
        String cmd = input.substring(0, input.indexOf('('));
        String args = input.substring(input.indexOf('(') + 1, input.length() - 1);

        if (cmd.indexOf("getSettings") > 0 || cmd == "getSettings") {

          LCDHandler::changeText("settingsPage.minTDiff", (String)SettingsValues.tdiffmin);
          LCDHandler::changeText("settingsPage.maxCT", (String)SettingsValues.tkmax);
          LCDHandler::changeText("settingsPage.minCT", (String)SettingsValues.tkmin);
          LCDHandler::changeText("settingsPage.maxBT", (String)SettingsValues.tbmax);
          LCDHandler::changeText("settingsPage.altitude", (String)SettingsValues.altitude);
        }

        if (cmd.indexOf("settingsSaved") > 0 || cmd == "settingsSaved") {//testing
          SettingsValues.tdiffmin = LCDHandler::getIntValue("settingsPage.minTDiff");
          SettingsValues.tdiffmininput = SettingsValues.tdiffmin;
         
          SettingsValues.tkmax = LCDHandler::getIntValue("settingsPage.maxCT");
          SettingsValues.tkmaxinput = SettingsValues.tkmax;
          
          SettingsValues.tkmin = LCDHandler::getIntValue("settingsPage.minCT");
          SettingsValues.tkmininput = SettingsValues.tkmin;
          
          SettingsValues.tbmax = LCDHandler::getIntValue("settingsPage.maxBT");
          SettingsValues.tbmaxinput = SettingsValues.tbmax;
         
          SettingsValues.altitude = LCDHandler::getIntValue("settingsPage.altitude");
          SettingsValues.altitudeinput = SettingsValues.altitude;
        }

        if (cmd.indexOf("getWIFIAPS") > 0 || cmd == "getWIFIAPS") LCDHandler::loadWIFIAPS();

        if (cmd.indexOf("connectWifi") > 0 || cmd == "connectWifi") {

          String ssid = args.substring(0, args.indexOf(','));
          String pass = args.substring(args.indexOf(',') + 1, args.length());

          Serial.println(ssid + "\t" + pass);
          WiFi.begin(ssid.c_str(), pass.c_str());
          while(WiFi.status() != WL_CONNECTED);
        }

      }
    }

    static void response() {
      while (Serial1.available()) {
        String input = "";
        input = Serial1.readString();
      }
    }
};
