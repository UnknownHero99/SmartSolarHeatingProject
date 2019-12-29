#include <WiFi.h>
#include <ESP32httpUpdate.h>
//OTA settings
const uint16_t port = 80;
const char * host = "sshp.time-out.si"; // ip or dns
const String firmwareLocation = "/updates/firmware"; // firmwareLocation file location
const String firmwareFileName = "firmware.bin";
String latestVersion;


//response
int contentLength = 0;
bool isValidContentType = false;

const char PAGE_update[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head></head>
<body>
<script type="text/javascript">
      function Redirect()
      {
        window.location="/";
      }
      setTimeout('Redirect()', 1000);
    </script>
</body>
</html>
)=====";

const char PAGE_noPermission[] PROGMEM = R"=====(
    <div id="content"> <article> <h1>You have no permission for this operation</h1>
    <script type="text/javascript">
    function Redirect()
    {
    window.location="/login";
    }
    setTimeout('Redirect()', 5000);
  </script>
)=====";

String getLatestVersion(){

  Serial.print("Connecting to ");
  Serial.println(host);
  String latestVersion = "";
  // Use WiFiClient class to create TCP connections
  WiFiClient client;

  if (!client.connect(host, port)) {
      Serial.println("Connection failed.");
      Serial.println("Waiting 5 seconds before retrying...");
      delay(5000);
      return releaseVersion;
  }

  client.print(String("GET ") + "/updates/index.php" + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Cache-Control: no-cache\r\n" +
               "Connection: close\r\n\r\n");
  String line = client.readString();
  Serial.println(line);
  latestVersion = line.substring(line.lastIndexOf(' ')+1, line.length());
  Serial.println(latestVersion);
  client.stop();
  return latestVersion;
}


bool checkForUpdate(){
  bool updateAvailable = false;
  latestVersion = getLatestVersion();

  int latestVersionDigits[2];
  latestVersionDigits[0] = latestVersion.substring(0, latestVersion.indexOf('.')).toInt();
  latestVersionDigits[1] = latestVersion.substring(latestVersion.indexOf('.')+1).toInt();
  int currentVersionDigits[2];
  currentVersionDigits[0] = releaseVersion.substring(0, releaseVersion.indexOf('.')).toInt();
  currentVersionDigits[1] = releaseVersion.substring(releaseVersion.indexOf('.')+1).toInt();


  Serial.println("latestVersion is: "+ String(latestVersion));
  Serial.println(String(latestVersionDigits[0]) + "."+ String(latestVersionDigits[1]));
  Serial.println(String(currentVersionDigits[0]) + "."+ String(currentVersionDigits[1]));

	if(latestVersionDigits[0] > currentVersionDigits[0]) return true;
  else if(latestVersionDigits[1] > currentVersionDigits[1]) return true;
  return false;
}

// Utility to extract header value from headers
String getHeaderValue(String header, String headerName) {
  return header.substring(strlen(headerName.c_str()));
}

void OTAUpdate() {
  if(!checkForUpdate()) return; //latest version already installed
  Serial.println("Connecting to: " + String(host));
          t_httpUpdate_return ret = ESPhttpUpdate.update("http://" + String(host) + firmwareLocation + "/" + latestVersion + "/" + firmwareFileName);
          switch(ret) {
              case HTTP_UPDATE_FAILED:
                  Serial.print("HTTP_UPDATE_FAILD Error (");
                  Serial.print(ESPhttpUpdate.getLastError());
                  Serial.print(ESPhttpUpdate.getLastErrorString().c_str());
                  break;

              case HTTP_UPDATE_NO_UPDATES:
                  Serial.println("HTTP_UPDATE_NO_UPDATES");
                  break;

              case HTTP_UPDATE_OK:
                  Serial.println("HTTP_UPDATE_OK");
                  break;
          }
      }
