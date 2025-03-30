#ifndef FLIGHT_STAGES_H
#define FLIGHT_STAGES_H

extern float max_altitude;
extern bool arms_deployed;
extern int flight_stage;
extern float flight_stage_data[];
extern int flight_stage_precisions[];
void calc_flight_stage();

#endif //FLIGHT_STAGES_H