#include <general_definitions.h>
#include <lsm9ds1_functions.h>
#include <SensorFusion.h>

SF fusion;

float pitch, roll, yaw, magheading, Xh, Yh;
float deltat;


void setup()
{
	// Set I2C pins
	Wire.setSDA(I2C_SDA);
	Wire.setSCL(I2C_SCL);
	Wire.setClock(400000);
	Wire.begin();

	// Start Serial and wait until it opens
	Serial.begin(115200);
	while (!Serial)
		delay(200);

	setup_lsm9ds1(lsm);
}


void loop()
{
  deltat = fusion.deltatUpdate();
  get_calibrated_lsm9ds1_data(lsm, calibrated_lsm_data, bias_corrected_magnetic);
  // invert y axis to get right-handed coordinate system.
  fusion.MadgwickUpdate(calibrated_lsm_data[3], -calibrated_lsm_data[4], calibrated_lsm_data[5], calibrated_lsm_data[0] ,-calibrated_lsm_data[1], calibrated_lsm_data[2], calibrated_lsm_data[6], -calibrated_lsm_data[7], calibrated_lsm_data[8], deltat);

  pitch = fusion.getPitch();
  roll = fusion.getRoll();
  yaw = fusion.getYaw();

  // calculating magnetic heading (much stabler than yaw calculation)
  // from https://avionicsduino.com/index.php/en/digital-compass/
  Xh = calibrated_lsm_data[6] * cos(pitch / 180 * PI) - calibrated_lsm_data[8] * sin(pitch / 180 * PI);
  Yh = -calibrated_lsm_data[6] * sin(roll / 180 * PI) * sin(pitch / 180 * PI) - calibrated_lsm_data[7] * cos(roll / 180 * PI) + calibrated_lsm_data[6] * sin(roll / 180 * PI) * cos(pitch / 180 * PI);
  magheading = 180 - atan2(Yh, Xh) * 180 / PI;
  if (magheading < 0)
    magheading += 360;


  Serial.print("Pitch: ");
  Serial.print(pitch);
  Serial.print("\tRoll: ");
  Serial.print(roll);
  Serial.print("\tYaw: ");
  Serial.print(yaw);
  Serial.print("\tMagheading: ");
  Serial.print(magheading); 
  Serial.print("\r"); // Carriage return to overwrite the same line
}