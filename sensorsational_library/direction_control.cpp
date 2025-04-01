#include "direction_control.h"
#include <math.h>
#include "gps_functions.h" // needed to access current pos
#include "orientation.h" // needed to access current heading
#include "general_definitions.h" // needed to access target pos


void adjustDirection()
{
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

  if (delta < TOLERANCE || delta > 360 - TOLERANCE) {
    moveForward();
  } else if (delta > 180) {
    turnLeft();
  } else {
    turnRight();
  }
}

void turnLeft() {
    Serial.println("Turning Left");
}
 
void turnRight() {
    Serial.println("Turning Right");
}
 
void moveForward() {
    Serial.println("Vollgas!");
}