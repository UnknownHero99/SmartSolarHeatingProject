class MyRenderer : public MenuComponentRenderer
{
public:
	virtual void render(Menu const& menu) const
	{
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("Controller Menu");
		lcd.setCursor(0, 1);
		menu.get_current_component()->render(*this);
	}

	virtual void render_menu_item(MenuItem const& menu_item) const
	{
		lcd.print(menu_item.get_name());
	}

	virtual void render_back_menu_item(BackMenuItem const& menu_item) const
	{
		lcd.print(menu_item.get_name());
	}

	virtual void render_numeric_menu_item(NumericMenuItem const& menu_item) const
	{
		lcd.print(menu_item.get_name());
	}

	virtual void render_menu(Menu const& menu) const
	{
		lcd.print(menu.get_name());
	}
};
MyRenderer my_renderer;

// Menu variables

MenuSystem ms(my_renderer);
//MenuItem espIP("web ui IP", &reqestIp);
Menu pumpModeMenu("Pump mode");
MenuItem pumpModeAutoMenu("Automatic", &pumpAutoModeLcd);
Menu pumpModeManualMenu("Manual");
MenuItem pumpModeOnMenu("Pump - on", &pumpOnModeLcd);
MenuItem pumpModeOffMenu("Pump - off", &pumpOffModeLcd);
Menu settingsMenu("Settings");
MenuItem RTCTimeSetMenu("set RTC Time", &RTCTimeSetLcd);
MenuItem tempdifmenu("Set min temp diff", &minTempDifferenceLcd);
MenuItem minTempCollectorMenu("Set min coll. T", &minTempCollectorLcd);
MenuItem maxTempCollectorMenu("Set max coll. T", &maxTempCollectorLcd);
MenuItem maxTempBoilerMenu("set max boiler T", &maxTempBoilerLcd);
MenuItem altitudeMenu("set altitude", &altitudeLcd);
Menu resetRebootMenu("Reboot/reset");
MenuItem rebootMenu("Reboot", &rebootLcd);
MenuItem resetMenu("Reset", &resetEepromLcd);
Menu lcdSettingsMenu("LCD Settings");
MenuItem brightnessMenu("Set Brightness", &brightnessLcd);
MenuItem contrastMenu("Set Contrast", &contrastLcd);
MenuItem exitMenu("Exit", &exitLcd);
MenuItem backMenu("back", &backLcd);





void menu_setup()
{
	lcd.begin(20, 4);
	ms.get_root_menu().add_menu(&pumpModeMenu);
	pumpModeMenu.add_item(&pumpModeAutoMenu);
	pumpModeMenu.add_menu(&pumpModeManualMenu);
	pumpModeManualMenu.add_item(&pumpModeOnMenu);
	pumpModeManualMenu.add_item(&pumpModeOffMenu);
	pumpModeManualMenu.add_item(&backMenu);
	pumpModeMenu.add_item(&backMenu);
	ms.get_root_menu().add_menu(&settingsMenu);
	settingsMenu.add_item(&RTCTimeSetMenu);
	settingsMenu.add_item(&tempdifmenu);
	settingsMenu.add_item(&minTempCollectorMenu);
	settingsMenu.add_item(&maxTempCollectorMenu);
	settingsMenu.add_item(&maxTempBoilerMenu);
	settingsMenu.add_item(&altitudeMenu);
	settingsMenu.add_menu(&lcdSettingsMenu);
	lcdSettingsMenu.add_item(&brightnessMenu);
	lcdSettingsMenu.add_item(&contrastMenu);
	lcdSettingsMenu.add_item(&backMenu);
	settingsMenu.add_menu(&resetRebootMenu);
	resetRebootMenu.add_item(&rebootMenu);
	resetRebootMenu.add_item(&resetMenu);
	resetRebootMenu.add_item(&backMenu);
	settingsMenu.add_item(&backMenu);
	ms.get_root_menu().add_item(&exitMenu);
}

void exitLcd(MenuItem* p_menu_item)
{
	lcd.clear();
	ms.reset();
	status = 1;
	statuslcd();
}

