# Explanations for the calibration procedure
This procedure is taken from https://github.com/FemmeVerbeek/Arduino_LSM9DS1/tree/master/examples.
An instructional video that shows the procedure can be found here: https://www.youtube.com/watch?v=BLvYFXoP33o

## Setting up the code
- The sensorsational_library must be in the arduino/libraries folder.
- The SensorFusion (https://github.com/aster94/SensorFusion/tree/master/examples) library must be installed.
- Change Line 33 in the file: Arduino/libraries/Sensorfusion/src/Sensorfusion.cpp to #define betaDef     2.0f        	// 2 * proportional gain

## Executing the accelerometer calibration
- Set the can on a horizontal surface (use a water level!) and leave it. It is important that it isn't moving during calibration.
- Execute calibration.ino and select accelerometer.
- Follow the instructions and do the calibration for the 3 axes (axes are labelled on the LSM9DS1).

## Executing the gyro calibration
- Set the can on a flat horizontal surface.
- Execute calibration.ino and select gyro.
- Follow the instructions. The rotation angle can be set in the code (the bigger, the better in theory)
- Repeat for all 3 axes.

## Executing the magnetic calibration
- The magnetic field strength must be looked up (https://www.ngdc.noaa.gov/geomag/calculators/magcalc.shtml#igrfwmm) and put into the general_definitions.h file.
- **Make sure you are away from all magnetic disturbance (power outlets, speakers, ...)!** This is best done outside and in the real environment (with all components installed).
- Follow the instructions on the screen and rotate the can around all axes.
- Stop when the max and min values don't change much anymore.
- Note: the magneto_calibration is preferred because it's more accurate.

## Setting the calibration parameters
- Write the determined parameters into the calibration_parameters.cpp file.
- For the simple magnetic calibration, use magOffsets and magSlopes. If the magneto calibration is done, use magBias and softIron.
