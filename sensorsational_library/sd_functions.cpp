#include "sd_functions.h"

File logfile_object;
const char logfile_name[] = "data.txt";

bool setup_sd_card()
{
  if (SD.begin(SD_CS))
  {
    Serial.println("SD card init success.");
    return true;
  }
  else 
  {
    Serial.println("SD card init failed!");
    return false;
  }
}


bool write_data_to_file(const char* filename, const char* data_string) 
{
  // the data_string is already formed as a global variable in main
  // so we just need to write this string to file
  File file = SD.open(filename, FILE_WRITE);
  if (!file) return false;
  file.println(data_string);
  file.close();
  return true;
}