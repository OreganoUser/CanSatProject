#include "flight_stages.h"
#include "bme_functions.h"
#include "general_definitions.h"

//these functions are intended to evaluate the flight stages of the cansat.
// 0 is pre-launch
// 1 is launch detection
// 2 is above threshhold height, before drop
// 3 is after drop
// 4 is just before landing (badly defined right now)
// 5 is after landing (not defined right now)


int flight_stage = -1;
float max_altitude = 0;
float min_altitude = 10000; // needs to be initialised as large value
// arms_deployed is true if arms deployed
bool arms_deployed = false;
// the float array holds the flight stage integer
// having it as float is easier to write to SD card or send via LoRa
float flight_stage_data[1] = {-1.0};
int flight_stage_precisions[1] = {0};

void calc_flight_stage()
{
	// bme_data[2] contains current altitude
	// first update max and min
	max_altitude = max(max_altitude, bme_data[2]);
	min_altitude = min(min_altitude, bme_data[2]);
	
	if (bme_data[2] <= THRESHOLD) // cases 0, 1, 4 and 5
	{
		if (bme_data[2] <= (min_altitude + 30)) // cases 0, 4, 5
		{
			if (max_altitude < THRESHOLD) // case 0
			{
				flight_stage =  0;
			}
			else // cases 4,5
			{
				// case 5 is not implemented right now
				flight_stage = 4;
			}
		}
		else // case 1
		{
			flight_stage = 1;
		}
	}
	else // cases 2, 3
	{ 
		if (bme_data[2] <= max_altitude - METERS_DELAY) // case 3
		{
			flight_stage = 3;
		}
		
		else // case 2
		{
			flight_stage = 2;
		}
	}
	flight_stage_data[0] = flight_stage;
}