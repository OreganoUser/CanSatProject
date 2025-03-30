#include "gps_functions.h"

Adafruit_GPS gps(&Wire);

//GPS variables
//gps fix, latitude, longitude, altitude, speed, angle
float gps_data[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
int gps_precisions[6] = {0, 5, 5, 1, 3, 0};

bool setup_gps(Adafruit_GPS& gps_object)
{
  if (gps_object.begin(0x10))
  {
    // we want recommended minimum navigation information (RMC) and global positioning system fix data (GGA)
    gps_object.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    // Set the update rate
    gps_object.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
    Serial.println("GPS init success.");
    delay(1000);
    // Request updates on antenna status, comment out to keep quiet
    gps_object.sendCommand(PGCMD_ANTENNA);

    // Ask for firmware version
    gps_object.println(PMTK_Q_RELEASE);
    return true;
  }
  else
  {
    Serial.println("GPS init failed!");
    delay(2500);
    return false;
  }
}

float convert_coord_to_decimal(float degmin) 
{
  // function converts coordinates from degrees, minutes, seconds to the decimal value
  int degrees = degmin / 100;         // Get the integer part (degrees)
  float minutes = degmin - (degrees * 100); // Get the decimal part (minutes)
  float decimalDegrees = degrees + (minutes / 60); // Convert minutes to decimal degrees
  return decimalDegrees;
}

void get_gps_data(float gps_data[]) {
  //function reads values from gps and updates gps_data if new position has been acquired
  unsigned long startTime = millis();

  // Read GPS data for 20ms to process incoming NMEA sentences
  while (millis() - startTime < 20) {
    char gps_data_read = gps.read();
  }

  if (gps.newNMEAreceived()) {
    if (gps.parse(gps.lastNMEA())) {
      if (gps.fix) {
        // gps fix, latitude, longitude, altitude, speed, angle
        gps_data[0] = gps.fix;
        gps_data[1] = convert_coord_to_decimal(gps.latitude);
        gps_data[2] = convert_coord_to_decimal(gps.longitude);
        gps_data[3] = gps.altitude;
        gps_data[4] = gps.speed;
        gps_data[5] = gps.angle;
      }
    }
  }
}