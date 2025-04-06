#include "general_definitions.h"
#include "bme_functions.h"
#include "gps_functions.h"
#include "lsm9ds1_functions.h"
#include "sd_functions.h"
#include "rf69_functions.h"
#include "rf95_functions.h"
#include "orientation.h"
#include "direction_control.h"
#include "flight_stages.h"



// define array for saving timestamp
float time_array[1] = {0.0};
int time_precisions[1] = {1};

// Create one structure that contains all variable arrays
// Arrays for sensor variables are already defined in the respective function files
int number_of_arrays = 6;
float* data_arrays[] = {time_array, bme_data, gps_data, calibrated_lsm_data, orientation_data, flight_stage_data};
size_t data_array_sizes[] = {1, 4, 6, 9, 4, 1};
// This array holds the digits precision needed for each of the sensor variables
int* precisions[] = {time_precisions, bme_precisions, gps_precisions, lsm_precisions, orientation_precisions, flight_stage_precisions};

// Create buffer for data string
char data_string[MESSAGE_BUFFER_SIZE] = {0};

// counts how many times loop is executed
int iteration_counter = 0;


void setup() {
  // Set I2C pins
  Wire.setSDA(I2C_SDA);
  Wire.setSCL(I2C_SCL);
  Wire.setClock(400000);
  Wire.begin();

  // Set SPI PINS
  SPI.setRX(SPI_RX);
  SPI.setTX(SPI_TX);
  SPI.setSCK(SPI_SCK);

  // Start Serial and wait until it opens
  Serial.begin(115200);
  while (!Serial)
    delay(200);

  setup_bme(bme);
  setup_gps(gps);
  setup_lsm9ds1(lsm);
  setup_sd_card();
  
  // For the RF95 module to work:
  // Need to call setup_sd_card() even if no SD card is connected!
  // Else: SPI has problems and it won't work
  setup_rf95(rf95);

  initMotors(); //esc's

  //Servo
  //servo.attach(SERVO_PIN);
  delay(1000);
  // start time measurement (in seconds)
  time_array[0] = millis() / 1000.0;
}


void loop() {
  // Estimate heading using LSM9DS1
  // This should be done as a first step because direction control needs accurate values
  // and we need to send it back to ground station, so it needs to happen before sending!
  // the number in update_mag_heading should be about 100 (200 should also work)
  // less than that gives bad accuracy
  // much more (for example 1000) slows execution down by quite a bit
  update_mag_heading(100);
  // Execute sensor data collection and writing functions
  get_bme_data(bme_data);
  get_gps_data(gps_data);
  get_calibrated_lsm9ds1_data(lsm, calibrated_lsm_data, bias_corrected_magnetic);
  //update timestamp (in s)
  time_array[0] = millis() / 1000.0;
  create_data_string(data_string, iteration_counter, data_arrays, data_array_sizes, number_of_arrays, precisions);
  // need to do create_data_string before write_data_to_file!!!
  write_data_to_file(logfile_name, data_string);
  send_data_rf95(rf95, data_string);
  iteration_counter++;
  
  // calculate flight stage here and do actions (deploy arms, adjust direction etc) (not working well now)

  Serial.println(orientation_data[0]);
  adjustDirection();
}