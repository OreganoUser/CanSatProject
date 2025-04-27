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


int flight_stage = -1;
float max_altitude = 0;
float min_altitude = 10000; // needs to be initialised as large value
//float previousAltitude = 0; //used for drop detection
//float momentary_acceleration = 0;

static unsigned long fall_start_time = 0;
static bool is_falling = false;
// arms_deployed is true if arms deployed
bool arms_deployed = false;
// the float array holds the flight stage integer
// having it as float is easier to write to SD card or send via LoRa
float flight_stage_data[1] = {-1.0};
int flight_stage_precisions[1] = {0};

/*
void calc_flight_stage(){
	//

	momentary_acceleration = sqrt(pow(calibrated_lsm_data[0], 2) + pow(calibrated_lsm_data[1], 2) + pow(calibrated_lsm_data[2], 2));

	unsigned long current_time = millis();


	if(!is_falling){
		if(bme_data[2] > THRESHOLD){ //flight_stage 2 & 3
			if(momentary_acceleration < FALL_ACCELERATION){
				is_falling = true;

			} else {
				flight_stage = 2;
			}

		} else if (momentary_acceleration > LAUNCH_ACCELERATION && flight_stage == 0){ //flight stage 1 (d'Flight stage 1 dierf net gesaat ginn wann grad 2/3 schon amgangen sinn)
			flight_stage = 1;
		} else {
			flight_stage = 0;
		}

	} else if (bme_data[2] < THRESHOLD && momentary_acceleration > IMPACT_ACCELERATION) {// Wann is_falling = true
			flight_stage = 4;
	} else if(is_falling && momentary_acceleration == LAUNCH_ACCELERATION){
		fall_start_time = current_time;
		if(current_time - fall_start_time > FALL_DELAY){
			flight_stage = 3;
		}
	}
}
*/






void calc_flight_stage() {

	if (flight_stage == -1)
	{
		flight_stage = 0;
	}

	max_altitude = fmax(bme_data[2], max_altitude);
	min_altitude = fmin(bme_data[2], min_altitude);

	if ((bme_data[2] > min_altitude + LAUNCH_THRESHOLD) && (flight_stage == 0))
	{
		flight_stage = 1;
	}

	if (bme_data[2] < max_altitude - DROP_THRESHOLD)
	{
		flight_stage = 3;
	}

	if ((bme_data[2] < min_altitude + LAUNCH_THRESHOLD) && (flight_stage = 3))
	{
		flight_stage = 4;
	}
	flight_stage_data[0] = flight_stage;
}