void backLcd(MenuItem* p_menu_item)
{
	ms.back();
	ms.display();
}

void controlhandler() {
	long newPosition = myEnc.read() / 4;
	buttonState = digitalRead(encoderSWPin);
	if (newPosition > oldPosition)
	{
		lastAction = millis();
		if (sleeping) {
			status = 1;
			lcdUpdate();
			wake();
		}
		else {
			if (status == 0)
			{
				ms.next();
				ms.display();
			}
		}
	}
	else if (newPosition < oldPosition) //left
	{
		lastAction = millis();
		if (sleeping) {
			status = 1;
			lcdUpdate();
			wake();
		}

		else {
			if (status == 0)
			{
				ms.prev();
				ms.display();
			}
		}
	}

	if (newPosition != oldPosition)
	{
		oldPosition = newPosition;
	}

	if (buttonState != lastButtonState) {
		if (buttonState == LOW) {
			lastAction = millis();
			if (sleeping) wake();
			else {
				if (status == 0)
					ms.select();
				ms.display();
			}
		}
	}

	lastButtonState = buttonState;

}



void statuslcd() {
	lcd.setCursor(0, 0);
	if (now.hour() < 10) lcd.print("0");
	lcd.print(now.hour());
	lcd.print(":");
	if (now.minute() < 10) lcd.print("0");
	lcd.print(now.minute());
	lcd.print(" ");
	lcd.print(now.day());
	lcd.print(".");
	lcd.print(now.month());
	lcd.print(".");
	lcd.print(now.year());
	lcd.print("      ");
	lcd.setCursor(0, 1);
	lcd.print("Tb: ");
	lcd.print(boilerSensor.temp());
	lcd.print("  ");
	lcd.setCursor(10, 1);
	lcd.print("Tr: ");
	lcd.print(roomTemp);
	lcd.setCursor(0, 2);
	lcd.print("Tc: ");
	lcd.print(collectorSensor.temp());
	lcd.print("  ");
	lcd.setCursor(10, 2);
	lcd.print("Hu: ");
	lcd.print(roomHumidity);
	lcd.print("%");
	lcd.setCursor(0, 3);
	lcd.print("P:");
	if (mainPump.isOperating()) lcd.print("On      ");
	else lcd.print("Off     ");
	lcd.print("  ");
	lcd.setCursor(10, 3);
	lcd.print("Pr: ");
	lcd.print(roomPressure);
	lcd.print("mB");
}

void statuslcd2() {
	lcd.setCursor(0, 0);
	lcd.print(now.day());
	lcd.print(".");
	lcd.print(now.month());
	lcd.setCursor(5, 0);
	lcd.print("Tc:");
	lcd.setCursor(9, 0);
	lcd.print("Tb:");
	lcd.setCursor(13, 0);
	lcd.print("Pr:");
	lcd.setCursor(18, 0);
	lcd.print("Hu");
	lcd.setCursor(0, 1);
	lcd.print("MAX ");
	lcd.setCursor(5, 1);
	lcd.print(collectorSensor.statistics("%Max"));
	lcd.print(" ");
	lcd.setCursor(9, 1);
	lcd.print(boilerSensor.statistics("%Max"));
	lcd.print(" ");
	lcd.setCursor(13, 1);
	lcd.print(roomMaxPressure, 0);
	lcd.print(" ");
	lcd.setCursor(18, 1);
	lcd.print(roomMaxHumidity, 0);
	lcd.setCursor(0, 2);
	lcd.print("MIN ");
	lcd.setCursor(5, 2);
	lcd.print(collectorSensor.statistics("%Min"));
	lcd.print(" ");
	lcd.setCursor(9, 2);
	lcd.print(boilerSensor.statistics("%Min"));
	lcd.print(" ");
	lcd.setCursor(13, 2);
	lcd.print(roomMinPressure, 0);
	lcd.print(" ");
	lcd.setCursor(18, 2);
	lcd.print(roomMinHumidity, 0);
	lcd.setCursor(0, 3);
	lcd.print("Pump: ");
	lcd.print(mainPump.operatingTime("%Hh %Mm     "));
}

