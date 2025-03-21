#include "general_definitions.h"
#include "sd_functions.h"
#include "lsm9ds1_functions.h"

// Define the duration for which the loop should run (30 seconds)
const unsigned long RUN_DURATION = 50000; // 50 seconds in milliseconds

// Variable to store the start time
unsigned long startTime;

void setup()
{
    // Set I2C pins
    Wire.setSDA(I2C_SDA);
    Wire.setSCL(I2C_SCL);
    Wire.setClock(400000);
    Wire.begin();

    // Set SPI PINS
    SPI.setRX(SPI_RX);
    SPI.setTX(SPI_TX);
    SPI.setSCK(SPI_SCK);

    // Start Serial (no waiting)
    Serial.begin(115200);
    while (!Serial)
        delay(200);

    // Initialize the LSM9DS1 sensor
    setup_lsm9ds1(lsm);

    // Initialize the SD card
    setup_sd_card();

    // Record the start time
    startTime = millis();
}

void loop()
{
    // Check if time has passed
    if (millis() - startTime < RUN_DURATION) {
        // Get data from the LSM9DS1 sensor
        get_lsm9ds1_data(lsm, lsm_data);

        // Write the data to the SD card
        writeFloatArrayToSD(lsm_data[6], lsm_data[7], lsm_data[8], "mag_data.txt");

        // Delay for 100 milliseconds
        delay(100);
    } else {
        // Stop doing anything after 30 seconds
        while (true) {
            // Infinite loop to stop further execution
        }
    }
}

void writeFloatArrayToSD(float value1, float value2, float value3, const char* filename) {
    File file = SD.open(filename, FILE_WRITE);
    if (file) {
        file.print(String(value1, 6));
        file.print("\t");
        file.print(String(value2, 6));
        file.print("\t");
        file.println(String(value3, 6)); // Move to the next line
        file.close();
    } else {
        Serial.println("Error opening file!");
    }
}