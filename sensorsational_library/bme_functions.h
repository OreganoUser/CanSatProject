#ifndef BME_FUNCTIONS_H
#define BME_FUNCTIONS_H

#include <Adafruit_BME280.h>
#include <Wire.h>

extern Adafruit_BME280 bme;
extern float bme_data[];
bool setup_bme(Adafruit_BME280& bme_object);
void get_bme_data(float bme_data[]);

#endif // BME_FUNCTIONS_H