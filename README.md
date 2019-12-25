# Smart Solar Heating Project
ESP32 based water heating collector controler 

## Components:
- Esp32
- 4x ds18b20 temperature sensors
- BME280 air pressure/temperature sensor
- Rtc
- 3.5" Nextion TFT lcd
- Rgb led diode (for status signalization)
- up to 4 relays

## Features:
- manual/automatic pump(relay) control
- min temp difference , if temp difference is lower than this pump wont be enabled
- max temp of boiler , if boiler is higher than this pupm will stop
- min temp of collector , if temp of collector is lowet than this pump wont be enabled
- max temp of collector , if temp of collecter is higher than this and boiler temp is lower than max temp of boiler pump will be enabled
- RTC time
- altitude(for pressure sensor)

## Features:
- Smart pump control
- web interface
- Log to thingspeak, displaying graphs in web interface
- Statistics (Max Min of temperatures, pressure etc.)
- Current sensor data
- RGB signalization

