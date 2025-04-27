#include "bme_functions.h"
#include "general_definitions.h"

//declare bme object
Adafruit_BME280 bme;

// BME variables
// saves temperature, pressure, altitude and humidity
float bme_data[4] = {0.0, 0.0, 0.0, 0.0};
int bme_precisions[4] = {1, 1, 1, 0};


bool setup_bme(Adafruit_BME280& bme_object)
{
  if (bme_object.begin(0x77, &Wire))
  {
    Serial.println("BME init success.");
    bme.setSampling(
    Adafruit_BME280::MODE_FORCED,       
    Adafruit_BME280::SAMPLING_X2,        // Temperature oversampling = 2x (more stable, still fast)
    Adafruit_BME280::SAMPLING_X16,       // Pressure oversampling = 16x (very stable altitude readings)
    Adafruit_BME280::SAMPLING_X1,        // Humidity oversampling = 1x (if you care about humidity, raise this too)
    Adafruit_BME280::FILTER_X16           // IIR filter on, level 16 (smooths fast noise/bursts)
    );
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