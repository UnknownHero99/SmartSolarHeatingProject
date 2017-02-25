/*
  Name:		SolarCollectorController.ino
  Created:	6/18/2016 12:09:28 PM
  Author:	UnknownHero99
*/

#include <SoftReset.h>
#include <SdFat.h>
#include <EEPROM.h>
#include <MenuSystem.h>
#include <LiquidCrystal.h>
#include <SPI.h>
#include <Wire.h>
#include <RTClib.h>
#include <Encoder.h>
#include <DHT.h>
#include <SFE_BMP180.h>
#include <ArduinoJson.h>
#include "Pump.h"
#include "TempSensors.h"



//Pin settings
#define lcdBrightnessPin 2
#define lcdContrastPin 3
#define DHTPin 31
#define encoderCLKPin 8
#define encoderDTPin 9
#define encoderSWPin 10
#define boilerSensorPin 4
#define collectorSensorPin 5
#define t1SensorPin 6
#define t2SensorPin 7
#define ChipSelect 53

SFE_BMP180 pressure;

TempSensor boilerSensor(boilerSensorPin);
TempSensor collectorSensor(collectorSensorPin);
TempSensor t1Sensor(t1SensorPin);
TempSensor t2Sensor(t2SensorPin);

Pump pumps[4] = { Pump("1", 23), Pump("2", 25), Pump("3", 27), Pump("4", 29) };

DHT dht(DHTPin, DHT22);

Encoder myEnc(encoderDTPin, encoderCLKPin);

SdFat SD;
File logfile;

RTC_DS1307 rtc;
DateTime now;

LiquidCrystal lcd(39, 41, 43, 45, 47, 49);

long oldPosition = myEnc.read() / 4;
double roomTemp, roomHumidity = 50;
int roomPressure = 1111;
unsigned long lastUpdate, lastAction = 0;
unsigned long statusDelay = 10L * 1000L; //quit menu after ms of no use
unsigned long sleepDelay = 30L * 1000L; //sleep after ms of no nuse
unsigned long updateInterval = 10L * 1000L;//sensor update interval
unsigned long logInterval = 300L * 1000L;//Log and esp to thingspeak interval

int status = 1;
bool autoMode = true;
bool sleeping = false;
int buttonState = 0;
int lastButtonState = 0;

int current;
String IP;
long millisHeld;
long secsHeld;
long prevSecsHeld;
byte previous = HIGH;
int settingsMinTempDifference, settingsMinTempCollector, settingsMaxTempCollector, settingsMaxTempBoiler, altitude, brightness, contrast;
double roomMinTemp, roomMaxTemp, roomMaxHumidity, roomMinHumidity, roomMinPressure, roomMaxPressure;
unsigned long lastLog, firstTime;


//rgb pins
int redPin = 13;
int greenPin = 11;
int bluePin = 12;

bool sdProblem = false;
bool pressureProblem = false;


void setup() {
	eeprom_read_config();
	wake();

	Serial.begin(9600);
	Serial2.begin(115200);

	dht.begin();

	if (pressure.begin())
		Serial.println("BMP180 init success");
	else
	{
		Serial.println("BMP180 init fail");
		pressureProblem = true;
	}

	
	if (!rtc.begin()) {
		//RTC
		Wire.begin();
		Serial.println("Problem with RTC");
	}
	now = rtc.now();

	if (!SD.begin(ChipSelect)) {
		lcd.clear();
		lcd.print("Problem with SD card");
		delay(1000);
		sdProblem = true;
	}

	sensorUpdate();
	resetStatistics();
	
	sdcardwrite();
	ledHandler();
	sendData();
	sendSettings();
	sendPumps();
	menu_setup();
}


void loop() {

	//esp8266 check for commands
	serialhandler();

	//log on sd card
	if (millis() - lastLog >= logInterval) sdcardwrite();

	//reset min and max statistics
	if (now.hour() == 0 && now.minute() == 0) {
		pumps[0].resetTime();
		resetStatistics();
	}

	//update time to display
	pumps[0].updateTime();


	//quit menu if you havent used encoder for some time
	if (millis() - lastAction >= statusDelay && status == 0) {
		status = 1;
		lcd.clear();
		statuslcd();
	}

	//go to sleep if you havent used encoder for some time
	if (millis() - lastAction >= sleepDelay && !sleeping) sleep();

	//update sensor values, use tempHandler and ledHandler
	if (millis() - lastUpdate >= updateInterval) {
		sensorUpdate();
		TempHandler();
		ledHandler();
	}

	if (status != 0) {
		menuEntranceHandler();
	}

	//run controlhandler if in menu
	if (status == 0 || sleeping)controlhandler();
}

