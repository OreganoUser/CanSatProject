#include "rf95_functions.h"
#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>

RH_RF95 rf95(RF95_CS, RF95_G0);



bool setup_rf95(RH_RF95& rf95_object)
{
  pinMode(RF95_RST, OUTPUT);
  digitalWrite(RF95_RST, HIGH);
  delay(100);
  // Manual reset
  digitalWrite(RF95_RST, LOW);
  delay(10);
  digitalWrite(RF95_RST, HIGH);
  delay(10);

  bool rf95_init_status;
  rf95_init_status = rf95_object.init();
  if (!rf95_init_status)
    Serial.println("RF95 init failed!");
  else
    Serial.println("RF95 init success.");
  if (!rf95_object.setFrequency(RF95_FREQ))
      Serial.println("RF95 setFrequency failed!");

  rf95_object.setSpreadingFactor(8);  // Adjust if needed
  rf95_object.setSignalBandwidth(125E3);  // Match sender
  rf95_object.setCodingRate4(5);
  // set sender and receiver addresses to make sure we're the only ones to get the messages
  rf95_object.setThisAddress(CANSAT_ADDRESS);
  rf95_object.setHeaderFrom(CANSAT_ADDRESS);
  rf95_object.setHeaderTo(GND_STATION_ADDRESS);

  // power from 5 to 23 dB. Always set false!
  rf95_object.setTxPower(23, false);
  if (rf95_init_status)
    return true;
  else
    return false;
}



void create_data_string(char* output_buffer, int package_number, float* data_arrays[], 
                       size_t* data_array_sizes, int number_of_arrays, int* precision[]) {
    char* ptr = output_buffer;
    int remaining_space = MESSAGE_BUFFER_SIZE - 1; // Reserve 1 byte for null terminator
    
    // Write package number
    int written = snprintf(ptr, remaining_space, "%d,", package_number);
    if (written < 0 || written >= remaining_space) {
        *output_buffer = '\0';
        return;
    }
    ptr += written;
    remaining_space -= written;
    
    // Write all data points
    for (int i = 0; i < number_of_arrays && remaining_space > 0; i++) {
        for (size_t j = 0; j < data_array_sizes[i] && remaining_space > 0; j++) {
            // Check if precision is 0 (omit decimal point)
            if (precision[i][j] == 0) {
                written = snprintf(ptr, remaining_space, "%.0f,", data_arrays[i][j]);
            } else {
                written = snprintf(ptr, remaining_space, "%.*f,", precision[i][j], data_arrays[i][j]);
            }
            
            if (written < 0 || written >= remaining_space) {
                break;
            }
            ptr += written;
            remaining_space -= written;
        }
    }
    
    // Replace the last comma with null terminator
    if (ptr > output_buffer) {
        *(ptr - 1) = '\0';
    } else {
        *ptr = '\0';
    }
}


void send_data_rf95(RH_RF95& rf95_object, char* buffer) {
    // this function takes the preformed message (buffer) and sends it
    // Calculate the length of the actual message (up to null terminator)
    uint8_t message_len = strlen(buffer);
    
    // Send only the relevant portion of the buffer
    rf95_object.send((const uint8_t*)buffer, message_len);
    
    // Optional debug output
    Serial.print("Sent ");
    Serial.println(buffer);
}