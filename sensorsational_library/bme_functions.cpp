#include "bme_functions.h"
#include "general_definitions.h"

//declare bme object
Adafruit_BME280 bme;

// BME variables
// saves temperature, pressure, altitude and humidity
float bme_data[4] = {0.0, 0.0, 0.0, 0.0};


bool setup_bme(Adafruit_BME280& bme_object)
{
  if (bme_object.begin(0x77, &Wire))
  {
    Serial.println("BME init success.");
    return true;
  }
  else
  {
    Serial.println("BME init failed!");
    return false;
  }
}

void get_bme_data(float bme_data[]) {
  // Read all sensor values and store them all in bme_data array
  bme_data[0] = bme.readTemperature();  // Temperature (°C)
  bme_data[1] = bme.readPressure() / 100.0F; // Pressure (hPa)
  bme_data[2] = bme.readAltitude(SEA_LEVEL_PRESSURE); // Altitude (meters)
  bme_data[3] = bme.readHumidity();  // Humidity (%)
}