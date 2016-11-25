const char PAGE_status[] PROGMEM = R"=====(
	<h2>trenutno stanje</h2>
	<br/>
	<table style="width:50%" align="center">
		<tr>
			<td>
				<h4>Senzor</h4></td>
			<td>
				<h4>Vrednost</h4></td>
		</tr>
		<tr>
			<td>
				<span>Status crpalke:</span>
			</td>
			<td>
				<span id="pumpstatus" name="pumpstatus"></span>
			</td>
		</tr>
		<tr>
			<td>
				<span>Avtomatsko delovanje črpalke:</span>
			</td>
			<td>
				<span id="pumpautomode" name="pumpautomode"></span>
			</td>
		</tr>
		<tr>
			<td>
				<span>Skupen čas delovanja(danes):</span>
			</td>
			<td>
				<span id="operatinghours" name="operatinghours"></span><span>h, </span>
				<span id="operatingminutes" name="operatingminutes"></span><span>m</span>
			</td>
		</tr>
		<tr>
			<td>
				<span>TK:</span>
			</td>
			<td>
				<span id="tempcollector" name="tempcollector"></span><span>°C</span>
			</td>
		</tr>
		<tr>
			<td>
				<span>TB:</span>
			</td>
			<td>
				<span id="tempboiler" name="tempboiler"></span><span>°C</span>
			</td>
		</tr>
		<tr>
			<td>
				<span>T1:</span>
			</td>
			<td>
				<span id="tempt1" name="tempt1"></span><span>°C</span>
			</td>
		</tr>
		<tr>
			<td>
				<span>T2:</span>
			</td>
			<td>
				<span id="tempt2" name="tempt2"></span><span>°C</span>
			</td>
		</tr>
		<tr>
			<td>
				<span>Sobna temperatura:</span>
			</td>
			<td>
				<span id="temproom" name="temproom"></span><span>°C</span>
			</td>
		</tr>
		<tr>
			<td>
				<span>Vlaga:</span>
			</td>
			<td>
				<span id="humidityroom" name="humidityroom"></span><span>%</span>
			</td>
		</tr>
		<tr>
			<td>
				<span>Zracni tlak:</span>
			</td>
			<td>
				<span id="pressureroom" name="pressureroom"></span><span>mBa</span>
			</td>
		</tr>
		<tr>
			<td><button onclick="GetStatusData()" class="shadow button">Osveži</button></td>
		</tr>
	</table>
	</div>
</div>

<script>
function GetStatusData()
{
	setValues("/status/data");
}

window.onload = function ()
{
	load("microajax.js","js", function() 
	{
			GetStatusData();
	});
}
function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}
</script>
)=====";

void send_system_status_data()
{
	update();
	String values = "";
	values += "pumpstatus|" + (String)ardData.pump1operating + "|span\n";
	values += "pumpautomode|" + (String)ardData.pumpautomode + "|span\n";
	values += "operatinghours|" + (String)ardData.operatinghours + "|span\n";
	values += "operatingminutes|" + (String)ardData.operatingminutes + "|span\n";
	values += "tempcollector|" + (String)ardData.tempcollector + "|span\n";
	values += "tempboiler|" + (String)ardData.tempboiler + "|span\n";
	values += "tempt1|" + (String)ardData.tempt1 + "|span\n";
	values += "tempt2|" + (String)ardData.tempt2 + "|span\n";
	values += "temproom|" + (String)ardData.temproom + "|span\n";
	values += "humidityroom|" + (String)ardData.humidityroom + "|span\n";
	values += "pressureroom|" + (String)ardData.pressureroom + "|span\n";
	server.send(200, "text/plain", values);
}

void handleStatus() {
	String content = String(PAGE_HEAD);
	if (is_authentified())content += String(PAGE_MENU_LOGEDIN);
	else content += String(PAGE_MENU_NORMAL);
	content += String(PAGE_status);
	content += String(PAGE_FOOT);
	server.send(200, "text/html", content);
}