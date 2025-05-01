#include "direction_control.h"
#include "flight_stages.h"
#include "math.h"
#include "Servo.h"
#include "Arduino.h"
#include "gps_functions.h" // needed to access current pos
#include "orientation.h" // needed to access current heading
#include "general_definitions.h" // needed to access target pos

extern bool arms_deployed;
Servo servo_arms;

Servo escLeft;
Servo escRight;


void initMotors() {
  servo_arms.attach(SERVO_PIN);
  servo_arms.write(200);
  escLeft.attach(MOTOR1_PIN);
  escRight.attach(MOTOR2_PIN);
}

void adjustDirection()
{

  if(flight_stage == 3){
    Serial.println("Deploying Arms");
    servo_arms.write(10);
    arms_deployed = true;
  } else {
    servo_arms.write(190);
    arms_deployed = false;
    escLeft.writeMicroseconds(0);
    escRight.writeMicroseconds(0);
  }

  if(arms_deployed && flight_stage == 3){

  // this function gets current position from gps and current magnetic heading from orientation
  // and calculates the needed adjustment to head towards target coordinates
  // the target coordinates are defined in general_definitions.h as TARGET_LAT and TARGET_LON
  float currentLat = gps_data[1];
  float currentLon = gps_data[2];
  float currentHeading = orientation_data[0]; //magheading

  float phi1 = currentLat * PI / 180.0;
  float phi2 = TARGET_LAT * PI / 180.0;
  float dLon = (TARGET_LON - currentLon) * PI / 180.0;

  // Variablen für atan2
  float y = sin(dLon) * cos(phi2);
  float x = cos(phi1) * sin(phi2) - sin(phi1) * cos(phi2) * cos(dLon);

  // Zielwinkel
  float targetHeading = atan2(y, x) * 180.0 / PI;
  targetHeading = fmod(targetHeading + 360, 360);

  // Winkelabweichung
  float delta = fmod(targetHeading - currentHeading + 360, 360);

  Serial.println(delta);

  if (delta < TOLERANCE || delta > 360 - TOLERANCE) {
    moveForward();
  } else if (delta > 180) {
    turnLeft(delta);
  } else {
    turnRight(delta);
  }
}
}

void turnLeft(float delta) {
  Serial.println("Turning Left");
  int speedAdjustment = constrain(-(delta-360), 0, 100);
  Serial.print("adj");
  Serial.println(speedAdjustment);
  escLeft.writeMicroseconds(2000 - speedAdjustment*0.2);
  escRight.writeMicroseconds(2000);
}
 
void turnRight(float delta) {
  Serial.println("Turning Right");
  int speedAdjustment = constrain(delta, 0, 100);
  Serial.print("adj");
  Serial.println(speedAdjustment);
  escLeft.writeMicroseconds(2000);
  escRight.writeMicroseconds(2000 - speedAdjustment*0.2);
}
 
void moveForward() {
    Serial.println("Vollgas!");
    escLeft.writeMicroseconds(2000);
    escRight.writeMicroseconds(2000);
}