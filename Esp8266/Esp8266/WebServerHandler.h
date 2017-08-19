
ESP8266WebServer server(80);

/*Include all pages and REST API*/
#include "Pages/Page_Functions.h"
#include "Pages/Page_Constants.h"
#include "Pages/Page_MicroAjax.js.h"
#include "Pages/Page_Status.h"
#include "Pages/Page_Settings.h"
#include "Pages/Page_Login.h"
#include "Pages/Page_Pumps.h"
#include "Pages/Page_Graphs.h"
#include "Pages/Page_404NotFound.h"
#include "Pages/Page_Update.h"
#include "Pages/API.h"



class WebServerHandler {
  public:
    static void initWebserver() {

      server.on("/", handleStatus);//status page
      server.on("/status/data", send_system_status_data);//get data to display on status page

      //API
      server.on("/api", send_system_api_data);
      server.on("/api/set", set_system_api_data);
      server.on("/api/login", api_handleLogin);

      server.on("/login", handleLogin);//login page

      server.on("/settings", handleSettings);//settings page
      server.on("/settings/data", send_system_settings_data);//get data to display on settings page

      server.on("/settings/ota", handleSystemUpdate);//OTA update

      server.on("/microajax.js", []() {//microajax
        server.send(200, "text/plain", PAGE_microajax_js);
      });

      server.on("/pumps", handlePumps);//pumps page
      server.on("/pumps/data", send_system_pumps_data);//get data to display on pumpspage

      server.on("/graphs", handleGraphs);//graphs page
      server.on("/graphs/data", send_system_graphs_data);//get data to graph on pumpspage


      server.onNotFound(handleNotFound);//Handle 404


      //here the list of headers to be recorded
      const char * headerkeys[] = { "User-Agent", "Cookie" };
      size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);

      //ask server to track these headers
      server.collectHeaders(headerkeys, headerkeyssize);
      server.begin();
    }

    static void handleClient() {
      server.handleClient();
    }
};
