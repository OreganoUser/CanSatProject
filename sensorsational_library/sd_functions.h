#ifndef SD_FUNCTIONS_H
#define SD_FUNCTIONS_H

#include <SD.h>
#include "general_definitions.h"
extern File logfile_object;
extern const char logfile_name[];


bool setup_sd_card();
bool write_data_to_file(const char* filename, const char* data_string) ;
#endif //SD_FUNCTIONS_H