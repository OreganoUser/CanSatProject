#ifndef DIRECTION_CONTROL_H
#define DIRECTION_CONTROL_H

#include <Arduino.h>

#define TOLERANCE 5.0  // tolerance for the right angle

extern bool arms_deployed;
void adjustDirection();
void turnLeft();
void turnRight();
void moveForward();

#endif
