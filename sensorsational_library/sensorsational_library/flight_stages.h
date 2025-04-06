#ifndef FLIGHT_STAGES_H
#define FLIGHT_STAGES_H

//extern float max_altitude;
extern float previousAltitude;
extern int flight_stage;
extern float flight_stage_data[1];
extern int flight_stage_precisions[1];
void calc_flight_stage();

#endif //FLIGHT_STAGES_H