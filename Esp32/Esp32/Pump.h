class Pump
{
  private:
    String name;
    int pumpPin;
    bool normalState;
    bool pumpOperation;
    unsigned long lastOperatingTimeUpdate;
    bool enabled = true;
    
    int operatingHours, operatingMinutes, operatingSeconds, operatingMilliSeconds = 0;
    
  public:

    Pump(String name, int pin, bool state = false) {
      this->name = name;
      this->pumpPin = pin;
      this->normalState = state;
      //pinMode(this->pumpPin, OUTPUT);
      off();
    }

    String getName(){
      return this->name;
    }

    int showPin() {
      return this->pumpPin;
    }

    bool isOperating() {
      return this->pumpOperation;
    }

    bool isEnabled() {
      return this->enabled;
    }

    void disable() {
      this->enabled = false;
    }

    void enable() {
      this->enabled = true;
    }

    void on() {
      this->lastOperatingTimeUpdate = millis();

      if (this->normalState) PCF_01.write(this->pumpPin, LOW);
      else PCF_01.write(this->pumpPin, HIGH);
      
      this->pumpOperation = true;
    }

    void off() {
      this->lastOperatingTimeUpdate = 0;
      if (this->normalState) PCF_01.write(this->pumpPin, HIGH);
      else PCF_01.write(this->pumpPin, LOW);;

      this->pumpOperation = false;
    }


    long lastUpdate() {
      return this->lastOperatingTimeUpdate;
    }

    String operatingTime(String format) {
      format.replace("%H", String(this->operatingHours));
      format.replace("%M", String(this->operatingMinutes));
      format.replace("%S", String(this->operatingSeconds));
      format.replace("%ms", String(this->operatingMilliSeconds));
      return  format;
    }



    void updateTime() {

      if (this->lastOperatingTimeUpdate != 0) {
        unsigned long currentMillis = millis();
        unsigned long milliSeconds = currentMillis - this->lastOperatingTimeUpdate;
        
        this->operatingHours += milliSeconds / (3600 * 1000) % 24;
        this->operatingMinutes += (milliSeconds / (60 * 1000)) % 60;
        this->operatingSeconds += milliSeconds / 1000 % 60;
        this->operatingMilliSeconds += milliSeconds % 1000;
        
        if (this->operatingMilliSeconds >= 1000) {
          this->operatingSeconds++;
          this->operatingMilliSeconds = this->operatingMilliSeconds % 60;
        }
        
        if (this->operatingSeconds >= 60) {
          this->operatingMinutes++;
          this->operatingSeconds = this->operatingSeconds % 60;
        }

        if (this->operatingMinutes >= 60) {
          this->operatingHours++;
          this->operatingMinutes = this->operatingMinutes % 60;
        }
        
        if (this->operatingHours >= 24) this->operatingHours = this->operatingHours % 24;
        this->lastOperatingTimeUpdate = currentMillis;
      }
      if(this->operatingHours < 0) delay(5000);
    }

    void resetTime() {
      this->operatingHours = 0;
      this->operatingMinutes = 0;
      this->operatingSeconds = 0;
    }
};
