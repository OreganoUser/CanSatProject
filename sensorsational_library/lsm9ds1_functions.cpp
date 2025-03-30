#include "lsm9ds1_functions.h"
#include "calibration_parameters.h"
#include "general_definitions.h"

Adafruit_LSM9DS1 lsm(&Wire);

//LSM9DS1 variables
//ax, ay, az, gx, gy, gz, mx, my, mz
float lsm_data[9] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
int lsm_precisions[9] = {4, 4, 4, 4, 4, 4, 4, 4, 4};
float bias_corrected_magnetic[3] = {0.0, 0.0, 0.0};
float calibrated_lsm_data[9] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

sensors_event_t a, m, g, temp;

bool setup_lsm9ds1(Adafruit_LSM9DS1& lsm_object)
{
  bool lsm9ds1_setup_status;
  lsm9ds1_setup_status = lsm_object.begin();
  if (!lsm9ds1_setup_status)
    Serial.println("LSM9DS1 init failed!");
  else
    Serial.println("LSM9DS1 init success.");

  lsm_object.setupAccel(lsm_object.LSM9DS1_ACCELRANGE_2G, lsm_object.LSM9DS1_ACCELDATARATE_50HZ);
  lsm_object.setupMag(lsm_object.LSM9DS1_MAGGAIN_4GAUSS);
  lsm_object.setupGyro(lsm_object.LSM9DS1_GYROSCALE_245DPS);
  if (!lsm9ds1_setup_status)
  {
    return false;
  }
  else
    return true;
}

void get_lsm9ds1_data(Adafruit_LSM9DS1& lsm_object, float lsm9ds1_data[9])
{
  //function reads all data and saves to lsm9ds1_data
  lsm_object.read();
  lsm_object.getEvent(&a, &m, &g, &temp);
  lsm9ds1_data[0] = a.acceleration.x;
  lsm9ds1_data[1] = a.acceleration.y;
  lsm9ds1_data[2] = a.acceleration.z;
  lsm9ds1_data[3] = g.gyro.x;
  lsm9ds1_data[4] = g.gyro.y;
  lsm9ds1_data[5] = g.gyro.z;
  lsm9ds1_data[6] = m.magnetic.x;
  lsm9ds1_data[7] = m.magnetic.y;
  lsm9ds1_data[8] = m.magnetic.z;
}

void get_calibrated_lsm9ds1_data(Adafruit_LSM9DS1& lsm_object, float calibrated_data[9], float bias_corrected[3])
{
  // get raw data
  lsm_object.read();
  lsm_object.getEvent(&a, &m, &g, &temp);
  #ifdef SIMPLE_CAL
    // adjust raw with offset and slope
    calibrated_data[0] = accSlopes[0] * (a.acceleration.x - accOffsets[0]);
    calibrated_data[1] = accSlopes[1] * (a.acceleration.y - accOffsets[1]);
    calibrated_data[2] = accSlopes[2] * (a.acceleration.z - accOffsets[2]);
    calibrated_data[3] = gyroSlopes[0] * (g.gyro.x - gyroOffsets[0]);
    calibrated_data[4] = gyroSlopes[1] * (g.gyro.y - gyroOffsets[1]);
    calibrated_data[5] = gyroSlopes[2] * (g.gyro.z - gyroOffsets[2]);
    calibrated_data[6] = magSlopes[0] * (m.magnetic.x - magOffsets[0]);
    calibrated_data[7] = magSlopes[1] * (m.magnetic.y - magOffsets[1]);
    calibrated_data[8] = magSlopes[2] * (m.magnetic.z - magOffsets[2]);
  #endif


  #ifdef FULL_CAL
    // adjust raw with offset and slope
    calibrated_data[0] = accSlopes[0] * (a.acceleration.x - accOffsets[0]);
    calibrated_data[1] = accSlopes[1] * (a.acceleration.y - accOffsets[1]);
    calibrated_data[2] = accSlopes[2] * (a.acceleration.z - accOffsets[2]);
    calibrated_data[3] = gyroSlopes[0] * (g.gyro.x - gyroOffsets[0]);
    calibrated_data[4] = gyroSlopes[1] * (g.gyro.y - gyroOffsets[1]);
    calibrated_data[5] = gyroSlopes[2] * (g.gyro.z - gyroOffsets[2]);
    // remove magnetic bias
    bias_corrected[0] = (m.magnetic.x - magBias[0]);
    bias_corrected[1] = (m.magnetic.y - magBias[1]);
    bias_corrected[2] = (m.magnetic.z - magBias[2]);
    //remove soft iron erros (matrix multiplication)
    calibrated_data[6] = softIron[0] * bias_corrected[0] + softIron[1] * bias_corrected[1] + softIron[2] * bias_corrected[2];
    calibrated_data[7] = softIron[3] * bias_corrected[0] + softIron[4] * bias_corrected[1] + softIron[5] * bias_corrected[2];
    calibrated_data[8] = softIron[6] * bias_corrected[0] + softIron[7] * bias_corrected[1] + softIron[8] * bias_corrected[2];
  #endif //SIMPLE_CAL
}