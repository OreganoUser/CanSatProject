#include "direction_control.h"
#include <math.h>



void adjustDirection(float targetLat, float targetLon) {
  float currentLat = gps_data[1];
  float currentLon = gps_data[2];
  float currentHeading = calibrated_lsm_data[8];

  float phi1 = currentLat * M_PI / 180.0;
  float phi2 = targetLat * M_PI / 180.0;
  float dLon = (targetLon - currentLon) * M_PI / 180.0;

  // Variablen für atan2
  float y = sin(dLon) * cos(phi2);
  float x = cos(phi1) * sin(phi2) - sin(phi1) * cos(phi2) * cos(dLon);

  // Zielwinkel
  float targetHeading = atan2(y, x) * 180.0 / M_PI;
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

void aimSatellite() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    if (input.startsWith("AIM")) {
      int firstSpace = input.indexOf(' ');
      if (firstSpace != -1) {
        String coords = input.substring(firstSpace + 1);
        coords.trim();
        int spaceIndex = coords.indexOf(' ');
        if (spaceIndex != -1) {
          String latStr = coords.substring(0, spaceIndex);
          String lonStr = coords.substring(spaceIndex + 1);
          targetLat = latStr.toFloat();
          targetLon = lonStr.toFloat();
          targetSet = true;
          Serial.println("Zielkoordinaten gesetzt!");
        }
      }
    }
  }
}
