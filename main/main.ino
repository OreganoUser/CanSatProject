#include "general_definitions.h"
#include "bme_functions.h"
#include "gps_functions.h"
#include "lsm9ds1_functions.h"
#include "sd_functions.h"
#include "rf69_functions.h"
#include "rf95_functions.h"
#include "orientation.h"
#include "direction_control.h"
#include <Servo.h>
#include "flight_stages.h"

Servo servo;

// Create one structure that contains all sensor variable arrays
// Arrays for sensor variables are already defined in the respective function files
int number_of_arrays = 3;
float* data_arrays[] = {bme_data, gps_data, calibrated_lsm_data};
size_t data_array_sizes[] = {4, 6, 9};
int precisions[] = {1, 5, 4}; // This array holds the digits precision needed for each sensor
// Create buffer for data string
char data_string[MESSAGE_BUFFER_SIZE] = {0};


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

  //Servo
  servo.attach(SERVO_PIN);

  delay(1000);
}

int iteration_counter = 0;

void loop() {
  // Execute sensor data collection and writing functions
  get_bme_data(bme_data);
  get_gps_data(gps_data);
  get_calibrated_lsm9ds1_data(lsm, calibrated_lsm_data, bias_corrected_magnetic);
  create_data_string(data_string, iteration_counter, data_arrays, data_array_sizes, number_of_arrays, precisions);
  // need to do create_data_string before write_data_to_file!!!
  write_data_to_file(logfile_name, data_string);
  send_data_rf95(rf95, data_string);
  iteration_counter++;

  // Estimate heading using LSM9DS+
  // the number in update_mag_heading should be about 100 (200 should also work)
  // less than that gives bad accuracy
  // much more (for example 1000) slows execution down by quite a bit
  update_mag_heading(100);
  Serial.println(magheading);

  // calculate flight stage
  calc_flight_stage();

  if (flight_stage == 3)
  {
    // detected drop, deploy arms
    servo.write(180);
    arms_deployed = true;
  }

  if (flight_stage == 4)
  {
    // close to ground, close arms again
    servo.write(0);
    arms_deployed = false;
  }

  if (flight_stage >= 3)
  {
    if (arms_deployed) // make sure arms are deployed!
    {
      // adjust direction
      adjustDirection();     
    }
  }

}