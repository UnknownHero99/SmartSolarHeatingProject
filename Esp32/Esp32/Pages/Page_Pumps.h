const char PAGE_pumps[] PROGMEM = R"=====(
<div id="content">
        <article>
          <h1>Upravljanje črpalk</h1>
          <style>
            input[type=radio]{position:absolute;visibility:hidden;display:none}.radio-group,label{display:inline-block}label{color:#FFF;cursor:pointer;font-weight:700;padding:5px 20px}.option-one:checked+label{background-color:#5cb85c}.option-two:checked+label{background-color:#ff551d}.option-three:checked+label{background-color:#d9534f}label+input[type=radio]+label{border-left:solid 4px #999}.radio-group{border:4px solid #999;border-radius:10px;overflow:hidden}
          </style>

            <form class="inline-block">
              <div class="text underline">Pumpa 1</div>
              <div class="radio-group">
                <input type="radio" class="option-one" id="pump1-on" name="selector" onclick="check1()" /><label class="text" for="pump1-on" >On</label><input type="radio" class="option-two" id="pump1-auto" name="selector" onclick="check1()" /><label class="text" for="pump1-auto">Auto</label><input type="radio" class="option-three" id="pump1-off" name="selector" onclick="check1()" /><label class="text" for="pump1-off">Off</label>
              </div>
            </form>
            <form class="inline-block">
              <div class="text underline">Pumpa 2</div>
              <div class="radio-group">
                <input type="radio" class="option-one" id="pump2-on" name="selector" onclick="check2()" /><label class="text" for="pump2-on">On</label><input type="radio" class="option-two" id="pump2-auto" name="selector" onclick="check2()" /><label class="text" for="pump2-auto">Auto</label><input type="radio" class="option-three" id="pump2-off" name="selector" onclick="check2()" /><label class="text" for="pump2-off">Off</label>
              </div>
            </form>
            <form class="inline-block">
              <div class="text underline">Pumpa 3</div>
              <div class="radio-group">
                <input type="radio" class="option-one" id="pump3-on" name="selector" onclick="check3()" /><label class="text" for="pump3-on">On</label><input type="radio" class="option-two" id="pump3-auto" name="selector" onclick="check3()" /><label class="text" for="pump3-auto">Auto</label><input type="radio" class="option-three" id="pump3-off" name="selector" onclick="check3()" /><label class="text" for="pump3-off">Off</label>
              </div>
            </form>
            <form class="inline-block">
              <div class="text underline">Pumpa 4</div>
              <div class="radio-group">
                <input type="radio" class="option-one" id="pump4-on" name="selector" onclick="check4()" /><label class="text" for="pump4-on">On</label><input type="radio" class="option-two" id="pump4-auto" name="selector" onclick="check4()" /><label class="text" for="pump4-auto">Auto</label><input type="radio" class="option-three" id="pump4-off" name="selector" onclick="check4()" /><label class="text" for="pump4-off">Off</label>
              </div>
            </form>
            <script>
              function check1() {
                if(document.getElementById('pump1-on').checked) window.location.replace('?pump1=ON');
                else if(document.getElementById('pump1-auto').checked) window.location.replace('?pump1=Auto');
                else if(document.getElementById('pump1-off').checked) window.location.replace('?pump1=OFF');
              }
              function check2() {
                if(document.getElementById('pump2-on').checked) window.location.replace('?pump2=ON');
                else if(document.getElementById('pump2-auto').checked) window.location.replace('?pump2=Auto');
                else if(document.getElementById('pump2-off').checked) window.location.replace('?pump2=OFF');
              }
              function check3() {
                if(document.getElementById('pump3-on').checked) window.location.replace('?pump3=ON');
                else if(document.getElementById('pump3-auto').checked) window.location.replace('?pump3=Auto');
                else if(document.getElementById('pump3-off').checked) window.location.replace('?pump3=OFF');
              }
              function check4() {
                if(document.getElementById('pump4-on').checked) window.location.replace('?pump4=ON');
                else if(document.getElementById('pump4-auto').checked) window.location.replace('?pump4=Auto');
                else if(document.getElementById('pump4-off').checked) window.location.replace('?pump4=OFF');
              }
            </script>

        </article>
      </div>
      <script>
        function GetStatusData()
        {
          setValues("/data/pumps");
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

String getPumpsData(){
  String values = "";

  values += "pump1status|";
  if(autoMode) values += "A";
  else if(pumps[0].isOperating())values += "On";
  else values += "Off";
  values += "|span\n";

  values += "pump2status|";
  if(pumps[1].isOperating())values += "On";
  else values += "Off";
  values += "|span\n";

  values += "pump3status|" ;
  if(pumps[2].isOperating())values += "On";
  else values += "Off";
  values += "|span\n";

  values += "pump4status|";
  if(pumps[3].isOperating())values += "On";
  else values += "Off";
  values += "|span\n";

  return values;
}
