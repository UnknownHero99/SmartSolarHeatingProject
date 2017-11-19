

/*Include all pages and REST API*/
#include "Pages/API.h"
#include "Pages/Page_404NotFound.h"
#include "Pages/Page_Constants.h"
#include "Pages/Page_Functions.h"
#include "Pages/Page_Graphs.h"
#include "Pages/Page_Login.h"
#include "Pages/Page_MicroAjax.js.h"
#include "Pages/Page_Pumps.h"
#include "Pages/Page_Settings.h"
#include "Pages/Page_Status.h"
#include "Pages/Page_Update.h"
AsyncWebServer server(80); // The Webserver

class WebServerHandler {
  public:
    static void initWebserver() {

      server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
        String content = String(PAGE_head);
        if (is_authentified(request))
          content += String(PAGE_menu_logedin);
        else
          content += String(PAGE_menu_normal);
        content += String(PAGE_status) + String(PAGE_foot);
        request->send(200, "text/html", content);
      });

      server.on("/status/data", HTTP_ANY, [](AsyncWebServerRequest * request) {
        sensorUpdate();
        request->send(200, "text/plain", getStatusData());
      });

      // API
      server.on("/api", HTTP_ANY, [](AsyncWebServerRequest * request) {

        request->send(200, "application/json", getApiData());
      });
      server.on("/api/set", HTTP_ANY,
      [](AsyncWebServerRequest * request) {
        handleSettings(request);
      });

      server.on("/api/login", HTTP_ANY, [](AsyncWebServerRequest * request) {
        if (request->hasArg("USERNAME") && request->hasArg("PASSWORD")) {
          if (request->arg("USERNAME") == loginUsername &&
              request->arg("PASSWORD") == loginPassword) {
            AsyncWebServerResponse *response = request->beginResponse(301);
            response->addHeader("Set-Cookie", "ESPSESSIONID=1");
            response->addHeader("Location", "/");
            request->send(response);
          }
        }
      });

      // login
      server.on("/login", HTTP_ANY, [](AsyncWebServerRequest * request) {
        if (request->hasHeader("Cookie")) {
          String cookie = request->header("Cookie");
        }
        if (request->hasArg("DISCONNECT")) {
          AsyncWebServerResponse *response = request->beginResponse(301);
          response->addHeader("Set-Cookie", "ESPSESSIONID=0");
          response->addHeader("Location", "/");
          request->send(response);
          return;
        }
        if (request->hasArg("USERNAME") && request->hasArg("PASSWORD")) {
          if (request->arg("USERNAME") == loginUsername &&
              request->arg("PASSWORD") == loginPassword) {
            AsyncWebServerResponse *response = request->beginResponse(301);
            response->addHeader("Set-Cookie", "ESPSESSIONID=1");
            response->addHeader("Location", "/");
            request->send(response);
            return;
          }
          String msg = "Napačno uporabniško ime/geslo! Poizkusi znova.";
        }
        String content = String(PAGE_head) + String(PAGE_menu_normal) +
                         String(PAGE_login) + String(PAGE_foot);
        request->send(200, "text/html", content);
      });

      // settings
      server.on("/settings", HTTP_ANY, [](AsyncWebServerRequest * request) {
        if (!is_authentified(request)) {
          AsyncWebServerResponse *response = request->beginResponse(301);
          response->addHeader("Set-Cookie", "ESPSESSIONID=1");
          response->addHeader("Location", "/");
          request->send(response);
          return;
        }

        request->send(200, "text/html",
                      String(PAGE_head) + String(PAGE_menu_logedin) +
                      String(PAGE_settings) + String(PAGE_foot));

        if (request->args() > 0) // Save Settings
        {
          if (request->hasArg("tdiffmininput") || request->hasArg("tkmaxinput") ||
              request->hasArg("tkmininput") || request->hasArg("tbmaxinput") ||
              request->hasArg("altitudeinput") || request->hasArg("tsapiinput") ||
              request->hasArg("tschidinput") ||
              request->hasArg("passwordinput")) {
            SettingsValues.tdiffmin = request->arg("tdiffmininput").toInt();
            SettingsValues.tdiffmininput = SettingsValues.tdiffmin;
            SettingsValues.tkmax = request->arg("tkmaxinput").toInt();
            SettingsValues.tkmaxinput = SettingsValues.tkmax;
            SettingsValues.tkmin = request->arg("tkmininput").toInt();
            SettingsValues.tkmininput = SettingsValues.tkmin;
            SettingsValues.tbmax = request->arg("tbmaxinput").toInt();
            SettingsValues.tbmaxinput = SettingsValues.tbmax;
            SettingsValues.altitude = request->arg("altitudeinput").toInt();
            SettingsValues.altitudeinput = SettingsValues.altitude;
            apiKey = request->arg("tsapiinput");
            thingspeakChannelID = request->arg("tschidinput");
            loginPassword = request->arg("passwordinput");
            File f = SPIFFS.open("/data.txt", "w");
            f.println(loginPassword + "|" + thingspeakChannelID + "|" + apiKey +
                      "|" + SettingsValues.tdiffmin + "|" + SettingsValues.tkmax +
                      "|" + SettingsValues.tkmin + "|" + SettingsValues.tbmax +
                      "|" + SettingsValues.altitude + "|");
            f.close();
          }
        }
      });
      server.on("/data/settings", HTTP_ANY, [](AsyncWebServerRequest * request) {

        request->send(200, "text/plain", getSettingsData());
      });
      // OTA
      server.on("/ota", HTTP_ANY, [](AsyncWebServerRequest * request) {
        String content = String(PAGE_head);
        if (is_authentified(request)) {
          content += String(PAGE_menu_logedin) + String(PAGE_update);
          OTAUpdate(); // Update
        }

        else {
          content += String(PAGE_menu_normal) + String(PAGE_noPermission);
        }
        content += String(PAGE_foot);
        request->send(200, "text/html", content);
      });

      server.on("/microajax.js", HTTP_ANY, [](AsyncWebServerRequest * request) {
        request->send(200, "text/html", PAGE_microajax_js);
      });

      server.on("/pumps", HTTP_ANY, [](AsyncWebServerRequest * request) {
        if (!is_authentified(request)) {
          AsyncWebServerResponse *response = request->beginResponse(301);
          response->addHeader("Set-Cookie", "ESPSESSIONID=1");
          response->addHeader("Location", "/");
          request->send(response);
          return;
        }

        request->send(200, "text/html",
                      String(PAGE_head) + String(PAGE_menu_logedin) +
                      String(PAGE_pumps) + String(PAGE_foot));
        if (request->hasArg("pump1")) {
          String arg = request->arg("pump1");
          if (arg == "ON") {
            pumps[0].on();
            autoMode = false;
          } else if (arg == "OFF") {
            pumps[0].off();
            autoMode = false;
          }
          else if (arg == "Auto")
            autoMode = true;
        }

        if (request->hasArg("pump2")) {
          String arg = request->arg("pump2");
          if (arg == "ON")
            pumps[1].on();
          else if (arg == "OFF")
            pumps[1].off();
          else if (arg == "Auto")
            ;
        }
        if (request->hasArg("pump3")) {
          String arg = request->arg("pump3");
          if (arg == "ON")
            pumps[2].on();
          else if (arg == "OFF")
            pumps[2].off();
          else if (arg == "Auto")
            ;
        }
        if (request->hasArg("pump4")) {
          String arg = request->arg("pump4");
          if (arg == "ON")
            pumps[3].on();
          else if (arg == "OFF")
            pumps[3].off();
          else if (arg == "Auto")
            ;
        }
      });
      server.on("/data/pumps", HTTP_ANY, [](AsyncWebServerRequest * request) {
        request->send(200, "text/plain", getPumpsData());
      });

      server.on("/graphs", HTTP_GET, [](AsyncWebServerRequest * request) {
        if (!is_authentified(request)) {
          AsyncWebServerResponse *response = request->beginResponse(301);
          response->addHeader("Set-Cookie", "ESPSESSIONID=1");
          response->addHeader("Location", "/");
          request->send(response);
          return;
        }
        request->send(200, "text/html",
                      String(PAGE_head) + String(PAGE_menu_logedin) +
                      String(PAGE_graphs) + String(PAGE_foot));

      });
      server.on("/data/graphs", HTTP_GET, [](AsyncWebServerRequest * request) {
        String values = "";
        if (thingspeakChannelID != NULL) {
          values += "graph1url|" + graphThingspeakURL + thingspeakChannelID +
                    "/charts/1" + graphProperties + "|iframe\n";
          values += "graph2url|" + graphThingspeakURL + thingspeakChannelID +
                    "/charts/2" + graphProperties + "|iframe\n";
          values += "graph3url|" + graphThingspeakURL + thingspeakChannelID +
                    "/charts/3" + graphProperties + "|iframe\n";
          values += "graph4url|" + graphThingspeakURL + thingspeakChannelID +
                    "/charts/4" + graphProperties + "|iframe\n";
          values += "graph5url|" + graphThingspeakURL + thingspeakChannelID +
                    "/charts/5" + graphProperties + "|iframe\n";
          values += "graph6url|" + graphThingspeakURL + thingspeakChannelID +
                    "/maps/channel_show" + "|iframe\n";
        }
        request->send(200, "text/plain", values);
      });

      server.onNotFound([](AsyncWebServerRequest * request) {
        String content = String(PAGE_head);
        if (is_authentified(request))
          content += String(PAGE_menu_logedin);
        else
          content += String(PAGE_menu_normal);
        content += String(PAGE_404notfound) + String(PAGE_foot);
        request->send(404, "text/html", content);
      });

      if (!MDNS.begin("SSHProject")) {
        Serial.println("Error setting up MDNS responder!");
        while (1) {
          delay(1000);
        }
      }
      Serial.println("mDNS responder started");
      server.begin();
      MDNS.addService("http", "tcp", 80);
    }
};
