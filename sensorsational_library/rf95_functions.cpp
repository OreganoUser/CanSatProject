#include "rf95_functions.h"

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

void send_data_rf95(RH_RF95& rf95_object, int package_number, float* data_arrays[], size_t* data_array_sizes, int number_of_arrays, int precision[]) 
{
    // Initialize a buffer to hold the message
    char message[MAX_MESSAGE_SIZE_RF95];
    int messageIndex = 0;

    // Add the package number to the message
    messageIndex += snprintf(message + messageIndex, MAX_MESSAGE_SIZE_RF95 - messageIndex, "%d", package_number);

    // Add a comma after the package number
    message[messageIndex++] = ',';

    // Loop through all the arrays and append their values to the message
    for (int i = 0; i < number_of_arrays; i++) {
        for (size_t j = 0; j < data_array_sizes[i]; j++) {
            // Use the precision value for this sensor array
            int current_precision = precision[i];

            // Append the float value to the message with the specified precision
            messageIndex += snprintf(message + messageIndex, MAX_MESSAGE_SIZE_RF95 - messageIndex, "%.*f", current_precision, data_arrays[i][j]);

            // If it's not the last value in the array, add a comma
            if (j < data_array_sizes[i] - 1) {
                message[messageIndex++] = ',';
            }
        }

        // After each array, add a comma (to separate arrays)
        if (i < number_of_arrays - 1) {
            message[messageIndex++] = ',';
        }
    }

    // Send the entire message over RF95
    rf95_object.send((const uint8_t*)message, messageIndex);
    rf95_object.waitPacketSent();  // Wait until the packet is sent

    // Optional: Print the message to Serial Monitor for debugging
    #ifdef DEBUG_MODE
      message[messageIndex] = '\0';  // Null-terminate the string for debugging
      Serial.println("Data sent: " + String(message));  // Print the message
    #endif //DEBUG_MODE
}