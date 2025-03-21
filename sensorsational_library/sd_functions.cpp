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


bool write_data_to_file(const char* filename, int array_number, float* sensor_arrays[], size_t array_sizes[], int precision[]) 
{
  // this function writes all data from all sensors to SD card as comma-separated values
  File file = SD.open(filename, FILE_WRITE);
  if (!file) {
    return false;
  }

  // Loop through each sensor array
  for (int i = 0; i < array_number; i++) {
    float* array = sensor_arrays[i];
    size_t size = array_sizes[i];

    for (size_t j = 0; j < size; j++) {
      file.print(array[j], precision[i]);  // Use the corresponding precision for each sensor
      if (i < array_number - 1 || j < size - 1) {
        file.print(", ");
      }
    }
  }

  file.println();
  file.close();
  return true;
}