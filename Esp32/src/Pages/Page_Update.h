#include <Update.h>

//OTA settings
const String host = "sshp.dejavu.si"; // OTA server
const int port = 80; // port number
const String firmwareLocation = "/updates/firmware/"; // firmwareLocation file location
const String firmwareFileName = "/firmware.bin";
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
  String latestVersion = "";
  //Serial.println("Checking latest Version AT: " + String(host));
  if (client.connect(host.c_str(), port)) {
    client.print(String("GET ") + "/updates/index.php" + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Cache-Control: no-cache\r\n" +
                 "Connection: close\r\n\r\n");
    String line = client.readStringUntil('\n');
    //Serial.println(line);
    latestVersion = line.substring(line.lastIndexOf(' ')+1, line.length()-1);
    //Serial.println(latestVersion);
    client.stop();
  }
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


  //Serial.println("latestVersion is: "+ String(latestVersion));
  //Serial.println(String(latestVersionDigits[0]) + "."+ String(latestVersionDigits[1]));
  //Serial.println(String(currentVersionDigits[0]) + "."+ String(currentVersionDigits[1]));

	if(latestVersionDigits[0] > currentVersionDigits[0]) return true;
  else if(latestVersionDigits[1] > currentVersionDigits[1]) return true;
  return false;
}

// Utility to extract header value from headers
String getHeaderValue(String header, String headerName) {
  return header.substring(strlen(headerName.c_str()));
}

void OTAUpdate() {
  bool updateAvailable = checkForUpdate();
  Serial.println(String(updateAvailable));
  if(!updateAvailable) return;

  if (client.connect(host.c_str(), port)) {
    // Connection Succeed.
    // Fecthing the BIN file
    //Serial.println("Fetching firmwareLocation: " + String(firmwareLocation));

    client.print(String("GET ") + firmwareLocation+latestVersion+firmwareFileName + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Cache-Control: no-cache\r\n" +
                 "Connection: close\r\n\r\n");

    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println("Client Timeout !");
        client.stop();
        return;
      }
    }

    while (client.available()) {

      String line = client.readStringUntil('\n');

      // remove space, to check if the line is end of headers
      line.trim();

      if (!line.length()) {
        //headers ended
        break; // start OTA
      }

      // Check if the HTTP Response is 200 otherwise cancel
      if (line.startsWith("HTTP/1.1")) {
        if (line.indexOf("200") < 0) {
          //Serial.println("HTTP response code is not 200");
          break;
        }
      }

      // get headers
      if (line.startsWith("Content-Length: ")) {
        contentLength = atoi((getHeaderValue(line, "Content-Length: ")).c_str());
        //Serial.println("content length " + String(contentLength) + " bytes");
      }

      if (line.startsWith("Content-Type: ")) {
        String contentType = getHeaderValue(line, "Content-Type: ");

        if (contentType == "application/octet-stream") {
          isValidContentType = true;
        }
      }
    }
  } else {
    // failed to connect to update server
  }

  // check contentLength and content type
  if (contentLength && isValidContentType) {
    // if there is enough flash to write BIN file
    bool canBegin = Update.begin(contentLength);


    if (canBegin) {
	  //OTA started need write to WEB GUI that it may take up to 5 mins
      size_t written = Update.writeStream(client);

      if (written == contentLength) {
		//check if writting was successful
		//written = content size...
      } else {
        //writting failed
      }

      if (Update.end()) {
        //OTA done
        if (Update.isFinished()) {
          //successfully updated
          ESP.restart();//restart ESP32
        } else {
          //update not finnished sth caused errpr
        }
      } else {
        //Update.getError() returns error message
      }
    } else {
      //Not enough space to begin OTA
      client.flush();
    }
  } else {
    //no content in response
    client.flush();
  }
}
