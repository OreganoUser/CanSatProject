#include "orientation.h"
#include <general_definitions.h>
#include "lsm9ds1_functions.h"
#include <SensorFusion.h>

// define inertial measurement filter
SF orientation_filter;

// orientation data
// magheading, yaw, roll, pitch
float orientation_data[4] = {0.0, 0.0, 0.0, 0.0};
int orientation_precisions[4] = {0, 0, 0, 0};

float Xh, Yh;
float deltat;

void update_mag_heading(int execution_number)
{
	for (int i=0; i < execution_number; i++)
	{
		deltat = orientation_filter.deltatUpdate();
  		get_calibrated_lsm9ds1_data(lsm, calibrated_lsm_data, bias_corrected_magnetic);
		// invert y axis to get right-handed coordinate system.
		orientation_filter.MadgwickUpdate(calibrated_lsm_data[3], -calibrated_lsm_data[4], calibrated_lsm_data[5], calibrated_lsm_data[0] ,-calibrated_lsm_data[1], calibrated_lsm_data[2], calibrated_lsm_data[6], -calibrated_lsm_data[7], calibrated_lsm_data[8], deltat);
	  	
	  	// calculate euler angles
	  	orientation_data[3] = orientation_filter.getPitch();
	  	orientation_data[2] = orientation_filter.getRoll();
	  	orientation_data[1] = orientation_filter.getYaw();

	  	// calculating magnetic heading (much stabler than simple yaw calculation)
	  	// from https://avionicsduino.com/index.php/en/digital-compass/
	  	Xh = calibrated_lsm_data[6] * cos(orientation_data[3] / 180 * PI) - calibrated_lsm_data[8] * sin(orientation_data[3] / 180 * PI);
	  	Yh = -calibrated_lsm_data[6] * sin(orientation_data[2] / 180 * PI) * sin(orientation_data[3]/ 180 * PI) - calibrated_lsm_data[7] * cos(orientation_data[2] / 180 * PI) + calibrated_lsm_data[6] * sin(orientation_data[2] / 180 * PI) * cos(orientation_data[3] / 180 * PI);
	  	orientation_data[0] = fmod(180 - atan2(Yh, Xh) * 180 / PI + 180, 360); //Flip coordinate system (+180) so that the motors push forward
	  	if (orientation_data[0] < 0)
	    	orientation_data[0] += 360;		
	}
}
