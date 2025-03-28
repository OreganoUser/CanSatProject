#include "orientation.h"
#include <general_definitions.h>
#include "lsm9ds1_functions.h"
#include <SensorFusion.h>

SF orientation_filter;

float pitch, roll, yaw, Xh, Yh;
static float magheading;
float deltat;

float update_mag_heading(int execution_number)
{
	for (int i=0; i < execution_number; i++)
	{
		deltat = orientation_filter.deltatUpdate();
  		get_calibrated_lsm9ds1_data(lsm, calibrated_lsm_data, bias_corrected_magnetic);
		// invert y axis to get right-handed coordinate system.
		orientation_filter.MadgwickUpdate(calibrated_lsm_data[3], -calibrated_lsm_data[4], calibrated_lsm_data[5], calibrated_lsm_data[0] ,-calibrated_lsm_data[1], calibrated_lsm_data[2], calibrated_lsm_data[6], -calibrated_lsm_data[7], calibrated_lsm_data[8], deltat);
	  	
	  	// calculate euler angles
	  	pitch = orientation_filter.getPitch();
	  	roll = orientation_filter.getRoll();
	  	yaw = orientation_filter.getYaw();

	  	// calculating magnetic heading (much stabler than yaw calculation)
	  	// from https://avionicsduino.com/index.php/en/digital-compass/
	  	Xh = calibrated_lsm_data[6] * cos(pitch / 180 * PI) - calibrated_lsm_data[8] * sin(pitch / 180 * PI);
	  	Yh = -calibrated_lsm_data[6] * sin(roll / 180 * PI) * sin(pitch / 180 * PI) - calibrated_lsm_data[7] * cos(roll / 180 * PI) + calibrated_lsm_data[6] * sin(roll / 180 * PI) * cos(pitch / 180 * PI);
	  	magheading = 180 - atan2(Yh, Xh) * 180 / PI;
	  	if (magheading < 0)
	    	magheading += 360;		
	}
    return magheading;
}