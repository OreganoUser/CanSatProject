The magneto program provides a better magnetic calibration than the simple model used in /calibration.

# Procedure
- Execute the the magneto_calibration.ino sketch and rotate the can around. Make sure to get all possible positions of the can!
- The data is measured during a certain duration (can be set in the sketch).
- After the measurement is done, take the created .txt file from the SD card and transfer it to the computer.
- Load the .txt file into the magneto program, set the magnetic field strength (taken from here: [Magnetic Field Strenght Calculator](https://www.ngdc.noaa.gov/geomag/calculators/magcalc.shtml#igrfwmm) ) and execute.
- Write the parameters into the calibration_parameters.cpp file (combined bias -> magBias and the 9 values are written to softIron in the following order: upper left, upper centre, upper right, second row left, ...).
