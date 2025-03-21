#ifndef LSM9DS1_FUNCTIONS_H
#define LSM9DS1_FUNCTIONS_H

#include "general_definitions.h"
#include <Adafruit_LSM9DS1.h>
#include <Wire.h>


extern Adafruit_LSM9DS1 lsm;
extern float lsm_data[9];
extern float calibrated_lsm_data[9];
extern float bias_corrected_magnetic[3];
extern sensors_event_t a, m, g, temp;
bool setup_lsm9ds1(Adafruit_LSM9DS1& lsm_object);
void get_lsm9ds1_data(Adafruit_LSM9DS1& lsm_object, float lsm9ds1_data[9]);
void get_calibrated_lsm9ds1_data(Adafruit_LSM9DS1& lsm_object, float calibrated_data[9], float bias_corrected[3]);

#endif //LSM9DS1_FUNCTIONS_H