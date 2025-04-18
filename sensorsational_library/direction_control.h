#ifndef DIRECTION_CONTROL_H
#define DIRECTION_CONTROL_H

#include <Arduino.h>
#include <Servo.h>

#define TOLERANCE 5.0  // tolerance for the right angle

extern bool arms_deployed;

extern Servo servo_arms;
extern Servo escLeft;
extern Servo escRight;

void initMotors();
void adjustDirection();
void turnLeft();
void turnRight();
void moveForward();

#endif
