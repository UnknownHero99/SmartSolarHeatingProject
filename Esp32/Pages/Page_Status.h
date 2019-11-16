const char PAGE_status[] PROGMEM = R"=====(
<div id="content">
        <article>
          <h1>Trenutno stanje</h1>
          <div style="text-align: center;max-width: 1250px;margin: 0 auto;">
            <div class="textValue"><div class="text underline">Stanje črpalke</div><div class="text" id="pumpstatus">-</div></div>
            <div class="textValue"><div class="text underline">Avtomatski način</div><div class="text" id="pumpautomode">-</div></div>
            <div class="textValue"><div class="text underline">Čas delovanja črpalke</div><span class="text" id="operatinghours">-</span><span class="text"> h </span><span class="text" id="operatingminutes">-</span><span class="text"> m</span></div>
          </div>
          <div style="text-align: center;max-width: 1250px;margin: 0 auto;">
            <div id="boilerTempContainer" class="progressbar"><div class="text progressbarText">Temp Bojlerja</div></div>
            <div id="collectorTempContainer" class="progressbar"><div class="text progressbarText">Temp Kolektorja</div></div>
            <div id="t1TempContainer" class="progressbar"><div class="text progressbarText">Temp T1</div></div>
            <div id="t2TempContainer" class="progressbar"><div class="text progressbarText">Temp T2</div></div>
            <div id="roomTempContainer" class="progressbar"><div class="text progressbarText">Sobna Temp</div></div>
            <div id="roomHumidityContainer" class="progressbar"><div class="text progressbarText">Sobna vlažnost</div></div>
            <div id="airPressureContainer" class="progressbar"><div class="text progressbarText">Zračni pritisk</div></div>
          </div>
          <button onclick="GetStatusData()" class="shadow button">Osveži</button>
        </article>
      </div>
      <script>
        var progressbarSettings = {
          color: '#ff551d',
          strokeWidth: 4,
          trailWidth: 1,
          easing: 'easeInOut',
          duration: 1400,
          text: {
            autoStyleContainer: false
          },
          from: { color: '#27ae60', width: 3 },
          to: { color: '#ff3f00', width: 5 },
          attachment: { units: "°C" },
          step: function(state, circle, attachment) {
            circle.path.setAttribute('stroke', state.color);
            circle.path.setAttribute('stroke-width', state.width);

          var value = Math.round(circle.value() * 1000)/10;
            if (value === 0) {
              circle.setText('');
            } else {
            if (attachment.min != undefined){
              var diff = attachment.max - attachment.min;

              value = Math.round(value*diff+attachment.min*100)/100;
              value = Math.round(value);
              circle.setText(value + " " + attachment.units);
            }
            else circle.setText(value + " " + attachment.units);
            }

          }
        }

        var progressBars = new Array(new ProgressBar.Circle(boilerTempContainer, progressbarSettings),
        new ProgressBar.Circle(collectorTempContainer, progressbarSettings),
        new ProgressBar.Circle(t1TempContainer, progressbarSettings),
        new ProgressBar.Circle(t2TempContainer, progressbarSettings),
        new ProgressBar.Circle(roomTempContainer, progressbarSettings),
        new ProgressBar.Circle(roomHumidityContainer, progressbarSettings),
        new ProgressBar.Circle(airPressureContainer, progressbarSettings));

        for(var i = 0; i < progressBars.length; i++){
          progressBars[i].text.style.fontFamily = '"Raleway", Helvetica, sans-serif';
          progressBars[i].text.style.fontSize = '1.5rem';
        }
      </script>
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

String getStatusData() {
  String values = "";

  values += "pumpstatus|";
  if (pumps[0].isOperating())
    values += "ON";
  else
    values += "OFF";
  values += "|span\n";

  values += "pumpautomode|";
  if (autoMode)
    values += "ON";
  else
    values += "OFF";
  values += "|span\n";

  values +=
      "operatinghours|" + String(pumps[0].operatingTime("%H")) + "|span\n";
  values +=
      "operatingminutes|" + String(pumps[0].operatingTime("%M")) + "|span\n";
  values += "tempcollector|" + String(collectorSensor.tempDouble()) + "|span\n";
  values += "tempboiler|" + String(boilerSensor.tempDouble()) + "|span\n";
  values +=
      "tempt1|" +
      String((t1Sensor.tempDouble() == -127 ? 0 : t1Sensor.tempDouble())) +
      "|span\n";
  values +=
      "tempt2|" +
      String((t2Sensor.tempDouble() == -127 ? 0 : t2Sensor.tempDouble())) +
      "|span\n";
  values += "temproom|" + String(roomTemp) + "|span\n";
  values += "humidityroom|" + String(roomHumidity) + "|span\n";
  values += "pressureroom|" + String(roomPressure) + "|span\n";
  return values;
}
