#include "calibration_parameters.h"


float accOffsets[3] = {-0.1394, -0.5904, -0.2007};
float accSlopes[3] = {0.9890, 0.9904, 0.9811};

float gyroOffsets[3] = {-0.0111, -0.0135, -0.0359};
float gyroSlopes[3] = {0.9031, 0.9182, 0.9129};


float magBias[3] = {-5.849918, 18.364603, -10.727742};
float softIron[9] = {1.004949, 0.049753, 0.005626, 0.049753, 0.954857, -0.005598, 0.005626, -0.005598, 1.009333};  

float magOffsets[3] = {21.7188,11.3709, -6.4309};
float magSlopes[3] = {1.362870, 1.344244, 1.390133};

/*
float accOffsets[3] = {0.0,0.0, 0.0};
float accSlopes[3] = {1.0, 1.0, 1.0};

float gyroOffsets[3] = {0.0,0.0, 0.0};
float gyroSlopes[3] = {1.0, 1.0, 1.0};



float magOffsets[3] = {0.0,0.0, 0.0};
float magSlopes[3] = {1.0, 1.0, 1.0};
*/