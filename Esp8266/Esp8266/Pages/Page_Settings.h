const char PAGE_settings[] PROGMEM = R"=====(
		<div id="content">
        <article>
          <h1>Nastavitve</h1>
          <form enctype="application/json" method="post">
            <table id="settingsTable" align="center">
              <tbody>
			  <tr>
                <th><h4>Nastavitve Sistema</h4></th>
                <th><h4>Trenutna<br />vrednost</h4></th>
                <th><h4>Nova<br />vrednost</h4></th>
              </tr>
              <tr>
                <td><p>Min temp razlika:</p></td>
                <td><span id="tdiffmin" name="tdiffmin"></span><span>°C</span></td>
                <td><input type="text" id="tdiffmininput" name="tdiffmininput" value="0"></td>
              </tr>
              <tr>
                <td><p>Maksimalna temperatura kolektorja:</p></td>
                <td><span id="tkmax" name="tkmax"></span><span>°C</span></td>
                <td><input type="text" id="tkmaxinput" name="tkmaxinput" value="0"></td>
              </tr>
              <tr>
                <td><p>Minimalna temperatura kolektorja:</p></td>
                <td><span id="tkmin" name="tkmin"></span><span>°C</span></td>
                <td><input type="text" id="tkmininput" name="tkmininput" value="0"></td>
              </tr>
              <tr>
                <td><p>Maksimalna temperatura bojlerja:</p></td>
                <td><span id="tbmax" name="tbmax"></span><span>°C</span></td>
                <td><input type="text" id="tbmaxinput" name="tbmaxinput" value="0"></td>
              </tr>
              <tr>
                <td><p>Nadmorska višina:</p></td>
                <td><span id="altitude" name="altitude"></span><span>m</span></td>
                <td><input type="text" id="altitudeinput" name="altitudeinput" value="0"></td>
              </tr>
              <tr>
                <td colspan="3"><h4>ThingSpeak nastavitve</h4></td>
              </tr>
              <tr>
                <td><p>Thingspeak API key:</p></td>
                <td><span id="tsapi" name="tsapi"></span></td>
                <td><input type="text" id="tsapiinput" name="tsapiinput" value=""></td>
              </tr>
              <tr>
                <td><p>Thingspeak channel ID:</p></td>
                <td><span id="tschid" name="tschid"></span></td>
                <td><input type="text" id="tschidinput" name="tschidinput" value=""></td>
              </tr>
              <tr>
                <td><p>Password:</p></td>
                <td><span id="password" name="password"></span></td>
                <td><input type="password" id="passwordinput" name="passwordinput" value=""></td>
              </tr>
              <tr>
                <td colspan="2"><button type="button" onclick="GetSettingsData()" class="shadow button">Osveži</button></td>
                <td colspan="1"><input class="shadow button" type="submit" value="Shrani" style="float: right;"></td>
              </tr>
            </tbody></table>
            <br>
          </form>
          <button type="button" onclick="OTA()" class="shadow button">Update Over The Air</button>
        </article>
      </div>
      <script>

function OTA() {
    updatePopup();
    OTAUpdate();
    setTimeout("Redirect()", 1000);
}

function Redirect() {
    window.location = "/";
}

function updatePopup() {
    var popup = document.createElement("div");
    popup.innerHTML = "<div id='disable' style='width: 100%;height: 100%;position: absolute;top: 0;z-index: 100;background: rgba(1, 1, 1, 0.76);'><div id='OTAPopup' style='position: fixed;right: 0;left: 0;text-align: center;margin: 0 auto;width: 50%;height: 100%;background: #FFF;'><h1 style='margin:0; padding-top: 30px;'>Posodabljanje programske opreme</h1><img style='margin:0 auto;margin-top:100px;max-width: 75%;' src='http://sshp.dejavu.si/img/loading.gif'></div></div>";
    document.getElementById("wrapper").appendChild(popup);
}

function OTAUpdate() {
    setValues("/settings/ota");
}

function GetSettingsData() {
    setValues("/settings/data");
}

window.onload = function() {
    load("microajax.js", "js", function() {
        GetSettingsData();
    });
}

