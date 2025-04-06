#ifndef CALIBRATION_FUNCTIONS_H
#define CALIBRATION_FUNCTIONS_H
#include "calibration_parameters.h"
#include "general_definitions.h"
#include "lsm9ds1_functions.h"
#include <Adafruit_LSM9DS1.h>

// Tunable parameters
extern const int numSamples; // Number of samples to take for each orientation
extern const float knownAngle; // Known rotation angle in degrees for gyro calib
extern float samplingRate; // Default to 119Hz (adjust based on setup)

void waitForUserInput();
void calibrateAccelerometer(Adafruit_LSM9DS1 &imu_object, float aX[2], float aY[2], float aZ[2]);
void measureAccAxis(const char* description, Adafruit_LSM9DS1 &imu_object, float a[2]);
void calibrateGyroscope(Adafruit_LSM9DS1 &imu_object);
void measureGyroAxis(char axis, Adafruit_LSM9DS1 &imu_object);
void calibrateMagnetometer(Adafruit_LSM9DS1 &imu_object, float mX[2], float mY[2], float mZ[2]);

#endif // CALIBRATION_FUNCTIONS_H