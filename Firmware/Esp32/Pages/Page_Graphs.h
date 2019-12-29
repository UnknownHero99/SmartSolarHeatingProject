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
        setValues("/data/graphs");
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
