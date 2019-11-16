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
    popup.innerHTML = "<div id='disable' style='width: 100%;height: 100%;position: absolute;top: 0;z-index: 100;background: rgba(1, 1, 1, 0.76);'><div id='OTAPopup' style='position: fixed;right: 0;left: 0;text-align: center;margin: 0 auto;width: 50%;height: 100%;background: #FFF;'><h1 style='margin:0; padding-top: 30px;'>Posodabljanje programske opreme</h1><img style='margin:0 auto;margin-top:100px;max-width: 75%;' src='http://sshp.time-out.si/img/loading.gif'></div></div>";
    document.getElementById("wrapper").appendChild(popup);
}

function OTAUpdate() {
    setValues("/ota");
}

function GetSettingsData() {
    setValues("/data/settings");
}

window.onload = function() {
    load("microajax.js", "js", function() {
        GetSettingsData();
    });
}

function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}
</script>
)=====";
String getSettingsData(){
	String values = "";
	values += "tdiffmin|" + (String)SettingsValues.tdiffmin + "|span\n";
	values += "tdiffmininput|" + (String)SettingsValues.tdiffmin + "|input\n";
	values += "tkmax|" + (String)SettingsValues.tkmax + "|span\n";
	values += "tkmaxinput|" + (String)SettingsValues.tkmax + "|input\n";
	values += "tkmin|" + (String)SettingsValues.tkmin + "|span\n";
	values += "tkmininput|" + (String)SettingsValues.tkmin + "|input\n";
	values += "tbmax|" + (String)SettingsValues.tbmax + "|span\n";
	values += "tbmaxinput|" + (String)SettingsValues.tbmax + "|input\n";
	values += "altitude|" + (String)SettingsValues.altitude + "|span\n";
  values += "altitudeinput|" + (String)SettingsValues.altitude + "|input\n";
  values += "tsapi|" + (String)apiKey + "|span\n";
  values += "tsapiinput|" + (String)apiKey + "|input\n";
  values += "tschid|" + (String)thingspeakChannelID + "|span\n";
  values += "tschidinput|" + (String)thingspeakChannelID + "|input\n";
  values += "password|" + (String)"******" + "|span\n";
  values += "passwordinput|" + (String)loginPassword + "|input";
	return values;
}

void handleSettings(AsyncWebServerRequest* &request) {
	if (!is_authentified(request)) {
		String header = "HTTP/1.1 301 OK\r\nLocation: /\r\nCache-Control: no-cache\r\n\r\n";
		request->send(200, "text/html", header);
		return;
	}

	request->send(200, "text/html", String(PAGE_head) + String(PAGE_menu_logedin) + String(PAGE_settings) + String(PAGE_foot));

	if (request->args() > 0)  // Save Settings
	{
		if (request->hasArg("tdiffmininput")) {
			SettingsValues.tdiffmin = request->arg("tdiffmininput").toInt();
			SettingsValues.tdiffmininput = SettingsValues.tdiffmin;
		}
		if (request->hasArg("tkmaxinput")) {
			SettingsValues.tkmax = request->arg("tkmaxinput").toInt();
			SettingsValues.tkmaxinput = SettingsValues.tkmax;
		}
		if (request->hasArg("tkmininput")) {
			SettingsValues.tkmin = request->arg("tkmininput").toInt();
			SettingsValues.tkmininput = SettingsValues.tkmin;
		}
		if (request->hasArg("tbmaxinput")) {
			SettingsValues.tbmax = request->arg("tbmaxinput").toInt();
			SettingsValues.tbmaxinput = SettingsValues.tbmax;
		}
		if (request->hasArg("altitudeinput")) {
      SettingsValues.altitude = request->arg("altitudeinput").toInt();
      SettingsValues.altitudeinput = SettingsValues.altitude;
    }
    if (request->hasArg("tsapiinput")) {
      apiKey = request->arg("tsapiinput");
      //eepromWriteString(0, 16, apiKey);
    }
    if (request->hasArg("tschidinput")) {
      thingspeakChannelID = request->arg("tschidinput");
      //eepromWriteString(25, 6, thingspeakChannelID);
    }
    if (request->hasArg("passwordinput")) {
      loginPassword = request->arg("passwordinput");
      File f = SPIFFS.open("/data.txt", "w");
      f.println(loginPassword + "|" + thingspeakChannelID + "|" + apiKey + "|");
      f.close();
    }
		String Settings = "Set(";
		Settings += "{\"mTD\": " + String(SettingsValues.tdiffmin) + ",\"maxTC\": " + String(SettingsValues.tkmax) + ",\"minTC\": " + String(SettingsValues.tkmin) + ",\"mTB\": " + String(SettingsValues.tbmax) + ",\"a\": " + String(SettingsValues.altitude) + "});";
		Serial.print(Settings);
	}
}