void statuslcd3() {
	lcd.setCursor(0, 0);
	lcd.print(now.day());
	lcd.print(".");
	lcd.print(now.month());
	lcd.setCursor(5, 0);
	lcd.print("T1:");
	lcd.setCursor(9, 0);
	lcd.print("T2:");
	lcd.setCursor(0, 1);
	lcd.print("Cur: ");
	lcd.setCursor(0, 2);
	lcd.print("Min: ");
	lcd.setCursor(0, 3);
	lcd.print("Max: ");
	lcd.setCursor(5, 1);
	lcd.print(t1Sensor.temp());
	lcd.print(" ");
	lcd.setCursor(9, 1);
	lcd.print(t2Sensor.temp());
	lcd.print(" ");
	lcd.setCursor(5, 2);
	lcd.print(t1Sensor.statistics("%Min"));
	lcd.print(" ");
	lcd.setCursor(9, 2);
	lcd.print(t2Sensor.statistics("%Min"));
	lcd.print(" ");
	lcd.setCursor(5, 3);
	lcd.print(t1Sensor.statistics("%Max"));
	lcd.print(" ");
	lcd.setCursor(9, 3);
	lcd.print(t2Sensor.statistics("%Max"));
	lcd.print(" ");

}

// used for setting values in eeprom, in name you write what it display on lcd, value is number where it starts,
//eepromnumver is where to write in eeprom set it to 999 if you dont want to write in eeprom,
//upperlimit is to limit how high setting number can get and lowerlimit is how low it can get
int settings_lcd(String name, int value, int eepromnumber, int upperlimit = 0, int lowerlimit = 0) {
	lcd.clear();
	lcd.print(name);
	lcd.print(":");
	lastButtonState = 0;
	delay(200);
	while (true)
	{
		if (millis() - lastUpdate >= updateInterval) {
			sensorUpdate();
			TempHandler();
		}

		lcd.setCursor(0, 1);
		if (upperlimit != 0 && upperlimit > 0 && value > upperlimit) value = upperlimit;
		if (value < lowerlimit) value = lowerlimit;
		lcd.print(value);
		lcd.print("  ");
		long newPosition = myEnc.read() / 4;
		buttonState = digitalRead(encoderSWPin);
		if (newPosition > oldPosition)//right
		{
			value++;
		}
		else if (newPosition < oldPosition) //left
		{
			value--;
		}

		if (newPosition != oldPosition)
		{
			oldPosition = newPosition;
		}
		if (buttonState != lastButtonState) {
			if (buttonState == LOW) {
				break;
			}
		}
		lastButtonState = buttonState;
	}
	lastAction = millis();
	if (eepromnumber != 999) EEPROM.update(eepromnumber, value);
	ms.display();
	return value;
}

void RTCTimeSetLcd(MenuItem* p_menu_item) {
	int dan = settings_lcd("Day in month", now.day(), 999, 31, 1);
	int mesec = settings_lcd("Month", now.month(), 999, 12, 1);
	int leto = settings_lcd("Year", now.year(), 999);
	int ure = settings_lcd("Hour", now.hour(), 999, 24);
	int minute = settings_lcd("Minute", now.minute(), 999, 59);
	int sekunde = settings_lcd("Second", now.second(), 999, 59);
	rtc.adjust(DateTime(leto, mesec, dan, ure, minute, sekunde));
	sensorUpdate();
}

void resetEepromLcd(MenuItem* p_menu_item) {
	EEPROM.update(0, 0);
	EEPROM.update(1, 0);
	EEPROM.update(2, 0);
	EEPROM.update(3, 0);
	EEPROM.update(6, 0);
	EEPROM.update(7, 0);
	EEPROM.update(8, 0);
	EEPROM.update(9, 0);
	ms.back();
	ms.display();
}

void rebootLcd(MenuItem* p_menu_item) {
	soft_restart();
}



void minTempDifferenceLcd(MenuItem* p_menu_item) {
	settingsMinTempDifference = settings_lcd("Min temp diff", settingsMinTempDifference, 0);
}

