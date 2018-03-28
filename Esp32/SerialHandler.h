class SerialHandler {
  public:
    static void handle() {
      String input = "";
      while (Serial1.available()) {
        input = Serial1.readStringUntil();
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

        if (cmd == "getWIFIAPS") LCDHandler::loadWIFIAPS();

        if (cmd == "pconnectWifi") {

          String ssid = args.substring(0, args.indexOf(','));
          String pass = args.substring(args.indexOf(',') + 1, args.length() - 1);
          //char ssid[ssidString.length()];
          //char pass[passString.length()];

          //ssidString.toCharArray(ssid, ssidString.length());
          //passString.toCharArray(pass, passString.length());
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
