#include "flight_stages.h"
#include "lsm9ds1_functions.h"
#include "gps_functions.h"
#include "general_definitions.h"

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


 void calc_flight_stage(){
// launch detection: automatically activated when the upwards acceleration significantly exceeds 9.81

	momentary_acceleration = sqrt(pow(calibrated_lsm_data[0], 2) + pow(calibrated_lsm_data[1], 2) + pow(calibrated_lsm_data[2], 2));

	if(gps_data[3] < THRESHOLD && flight_stage < 3){  //below threshold hight: stage 0, 1, 4, 5
		if( momentary_acceleration > LAUNCH_ACCELERATION && flight_stage == 0){
			flight_stage = 1;
			Serial.println("Flight stage = 1");
		} else if(flight_stage <= 0){
			flight_stage = 0;
			Serial.println("Flight stage = 0");
		} /*else {
			flight_stage = 4;
			Serial.println("Flight stage = 4");
		}*/
		
	} else { //over threshold height: stage 2, 3
		if(gps_data[3] < previousAltitude){
			flight_stage = 3;
			Serial.println("Flight stage = 3");
		} else {
			flight_stage = 2;
			Serial.println("Flight stage = 2");
			previousAltitude = gps_data[3];
		}

	}

} 



/*void manual_flight_stage_override() {
    // Prüfe, ob Daten über den Serial Monitor eingetroffen sind
    if (Serial.available() > 0) {
        // Lese das erste Zeichen
        char inputChar = Serial.read();

        // Wenn es eine Ziffer zwischen '0' und '5' ist, setze den entsprechenden Flugstatus
        if (inputChar >= '0' && inputChar <= '5') {
            flight_stage = inputChar - '0';
            flight_stage_data[0] = (float)flight_stage;
            Serial.print("Manuelle Flight-Stage-Uebersteuerung: ");
            Serial.println(flight_stage);
        }
    }
} 








/*void calc_flight_stage()
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
} */