function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}
</script>
)=====";

void send_system_settings_data()
{
  unsigned long lastRequest = millis();
	SerialHandler::requestSettings();
  while(!Serial.available()){
    if(millis() - lastRequest >= noDataRecivedInterval){
      SerialHandler::requestSettings();
      lastRequest = millis();
    }
  }
  SerialHandler::handle();
	String values = "";
	values += "tdiffmin|" + (String)ardSettings.tdiffmin + "|span\n";
	values += "tdiffmininput|" + (String)ardSettings.tdiffmin + "|input\n";
	values += "tkmax|" + (String)ardSettings.tkmax + "|span\n";
	values += "tkmaxinput|" + (String)ardSettings.tkmax + "|input\n";
	values += "tkmin|" + (String)ardSettings.tkmin + "|span\n";
	values += "tkmininput|" + (String)ardSettings.tkmin + "|input\n";
	values += "tbmax|" + (String)ardSettings.tbmax + "|span\n";
	values += "tbmaxinput|" + (String)ardSettings.tbmax + "|input\n";
	values += "altitude|" + (String)ardSettings.altitude + "|span\n";
  values += "altitudeinput|" + (String)ardSettings.altitude + "|input\n";
  values += "tsapi|" + (String)apiKey + "|span\n";
  values += "tsapiinput|" + (String)apiKey + "|input\n";
  values += "tschid|" + (String)thingspeakChannelID + "|span\n";
  values += "tschidinput|" + (String)thingspeakChannelID + "|input\n";
  values += "password|" + (String)"******" + "|span\n";
  values += "passwordinput|" + (String)loginPassword + "|input";
	server.send(200, "text/plain", values);
}

void handleSettings() {
	if (!is_authentified()) {
		String header = "HTTP/1.1 301 OK\r\nLocation: /\r\nCache-Control: no-cache\r\n\r\n";
		server.sendContent(header);
		return;
	}

	server.sendContent(String(PAGE_head) + String(PAGE_menu_logedin) + String(PAGE_settings) + String(PAGE_foot));

	if (server.args() > 0)  // Save Settings
	{
		if (server.hasArg("tdiffmininput")) {
			ardSettings.tdiffmin = server.arg("tdiffmininput").toInt();
			ardSettings.tdiffmininput = ardSettings.tdiffmin;
		}
		if (server.hasArg("tkmaxinput")) {
			ardSettings.tkmax = server.arg("tkmaxinput").toInt();
			ardSettings.tkmaxinput = ardSettings.tkmax;
		}
		if (server.hasArg("tkmininput")) {
			ardSettings.tkmin = server.arg("tkmininput").toInt();
			ardSettings.tkmininput = ardSettings.tkmin;
		}
		if (server.hasArg("tbmaxinput")) {
			ardSettings.tbmax = server.arg("tbmaxinput").toInt();
			ardSettings.tbmaxinput = ardSettings.tbmax;
		}
		if (server.hasArg("altitudeinput")) {
      ardSettings.altitude = server.arg("altitudeinput").toInt();
      ardSettings.altitudeinput = ardSettings.altitude;
    }
    if (server.hasArg("tsapiinput")) {
      apiKey = server.arg("tsapiinput");
      eepromWriteString(0, 16, apiKey);
    }
    if (server.hasArg("tschidinput")) {
      thingspeakChannelID = server.arg("tschidinput");
      eepromWriteString(25, 6, thingspeakChannelID);
    }
    if (server.hasArg("passwordinput")) {
      loginPassword = server.arg("passwordinput");
      File f = SPIFFS.open("/data.txt", "w");
      f.println(loginPassword + "|" + thingspeakChannelID + "|" + apiKey + "|");
      f.close();
    }
		String Settings = "Set(";
		Settings += "{\"mTD\": " + String(ardSettings.tdiffmin) + ",\"maxTC\": " + String(ardSettings.tkmax) + ",\"minTC\": " + String(ardSettings.tkmin) + ",\"mTB\": " + String(ardSettings.tbmax) + ",\"a\": " + String(ardSettings.altitude) + "});";
		Serial.print(Settings);
	}
}
