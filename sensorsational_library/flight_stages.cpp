#include "flight_stages.h"
#include "lsm9ds1_functions.h"
#include "gps_functions.h"
#include "general_definitions.h"
#include "bme_functions.h"

//these functions are intended to evaluate the flight stages of the cansat.
// 0 is pre-launch
// 1 is launch detection
// 2 is above threshhold height, before drop
// 3 is after drop
// 4 is just before landing (badly defined right now)
// 5 is after landing (not defined right now)


int flight_stage = 0;
float max_altitude = 0;
float min_altitude = 10000; // needs to be initialised as large value

static unsigned long fall_start_time = 0;
static bool is_falling = false;
// arms_deployed is true if arms deployed
bool arms_deployed = false;
// the float array holds the flight stage integer
// having it as float is easier to write to SD card or send via LoRa
float flight_stage_data[1] = {0.0};
int flight_stage_precisions[1] = {0};






void calc_flight_stage() {

	max_altitude = fmax(bme_data[2], max_altitude);
	min_altitude = fmin(bme_data[2], min_altitude);

	if ((bme_data[2] < (min_altitude + LAUNCH_THRESHOLD)) && (flight_stage == 3))
	{
		flight_stage = 4;
	}

	if ((bme_data[2] < (max_altitude - DROP_THRESHOLD)) && (flight_stage == 2))
	{
		flight_stage = 3;
	}

	if ((bme_data[2] > (min_altitude + ALTITUDE_THRESHOLD)) && (flight_stage == 1))
	{
		flight_stage = 2;
	}

	if ((bme_data[2] > (min_altitude + LAUNCH_THRESHOLD) && (flight_stage == 0))
	{
		flight_stage = 1;
	}
	
	flight_stage_data[0] = flight_stage;
}