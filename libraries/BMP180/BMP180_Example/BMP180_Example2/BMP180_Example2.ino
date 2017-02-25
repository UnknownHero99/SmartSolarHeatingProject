//---Example BMP180 sketch by Benbojangles 2013----//
//---This sketch will make your altitude zero when you press the reset button---//
//---Allowing you to know increases and decreases in altitude based on your initial location---//
//---Altitude is in Meters---//



// Include the Wire library for I2C access.
#include <Wire.h>
// Include the BMP180 library.
#include <BMP180.h>

//------------ Store an instance of the BMP180 sensor. ----------------//
BMP180 barometer;
//------------ Use the on board LED for an indicator. -----------------//
int indicatorLed = 13; 


//--- Set current pressure & altitude, in this example it will be zero when we press reset button. ---//
int initialPressure = 0;
int initialAltitude = 0;

void setup()
{
  initialPressure =  barometer.GetPressure();
  initialAltitude = barometer.GetAltitude(initialPressure);
  
  long currentPressure = barometer.GetPressure();
  float altitude = barometer.GetAltitude(initialPressure);
  // Start the serial library to output our messages.
  Serial.begin(9600);
  // Start the I2C on the Arduino for communication with the BMP180 sensor.
  Wire.begin();
  // Set up the Indicator LED.
  pinMode(indicatorLed, OUTPUT);
  // Check to see if connected to the sensor.
  if(barometer.EnsureConnected())
  {
    Serial.println("Connected to BMP180."); // Output - connected to the computer.
    digitalWrite(indicatorLed, HIGH); // Set our LED.
    
     // When connected, reset the device to ensure a clean start.
    barometer.SoftReset();
    // Initialize the sensor and pull the calibration data.
    barometer.Initialize();
  }
  else
  { 
    Serial.println("Not connected to BMP180.");
    digitalWrite(indicatorLed, LOW); // Set our LED.
  }
}

void loop()
{
  if(barometer.IsConnected)
  {
    // Retrive the current pressure in Pascals.
    long currentPressure = barometer.GetPressure();
    
    // Retrive the current altitude (in meters).
    float altitude = barometer.GetAltitude(currentPressure);
    
    // Print out the Altitude.
    Serial.print("\tAltitude: ");
    Serial.print(altitude);
    Serial.print(" m");
    
    // Retrive the current temperature in degrees celcius.
    float currentTemperature = barometer.GetTemperature();
    
    Serial.println(); // Start a new line.
    delay(500); // Show new results every half second.
  }
}
