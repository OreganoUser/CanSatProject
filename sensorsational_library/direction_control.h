#ifndef DIRECTION_CONTROL_H
#define DIRECTION_CONTROL_H

#include <Arduino.h>

#define TOLERANCE 5.0  // tolerance for the right angle

void adjustDirection(float targetLat, float targetLon);
void turnLeft();
void turnRight();
void moveForward();
void aimSatellite();

extern float targetLat;
extern float targetLon;
extern bool targetSet;
extern float gps_data[9];
extern float calibrated_lsm_data[9];

#endif
