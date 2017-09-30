//replace with url to thingspeak graphs
const String graphThingspeakURL = "https://thingspeak.com/channels/";
const String graphProperties = "?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line";

const char PAGE_graphs[] PROGMEM = R"=====(
<div id="content">
        <article>
          <h1>Grafi</h1>
          <iframe id="graph1url" width="450" height="260" style="border: 1px solid #cccccc;" ></iframe>
          <iframe id="graph2url" width="450" height="260" style="border: 1px solid #cccccc;" ></iframe>
          <iframe id="graph3url" width="450" height="260" style="border: 1px solid #cccccc;" ></iframe>
          <iframe id="graph4url" width="450" height="260" style="border: 1px solid #cccccc;" ></iframe>
          <iframe id="graph5url" width="450" height="260" style="border: 1px solid #cccccc;" ></iframe>
          <iframe id="graph6url" width="450" height="260" style="border: 1px solid #cccccc;" ></iframe>
        </article>
      </div>
      <script>
      function GetGraphsData()
      {
        setValues("/graphs/data");
      }

      window.onload = function ()
      {
        load("microajax.js","js", function()
        {
            GetGraphsData();
        });
      }
      function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}

      </script>
)=====";

void send_system_graphs_data()
{
	String values = "";
  if(thingspeakChannelID != NULL ){
	values += "graph1url|" + graphThingspeakURL + thingspeakChannelID + "/charts/1"+ graphProperties + "|iframe\n";
  values += "graph2url|" + graphThingspeakURL + thingspeakChannelID + "/charts/2"+ graphProperties + "|iframe\n";
  values += "graph3url|" + graphThingspeakURL + thingspeakChannelID + "/charts/3"+ graphProperties + "|iframe\n";
  values += "graph4url|" + graphThingspeakURL + thingspeakChannelID + "/charts/4"+ graphProperties + "|iframe\n";
  values += "graph5url|" + graphThingspeakURL + thingspeakChannelID + "/charts/5"+ graphProperties + "|iframe\n";
  values += "graph6url|" + graphThingspeakURL + thingspeakChannelID + "/maps/channel_show" + "|iframe\n";
  }
	server.send(200, "text/plain", values);
}

void handleGraphs() {
	if (!is_authentified()) {
		String header = "HTTP/1.1 301 OK\r\nLocation: /\r\nCache-Control: no-cache\r\n\r\n";
		server.sendContent(header);
		return;
	}
  server.send(200, "text/html", String(PAGE_head)+String(PAGE_menu_logedin)+String(PAGE_graphs)+String(PAGE_foot));


}
