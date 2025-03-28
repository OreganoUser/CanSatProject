#ifndef RF95_FUNCTIONS_H
#define RF95_FUNCTIONS_H
#include <RH_RF95.h>
#include "general_definitions.h"

extern RH_RF95 rf95;

bool setup_rf95(RH_RF95& rf95_object);
void send_data_rf95(RH_RF95& rf95_object, char* buffer);
void create_data_string(char* output_buffer, int package_number, float* data_arrays[], size_t* data_array_sizes, int number_of_arrays, int precision[]);

#endif //RF95_FUNCTIONS_H