void minTempCollectorLcd(MenuItem* p_menu_item)
{
	settingsMinTempCollector = settings_lcd("Min temp of coll.", settingsMinTempCollector, 1);
}

void maxTempCollectorLcd(MenuItem* p_menu_item)
{
	settingsMaxTempCollector = settings_lcd("Max temp of coll.", settingsMaxTempCollector, 2);
}

void maxTempBoilerLcd(MenuItem* p_menu_item)
{
	settingsMaxTempBoiler = settings_lcd("Max Temp of boiler", settingsMaxTempBoiler, 3);
}

void contrastLcd(MenuItem* p_menu_item) {
	contrast = map(settings_lcd("Contrast", map(contrast, 255, 0, 0, 100), 999, 100), 0, 100, 255, 0);
	EEPROM.update(4, contrast);
	wake();
}

void brightnessLcd(MenuItem* p_menu_item) {
	brightness = map(settings_lcd("Brightness", map(brightness, 0, 255, 0, 100), 999, 100), 0, 100, 0, 255);
	EEPROM.update(5, brightness);
	wake();
}

void altitudeLcd(MenuItem* p_menu_item) {
	settings_lcd("Altitude thousands", EEPROM.read(9), 9, 9);
	settings_lcd("Altitude hundreds", EEPROM.read(8), 8, 9);
	settings_lcd("Altitude tens", EEPROM.read(7), 7, 9);
	settings_lcd("Altitude ones", EEPROM.read(6), 6, 9);
	altitude = eeprommultiplace(6, 7, 8, 9);
	lcd.clear();
	lcd.print("set Altitude: ");
	lcd.setCursor(0, 1);
	lcd.print(altitude);
	delay(2000);
}




void pumpAutoModeLcd(MenuItem* p_menu_item) {
	autoMode = true;
	ms.back();
	ms.display();
}

void pumpOnModeLcd(MenuItem* p_menu_item) {
	autoMode = false;
	mainPump.on();
	ms.back();
	ms.back();
	ms.display();
}

void pumpOffModeLcd(MenuItem* p_menu_item) {
	autoMode = false;
	mainPump.off();
	ms.back();
	ms.back();
	ms.display();
}

void reqestIp(MenuItem* p_menu_item) {
	Serial2.print("GetIP();");
	ms.reset();
	lcd.clear();
	lcd.print("IP of esp:");
	lcd.println(IP);
	delay(5000);
	statuslcd();
}

void sleep() {
	sleeping = true;
	analogWrite(lcdContrastPin, 255);
	analogWrite(lcdBrightnessPin, 0);
	ms.reset();
}

void wake() {
	sleeping = false;
	analogWrite(lcdContrastPin, contrast);
	analogWrite(lcdBrightnessPin, brightness);

}

void menuEntranceHandler() {
	current = digitalRead(encoderSWPin);
	long newPosition = myEnc.read() / 4;
	if (current == LOW && previous == HIGH && (millis() - firstTime) > 200) {
		firstTime = millis();
	}

	millisHeld = (millis() - firstTime);
	secsHeld = millisHeld / 1000;

	if (millisHeld > 50) {

		if (current == HIGH && previous == LOW && secsHeld >= 2) {
			status = 0;
			ms.display();
			lastAction = millis();
			delay(200);
		}
	}

	if (newPosition > oldPosition)//right
	{
		lastAction = millis();
		status++;
		if (status >= 4) status = 1;
		lcd.clear();
		lcdUpdate();

	}
	else if (newPosition < oldPosition) //left
	{
		lastAction = millis();
		status--;
		if (status <= 0) status = 3;
		lcd.clear();
		lcdUpdate();
	}

	if (newPosition != oldPosition)
	{
		oldPosition = newPosition;
	}

	previous = current;
	prevSecsHeld = secsHeld;
}

void lcdUpdate() {

	switch (status) {
	case 1:
		statuslcd();
		break;
	case 2:
		statuslcd2();
		break;
	case 3:
		statuslcd3();
		break;
	}
}


