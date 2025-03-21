#include "rf69_functions.h"

RH_RF69 rf69(RF69_CS, RF69_G0);
uint8_t encryption_key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};


bool setup_rf69(RH_RF69& rf69_object, uint8_t encryptkey[])
{
  bool rf69_init_status;
  rf69_init_status = rf69_object.init();
  if (!rf69_init_status)
    Serial.println("RFM69 init failed!");
  else
    Serial.println("RFM69 init success.");
  if (!rf69_object.setFrequency(RF69_FREQ))
      Serial.println("RFM69 setFrequency failed!");
  rf69_object.setTxPower(20, true);
  rf69_object.setEncryptionKey(encryptkey); 
  if (rf69_init_status)
    return true;
  else
    return false;
}



void create_data_string(char* data_string, int number_of_arrays, float* data_arrays[], size_t* data_array_sizes, int precision[]) {
  // Helper function to concatenate data from all sensors into one long string (using char array)

  int idx = 0;

  // Loop through each array and append the data
  for (int i = 0; i < number_of_arrays; i++) {
    for (size_t j = 0; j < data_array_sizes[i]; j++) {
      // Ensure we don't exceed the max length
      if (idx >= MAX_CONCATENATED_STRING_LENGTH) {
        data_string[idx] = '\0';  // Null-terminate string
        return;
      }

      // Use the specified precision for the current sensor array
      int len = snprintf(data_string + idx, MAX_CONCATENATED_STRING_LENGTH - idx, "%.*f", precision[i], data_arrays[i][j]);
      idx += len;

      // Add comma unless it's the last value
      if (i < number_of_arrays - 1 || j < data_array_sizes[i] - 1) {
        if (idx < MAX_CONCATENATED_STRING_LENGTH - 1) {
          data_string[idx++] = ',';
        } else {
          data_string[idx] = '\0';  // Ensure null-termination if out of space
          return;
        }
      }
    }
  }
  data_string[idx] = '\0';  // Null-terminate the string
}


void send_data_part(RH_RF69& rf69, int iteration, int chunk_index, int total_chunks, char* message) {
  // This function sends one data chunk (piece of the whole string) that is small enough
  // to fit into one RF69 message.
  // Every chunk gets a packet number, chunk index, and total chunks as a header.
  // This helps the receiver know which packet this chunk is part of and how many chunks to expect.

  char header[HEADER_SIZE_RF69];  // Space for header, adjust size if necessary
  // The header contains packet number (iteration), the number of the current chunk, and the total number of chunks to expect
  int len = snprintf(header, sizeof(header), "%d,%d,%d,", iteration, chunk_index, total_chunks);

  char full_message[MAX_MESSAGE_SIZE_RF69];
  memcpy(full_message, header, len);  // Copy the header to the full message
  int remaining_space = MAX_MESSAGE_SIZE_RF69 - len;

  // Copy the chunk data into the remaining space in full_message
  strncpy(full_message + len, message, remaining_space - 1);
  full_message[MAX_MESSAGE_SIZE_RF69 - 1] = '\0';  // Ensure null-termination

  // Send the message via RF69 module
  rf69.send((uint8_t*)full_message, strlen(full_message));
  rf69.waitPacketSent();  // Wait until the message is sent
}

// Function to split and send the data
void send_data_rf69(RH_RF69& rf69, int iteration, int number_of_arrays, float* data_arrays[], size_t* data_array_sizes, int precision[]) {
  // This function takes all sensor data, creates one string from it,
  // splits it into chunks small enough to be sent, and sends each chunk sequentially while adding a header.

  // Create one big data string with all data
  char data_string[MAX_CONCATENATED_STRING_LENGTH];  // To store the concatenated data string
  create_data_string(data_string, number_of_arrays, data_arrays, data_array_sizes, precision);

  // Calculate into how many chunks this total data string needs to be split
  int total_chunks = (strlen(data_string) + MAX_DATA_SIZE_RF69 - 1) / MAX_DATA_SIZE_RF69;
  int start_index = 0;

  // Send each chunk with a header
  for (int chunk_index = 0; chunk_index < total_chunks; chunk_index++) {
    int chunk_size = min(MAX_DATA_SIZE_RF69, strlen(data_string) - start_index);
    char chunk[MAX_DATA_SIZE_RF69];
    strncpy(chunk, data_string + start_index, chunk_size);  // Get the chunk of the data string
    chunk[chunk_size] = '\0';  // Null-terminate the chunk

    send_data_part(rf69, iteration, chunk_index + 1, total_chunks, chunk);  // Send the chunk with iteration and chunk index
    start_index += chunk_size;  // Move the start index for the next chunk
  }
}
