#include <OneWire.h>
#include <DallasTemperature.h>

class TempSensor
{
  private:
    int sensorPin;
    double sensorTemperature = -127;
    double sensorMaxTemperature = 0;
    double sensorMinTemperature = 0;
    double sensorAvgTemperature = 0;
    unsigned long numberOfReadings = 0;
    unsigned long avgSumHelper;
    OneWire* sensorOneWire;
    DallasTemperature* sensor;
    bool enabled = true;

  public:


    TempSensor(int pin) {
      this->sensorPin = pin;
      sensorOneWire = new OneWire(this->sensorPin);
      sensor = new DallasTemperature(this->sensorOneWire);
      sensor->begin();
      this->sensorMaxTemperature = 100.0;
      this->sensorMinTemperature = 100.0;
      this->sensorAvgTemperature = 100.0;
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
    int showPin() {
      return this->sensorPin;
    }

    String updateTemp() {
      if (this->enabled) {
        sensor->requestTemperatures();
        double temporary = sensor->getTempCByIndex(0);
        Serial.println(temporary);
        if (temporary == -127.00) return "Na";

        this->sensorTemperature = temporary;
        this->numberOfReadings++;
        this->avgSumHelper += sensorTemperature;

        if (this->sensorTemperature < this->sensorMinTemperature && !isnan(this->sensorTemperature)) this->sensorMinTemperature = this->sensorTemperature;
        if (this->sensorTemperature > this->sensorMaxTemperature && !isnan(this->sensorTemperature)) this->sensorMaxTemperature = this->sensorTemperature;

        return String(this->sensorTemperature);
      }
      else return "Na";
    }

    String temp() {
      if (this->enabled && this->sensorTemperature != -127) return String(this->sensorTemperature, 0);
      else return "Na";
    }

    double tempDouble() {
      if (this->enabled) return this->sensorTemperature;
      else return -127;
    }

    double avgTemp() {
      temp();
      this->sensorAvgTemperature = this->avgSumHelper / this->numberOfReadings;
      return this->sensorAvgTemperature;
    }

    String statistics(String format) {
      if (this->sensorTemperature != -127) format.replace("%Max", String(this->sensorMaxTemperature, 0));
      else format.replace("%Max", "Na");

      if (this->sensorTemperature != -127) format.replace("%Min", String(this->sensorMinTemperature, 0));
      else format.replace("%Min", "Na");

      if (this->sensorTemperature != -127) format.replace("%Avg", String(this->sensorAvgTemperature, 0));
      else format.replace("%Avg", "Na");

      if (this->sensorTemperature != -127) format.replace("%Cur", String(this->sensorTemperature, 0));
      else format.replace("%Cur", "Na");

      return  format;
    }

    void resetStatistics() {
      this->sensorMaxTemperature = sensorTemperature;
      this->sensorMinTemperature = sensorTemperature;
      this->sensorAvgTemperature = sensorTemperature;
      this->sensorTemperature = sensorTemperature;
    }


};
