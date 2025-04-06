#ifndef RF69_FUNCTIONS_H
#define RF69_FUNCTIONS_H

#include <RH_RF69.h>
#include "general_definitions.h"
extern uint8_t encryption_key[];
extern RH_RF69 rf69;
bool setup_rf69(RH_RF69& rf69, uint8_t encryptkey[]);
void create_data_string(char* data_string, int number_of_arrays, float* data_arrays[], size_t* data_array_sizes, int precision[]);
void send_data_rf69(RH_RF69& rf69, int iteration, int number_of_arrays, float* data_arrays[], size_t* data_array_sizes, int precision[]);
void send_data_rf69(RH_RF69& rf69, int iteration);

#endif //RF69_FUNCTIONS_H
