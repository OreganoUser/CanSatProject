# Code Structure
## sensorsational_library
This folder contains all the functions. This folder **should be placed in the arduino libraries folder** to make it accessible to all other sketches.The functions are split up in different files depending on the component/function they belong to. The heart piece of the library is **general_definitions.h** since it contains all pin definitions, frequency definition, etc. **calibration_parameters.cpp** contains the calibration parameters for the LSM9DS1 which must be changed manually if a new calibration procedure is done.

## main
Main contains the code that is run on the can. It relies on external functions from the sensorsational_library and is kept small to keep it readable.

## calibration
calibration contains the calibration routines for the LSM9DS1 that have been taken from: https://github.com/FemmeVerbeek/Arduino_LSM9DS1/tree/master/examples. See also instructional video here: https://www.youtube.com/watch?v=BLvYFXoP33o. This calibration must be done in the real environment (inside the can with all components in place) **once** to determine the calibration parameters.

## magneto_calibration
magnet_calibration provides a better magnetic calibration than in the calibration folder. This sketch measure a large array of magnetic data and saves it as .txt file on the SD card. To get the calibration parameters, this .txt file must be loaded onto a computer and imported into the magneto v1.2 code (download here). The calibration parameters need to be input into the calibration_paramters.cpp file in the sensorsational_library.

## motor_test
motor_test is example code to set the BLDC motor speed for the propellers. To be done: implement functions that set motor speeds and can be called inside the main code and add them to the sensorsational_library.

## orientation
orientation contains example code on how to use the SensorFusion library to calculate the magnetic heading. To be done: implement functions that can be called inside the main code and add them to the sensorsational_library.

## receiver
receiver is example code for receiving the data (groundstation) from the radio module and outputting them to the serial.
