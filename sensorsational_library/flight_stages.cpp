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
//float max_altitude = 0;
//float min_altitude = 10000; // needs to be initialised as large value
float previousAltitude = 0; //used for drop detection
float momentary_acceleration = 0;
// arms_deployed is true if arms deployed
bool arms_deployed = false;
// the float array holds the flight stage integer
// having it as float is easier to write to SD card or send via LoRa
float flight_stage_data[1] = {-1.0};
int flight_stage_precisions[1] = {0};


 /*void calc_flight_stage(){
// launch detection: automatically activated when the upwards acceleration significantly exceeds 9.81
	Serial.print("height:");
	Serial.println(bme_data[2]);

	momentary_acceleration = sqrt(pow(calibrated_lsm_data[0], 2) + pow(calibrated_lsm_data[1], 2) + pow(calibrated_lsm_data[2], 2));

	if(gps_data[3] < THRESHOLD && flight_stage < 3){  //below threshold hight: stage 0, 1, 4, 5
		if( momentary_acceleration > LAUNCH_ACCELERATION && flight_stage == 0){
			flight_stage = 1;
		} else if(flight_stage <= 0){
			flight_stage = 0;
		} else {
			flight_stage = 4;
			Serial.println("Flight stage = 4");
		}
		
	} else { //over threshold height: stage 2, 3
		if(gps_data[3] < previousAltitude){
			flight_stage = 3;
		} else {
			flight_stage = 2;
			previousAltitude = gps_data[3];
		}

	}

	flight_stage_data[0] = (float)flight_stage;
} */



/*void calc_flight_stage()
{
	// bme_data[2] contains current altitude
	// first update max and min
	max_altitude = max(max_altitude, bme_data[2]);
	min_altitude = min(min_altitude, bme_data[2]);

	Serial.print("height:");
	Serial.println(bme_data[2]);
	
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
} */



/*void calc_flight_stage() {

	//Nemmen mam BME
	//As basic as possible. Less is more
    static unsigned long fall_start_time = 0;
    static bool is_falling = false;

	previousAltitude;
	static unsigned long last_update_time = 0;    // verzögerung fir dass previousAltitude net emmer selwecht ass wei current_altitude
    const unsigned long UPDATE_INTERVAL = 2000UL;

    float current_altitude = bme_data[2];
    unsigned long current_time = millis(); // aktuell Zäit fir d'Fallverzögerung

	Serial.print("current_altitude:");
	Serial.println(current_altitude);

    if(current_altitude < (previousAltitude + 5) && current_altitude > (previousAltitude - 5)){ //kleng Toleranz
		flight_stage = 0; // An dessem fall ass flight_stage = 0 dat selwecht wei flight_stage = 4/5. Dei wichtegst Aufgab vun beiden ass, dass d'Motoren net dreinen an dofir kennen se och dei selwecht bleiwen
		is_falling = false;
		fall_start_time = 0;
		Serial.println("BBBBBBBBBBBBBBBBBBB");
	} else if (current_altitude > previousAltitude) {
		Serial.println("AAAAAAAAAAAAAAAAAAAAAA");
		if(bme_data[2] > THRESHOLD){
			flight_stage == 2;
		} else{
        flight_stage = 1; 
		}
        is_falling = false;
        fall_start_time = 0;
    } else if (current_altitude < previousAltitude) {
		Serial.println("FALLING!!");
        if (!is_falling) {
            is_falling = true;
            fall_start_time = current_time;
        } else if (current_time - fall_start_time >= 5000) { //Timedelay before falling
            flight_stage = 3;
        }
    }

    if (current_time - last_update_time >= UPDATE_INTERVAL) {
        previousAltitude     = current_altitude;
        last_update_time     = current_time;
	}

	Serial.print("previous_altitude:");
	Serial.println(previousAltitude);

	flight_stage_data[0] = flight_stage;
} */


void calc_flight_stage(){
	//

	momentary_acceleration = sqrt(pow(calibrated_lsm_data[0], 2) + pow(calibrated_lsm_data[1], 2) + pow(calibrated_lsm_data[2], 2));

	static unsigned long fall_start_time = 0;
    static bool is_falling = false;
	unsigned long current_time = millis();


	if(!is_falling){
		if(bme_data[2] > THRESHOLD){ //flight_stage 2 & 3
			if(momentary_acceleration < FALL_ACCELERATION){
				is_falling = true;
				fall_start_time = current_time;

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
	} else {
		if(current_time - fall_start_time > FALL_DELAY){
			flight_stage = 3;
		}
	}
}








/*void calc_flight_stage() {
	//maluell
    if (Serial.available() > 0) {
        char inputChar = Serial.read();

        if (inputChar >= '0' && inputChar <= '5') {
            flight_stage = inputChar - '0';
            flight_stage_data[0] = (float)flight_stage;
            Serial.print("Manual flight_stage ");
            Serial.println(flight_stage);
        }
    }
} */