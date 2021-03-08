#include <Arduino.h>

/*Libraries*/
#include <WiFi.h>

#include "FS.h"
#include <SPIFFS.h>

//#define ASYNC_TCP_SSL_ENABLED 1
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>

#include <ArduinoJson.h>

#include <RTClib.h>
#include <BME280I2C.h>
#include <Wire.h>

// Choose two free pins
#define SERIAL1_RXPIN 19
#define SERIAL1_TXPIN 18

RTC_DS3231 rtc;
BME280I2C bme;

String loginPassword = "";
String apiKey;
String thingspeakChannelID;
const char *loginUsername = "admin";
const String releaseVersion = "1.1";

/*Struct for saving arduino settings*/
struct Settings {
  int tdiffmin = 0;
  int tdiffmininput = 0;
  int tkmax = 0;
  int tkmaxinput = 0;
  int tkmin = 0;
  int tkmininput = 0;
  int tbmax = 0;
  int tbmaxinput = 0;
  int altitude = 0;
  int altitudeinput = 0;
  String IP = "";
  String ssid = "";
  String password = "";
} SettingsValues;

struct statistics {
  double roomMinTemp;
  double roomMaxTemp;
  double roomMaxHumidity;
  double roomMinHumidity;
  double roomMinPressure;
  double roomMaxPressure;
} statisticsValues;

#include "LCDHandler.h"
#include "SerialHandler.h"
#include "global.h"
#include "WebServerHandler.h"

TaskHandle_t sensorsTask;

void setup(void) {
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, SERIAL1_RXPIN, SERIAL1_TXPIN);
  LCDHandler::switchPage(0);
  LCDHandler::changeText("t0", "Initalizing temperature sensors");
  while (boilerSensor.tempDouble() == -127.0 || collectorSensor.tempDouble() == -127.0 ) {
    boilerSensor.updateTemp();
    collectorSensor.updateTemp();
  }
  latestVersion = releaseVersion;
  LCDHandler::wake();
  LCDHandler::changeText("loadingPage.t0", "Initalizing RTC module");
  Wire.begin(21, 22);

  if (!rtc.begin()) {
    //RTC
    LCDHandler::changeText("loadingPage.t0", "RTC module problem");
    Serial.println("Problem with RTC");
  }
  //now = rtc.now();

  LCDHandler::changeText("loadingPage.t0", "Initalizing BME sensor");

  while (!bme.begin())
  {
    LCDHandler::changeText("loadingPage.t0", "BME sensor problem");
    Serial.println("Could not find BME280 sensor!");
    delay(1000);
  }
  BMEUpdate();

  // begin SPIFFS and read data from it
  SPIFFSInitReadData();
    Serial.println("begin");
  LCDHandler::changeText("loadingPage.t0", "Connecting to WIFI");
  // connect to WIFI
  wifiConnect();

  int timezone = 2;
  int UTCOffsetInSecconds = 60*60*timezone;
  WiFiUDP ntpUDP;
  NTPClient timeClient(ntpUDP, "pool.ntp.org", UTCOffsetInSecconds);
  timeClient.begin();
  while(!timeClient.update()) {
  timeClient.forceUpdate();
  }
  String formattedDate = timeClient.getFormattedDate();
   int splitT = formattedDate.indexOf("T");
  char dayStamp[10];
  formattedDate.substring(0, splitT).toCharArray(dayStamp,10);
  // Extract time
 char timeStamp[5];
 formattedDate.substring(splitT+1, formattedDate.length()-1).toCharArray(timeStamp,5);
  DateTime date = DateTime(F(dayStamp), F(timeStamp));
  rtc.adjust(date);

  // init webserver
  LCDHandler::changeText("loadingPage.t0", "Initalizing Webserver");
  WebServerHandler::initWebserver();
  sensorUpdate(); //update sensors data

  resetStatistics(); //reset pump operating time statistics
  ledSetup();
  ledHandler(); //run ledhandler


  LCDHandler::switchPage(2);
  updateStatusPage();
  sendToThingspeak();
  Serial.println("setup successfuly completed");
  xTaskCreatePinnedToCore(
             sensorsTaskFunction, /* Task function. */
             "sensorsTask",   /* name of task. */
             10000,     /* Stack size of task */
             NULL,      /* parameter of the task */
             1,         /* priority of the task */
             &sensorsTask,    /* Task handle to keep track of created task */
             1);        /* pin task to core 0 */
}

void sensorsTaskFunction( void * pvParameters ){
  SerialHandler::handle();

  //do it every minute
  unsigned long currentMillis = millis();
  if (currentMillis - lastUpdate >= updateInterval) {

    sensorUpdate(); //update sensors data
    TempHandler();
    ledHandler();

    //update pumps operating time
    for (int i = 0; i < 4; i++) pumps[i].updateTime();
    
    //if disconnected from wifi reconnect
    if (WiFi.status() != WL_CONNECTED) wifiConnect(); // reconnect to WIFI

    //check if statistics needs to be cleared
    if (now.hour() == 0 && now.minute() == 0) {
      pumps[0].resetTime();
      resetStatistics();
    }

    //Update LCD statusPage
    updateStatusPage();

    lastUpdate = millis();
  }
  
  if (currentMillis - lastThingspeak >= thingspeakInterval) {
    sendToThingspeak();
    lastThingspeak = millis();
  }
}

void loop(void) {


}
