const char PAGE_graphs[] PROGMEM = R"=====(
		<h2>Grafi</h2>
		<br/>
		<iframe id="graph1url" width="450" height="260" style="border: 1px solid #cccccc;" src=""></iframe>
		<iframe id="graph2url" width="450" height="260" style="border: 1px solid #cccccc;" src=""></iframe>
		<iframe id="graph3url" width="450" height="260" style="border: 1px solid #cccccc;" src=""></iframe>
		<iframe id="graph4url" width="450" height="260" style="border: 1px solid #cccccc;" src=""></iframe>
		<iframe id="graph5url" width="450" height="260" style="border: 1px solid #cccccc;" src=""></iframe>
		<iframe id="graph6url" width="450" height="260" style="border: 1px solid #cccccc;" src=""></iframe>
	</div>
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
	Serial.print("GetPumps();");
	serialHandler();
	String values = "";
	values += "graph1url|" + graph1 + "|iframe\n";
	values += "graph2url|" + graph2 + "|iframe\n";
	values += "graph3url|" + graph3 + "|iframe\n";
	values += "graph4url|" + graph4 + "|iframe\n";
	values += "graph5url|" + graph5 + "|iframe\n";
	values += "graph6url|" + graph6 + "|iframe\n";
	server.send(200, "text/plain", values);
}

void handleGraphs() {
	if (!is_authentified()) {
		String header = "HTTP/1.1 301 OK\r\nLocation: /\r\nCache-Control: no-cache\r\n\r\n";
		server.sendContent(header);
		return;
	}

	String content = String(PAGE_head);
	content += String(PAGE_menu_logedin);
	content += String(PAGE_graphs);
	content += String(PAGE_foot);
	server.send(200, "text/html", content);
}