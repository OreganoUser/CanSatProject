//I2C pins (BME280, GPS, LSM9DS1)
//Attention to I2C Bus! If using I2C0 bus, we need to use Wire
//if using I2C1 bus (check pico pinout), we need to use Wire1 and change accordingly in setup functions
#define I2C_SDA  16
#define I2C_SCL  17

//Shared SPI pins (radio and SD)
#define SPI_RX 4
#define SPI_TX 7
#define SPI_SCK 2

// RF95
#define RF95_CS 8
#define RF95_RST 9
#define RF95_G0 0
#define RF95_FREQ 433.4
#define MESSAGE_BUFFER_SIZE 275
// Define the CanSat's and the ground station's address
#define CANSAT_ADDRESS 42
#define GND_STATION_ADDRESS 211


// RF69
//can use same CS, RST and G0 pins as RF95 since they are not used together!
#define RF69_CS 8
#define RF69_RST 9
#define RF69_G0 0
#define RF69_FREQ 433.4
#define MAX_MESSAGE_SIZE_RF69 61 //max total message length
#define HEADER_SIZE_RF69 10 //space reserved for the header (iteration_counter, chunk number)
#define MAX_DATA_SIZE_RF69 (MAX_MESSAGE_SIZE_RF69 - HEADER_SIZE_RF69)  // remaining space for actual data
#define MAX_CONCATENATED_STRING_LENGTH 255 // length of string for all sensor data. 255 bytes should be enough but can be changed

// SD card
#define SD_CS 10

#define SEA_LEVEL_PRESSURE 1017 // needs to be changed the day of launch so that bme calculates correct altitude

// for calibration
#define gravity 9.809  // Gravity constant
#define mag_intensity 48.765 // intensity of magnetic field (in microtesla) at your location have to research this! https://www.ngdc.noaa.gov/geomag/calculators/magcalc.shtml#igrfwmm

// define which calibration to use when getting lsm data.
// SIMPLE_CAL is simple offset slope model
// FULL_CAL is Hard and soft iron model
//#define SIMPLE_CAL
#define FULL_CAL