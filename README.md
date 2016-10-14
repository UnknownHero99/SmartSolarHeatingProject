# SolarCollectorController
It's a program for solar collector controller, using arduino and esp8266.

## Parts used:
- Arduino
- Esp8266
- 2x ds18b20 temperature sensors
- Dht22
- BMP180 pressure sensor (for enviroment, not for water pressure in system)
- Encoder
- Sdcard module
- Rtc module
- 20x4 lcd
- Rgb led diode (for status signalization)

## You can set:
- manual controll(on off) or automatic
- min temp difference , if temp difference is lower than this pump wont be enabled
- max temp of boiler , if boiler is higher than this pupm will stop
- min temp of collector , if temp of collector is lowet than this pump wont be enabled
- max temp of collector , if temp of collecter is higher than this and boiler temp is lower than max temp of boiler pump will be enabled
- RTC time
- altitude(for pressure sensor)

## Features:
- Log to SD card
- Smart pump control
- Esp8266 web interface to control pump options are Pump on, Pump off and Automatic(in development)
- Log to thingspeak, displaying graphs in esp8266 web interface
- Statistics (Max Min of temperatures, pressure etc.)
- Current sensor data
- Encoder controlled
- RGB signalization

## RGB signalization:
- Green, pump is operating
- Violet, pump is not operating
- Red, overheated
- Blue, sensor problem, check serial monitor for more details
- Orange, SD card problem

## Pins configuration:
- 3 -> LCD A	LCD brightness control
- 4 -> LCD V0 	LCD Contrast control
- 5 -> Boiler temp sensor data pin
- 6 -> Collector temp sensor data pin
- 7 -> t1 temp sensor data pin
- 8 -> t2 temp sensor data pin
- 9 -> RGB led blue pin
- 10 -> RGB led green pin
- 11 -> RGB led red pin
- 16, 17 -> esp communication
- 18 -> Encoder DT pin
- 19 -> Encoder CLK pin
- 20 -> RTC SDA pin
- 21 -> RTC SCL pin
- 28 -> Encoder SW pin
- 29 -> DHT22 data pin
- 30 -> Relay trig pin
- 39 -> LCD RS pin
- 41 -> LCD E pin
- 43 -> LCD D4 pin
- 45 -> LCD D5 pin
- 47 -> LCD D6 pin
- 49 -> LCD D7 pin 
- 50 -> SD card module MISO
- 51 -> SD card module MOSI
- 52 -> SD card module SCK
- 53 -> SD card module CS



