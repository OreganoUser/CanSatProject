#include <general_definitions.h>
#include <bme_functions.h>
#include <gps_functions.h>
#include <lsm9ds1_functions.h>
#include <sd_functions.h>
#include <rf69_functions.h>
#include <rf95_functions.h>
#include <calibration_parameters.h>
#include <calibration_functions.h>
#include "hardware/uart.h"


//create one structure that contains all sensor variable arrays
//arrays for sensor variables are already defined in the respective function files
int number_of_arrays = 3;
float* data_arrays[] = {bme_data, gps_data, lsm_data};
size_t data_array_sizes[] = {4, 6, 9};
int precisions[] = {1, 5, 4}; // this array holds the digits precision that is needed for each sensor

//event variables are defined in the lsm9ds1_functions file sensors_event_t accel, gyro, mag, temp;

//Max, Min Values for acc and mag sensor.
// Index 0 is Max, Index 1 is Min
// these are local variables because only needed for calibration
float accX[2] = {0.0, 0.0};
float accY[2] = {0.0, 0.0};
float accZ[2] = {0.0, 0.0};
float magX[2] = {0.0, 0.0};
float magY[2] = {0.0, 0.0};
float magZ[2] = {0.0, 0.0};

void setup()
{
    // Calibration parameters are reset because we are performing calibration
    for (int i = 0; i < 3; i++) 
        accOffsets[i] = 0.0f;  // Set all elements to 0

    for (int i = 0; i < 3; i++) 
        accSlopes[i] = 1.0f;  // Set all elements to 1

    for (int i = 0; i < 3; i++) 
        gyroOffsets[i] = 0.0f;  // Set all elements to 0

    for (int i = 0; i < 3; i++) 
        gyroSlopes[i] = 1.0f;  // Set all elements to 1

    for (int i = 0; i < 3; i++) 
        magOffsets[i] = 0.0f;  // Set all elements to 0

    for (int i = 0; i < 3; i++) 
        magSlopes[i] = 1.0f;  // Set all elements to 1
    
    // Set I2C pins
	Wire.setSDA(I2C_SDA);
	Wire.setSCL(I2C_SCL);
	Wire.begin();

	// Set SPI PINS
	SPI.setRX(SPI_RX);
	SPI.setTX(SPI_TX);
	SPI.setSCK(SPI_SCK);

	// Start Serial and wait until it opens
	Serial.begin(115200);
	while (!Serial)
		delay(200);

    // only lsm9ds1 needs to be setup, so bme, gps, sd and rf95 can be commented out
	setup_bme(bme);

	setup_gps(gps);

	setup_lsm9ds1(lsm);

	setup_sd_card();
	
	//for the rf95 module to work:
	//need to call setup_sd_card() even if no sd card is connected!
	//else: spi has problems and it won't work
	setup_rf95(rf95);

	// Prompt user for calibration choice
    Serial.println("Starting Calibration");
    Serial.println("ATTENTION: Determined calibration parameters must be manually written to calibration_parameters.cpp if they are to be used in the future!");
    Serial.println("Choose calibration type:");
    Serial.println("1. Accelerometer Calibration");
    Serial.println("2. Gyroscope Calibration");
    Serial.println("3. Magnetometer Calibration");
    Serial.println("4. Full Calibration");
    Serial.println("Enter your choice (1, 2, 3, or 4):");

	// Wait for user input
    while (Serial.available() == 0) {}
    int choice = Serial.parseInt();

    // Execute the chosen calibration
    switch (choice) {
        case 1:
            calibrateAccelerometer(lsm, accX, accY, accZ);
            break;
        case 2:
            calibrateGyroscope(lsm);
            break;
        case 3:
            calibrateMagnetometer(lsm, magX, magY, magZ);
            break;
        case 4:
            calibrateAccelerometer(lsm, accX, accY, accZ);
            calibrateGyroscope(lsm);
            calibrateMagnetometer(lsm, magX, magY, magZ);
            break;
        default:
            Serial.println("Invalid choice. No calibration performed.");
            break;
    }
}


void loop()
{
}