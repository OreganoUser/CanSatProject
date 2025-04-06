#ifndef GPS_FUNCTIONS_H
#define GPS_FUNCTIONS_H

#include <Wire.h>
#include <Adafruit_GPS.h>
#include "general_definitions.h"


extern Adafruit_GPS gps;
extern float gps_data[];
extern int gps_precisions[];
bool setup_gps(Adafruit_GPS& gps_object);
float convert_coord_to_decimal(float degmin);
void get_gps_data(float gps_data[]);

#endif // GPS_FUNCTIONS_H
