#include "calibration_functions.h"

// Tunable parameters
const int numSamples = 1000; // Number of samples to take for each orientation
const float knownAngle = 180; // Known rotation angle in degrees for gyro calib
float samplingRate = 119.0; // Default to 119Hz (adjust based on setup)


void waitForUserInput() 
{
    // Clear the serial buffer
    while (Serial.available() > 0) {
        Serial.read();  // Discard any leftover characters
    }

    // Now wait for the user to press Enter (newline)
    while (Serial.available() == 0) {
        // Optionally, you can print a message or include a delay for responsiveness
        delay(10); // Small delay to prevent blocking other operations unnecessarily
    }

    // Read the input until the newline character is received
    String input = Serial.readStringUntil('\n');
}

void calibrateAccelerometer(Adafruit_LSM9DS1 &imu_object, float aX[2], float aY[2], float aZ[2]) 
{
    
    // X-axis pointing up
    Serial.println("Align the sensor with X-axis pointing up and press Enter to start measurement.");
    waitForUserInput();
    measureAccAxis("X+", imu_object, aX);
    Serial.print("X+ measured value: ");
    Serial.println(aX[0], 4);


    // X-axis pointing down
    Serial.println("Align the sensor with X-axis pointing down and press Enter to start measurement.");
    waitForUserInput();
    measureAccAxis("X-", imu_object, aX);
    Serial.print("X- measured value: ");
    Serial.println(aX[1], 4);

    // Y-axis pointing up
    Serial.println("Align the sensor with Y-axis pointing up and press Enter to start measurement.");
    waitForUserInput();
    measureAccAxis("Y+", imu_object, aY);
    Serial.print("Y+ measured value: ");
    Serial.println(aY[0], 4);

    // Y-axis pointing down
    Serial.println("Align the sensor with Y-axis pointing down and press Enter to start measurement.");
    waitForUserInput();
    measureAccAxis("Y-", imu_object, aY);
    Serial.print("Y- measured value: ");
    Serial.println(aY[1], 4);

    // Z-axis pointing up
    Serial.println("Align the sensor with Z-axis pointing up and press Enter to start measurement.");
    waitForUserInput();
    measureAccAxis("Z+", imu_object, aZ);
    Serial.print("Z+ measured value: ");
    Serial.println(aZ[0], 4);

    // Z-axis pointing down
    Serial.println("Align the sensor with Z-axis pointing down and press Enter to start measurement.");
    waitForUserInput();
    measureAccAxis("Z-", imu_object, aZ);
    Serial.print("Z- measured value: ");
    Serial.println(aZ[1], 4);

    // Calculate Offset and Slope
    //Offset = (max + min) / 2
    accOffsets[0] = (aX[0] + aX[1]) / 2.0;
    accOffsets[1] = (aY[0] + aY[1]) / 2.0;
    accOffsets[2] = (aZ[0] + aZ[1]) / 2.0;

    accSlopes[0] = (2.0 * gravity) / (aX[0] - aX[1]);
    accSlopes[1] = (2.0 * gravity) / (aY[0] - aY[1]);
    accSlopes[2] = (2.0 * gravity) / (aZ[0] - aZ[1]);

    // Display Results
    Serial.println("Acceleration Calibration complete. Results:");
    Serial.print("X-axis: Min = "); Serial.print(aX[1], 4); Serial.print(", Max = "); Serial.println(aX[0], 4);
    Serial.print("Y-axis: Min = "); Serial.print(aY[1], 4); Serial.print(", Max = "); Serial.println(aY[0], 4);
    Serial.print("Z-axis: Min = "); Serial.print(aZ[1], 4); Serial.print(", Max = "); Serial.println(aZ[0], 4);

    Serial.println("\nCalculated Offsets:");
    Serial.print("Offset X: "); Serial.println(accOffsets[0], 4);
    Serial.print("Offset Y: "); Serial.println(accOffsets[1], 4);
    Serial.print("Offset Z: "); Serial.println(accOffsets[2], 4);

    Serial.println("\nCalculated Slopes:");
    Serial.print("Slope X: "); Serial.println(accSlopes[0], 4);
    Serial.print("Slope Y: "); Serial.println(accSlopes[1], 4);
    Serial.print("Slope Z: "); Serial.println(accSlopes[2], 4);
}

void measureAccAxis(const char* description, Adafruit_LSM9DS1 &imu_object, float a[2]) 
{
    float sum = 0;
    for (int i = 0; i < numSamples; i++) 
    {
        get_lsm9ds1_data(imu_object, lsm_data);
        if (description[1] == '+')
        {
            if (description[0] == 'X')
                a[0] = max(a[0], lsm_data[0]);  
            else if (description[0] == 'Y')
                a[0] = max(a[0], lsm_data[1]);
            else if (description[0] == 'Z')
                a[0] = max(a[0], lsm_data[2]);
        }
        else if (description[1] == '-')
        {
            if (description[0] == 'X')
                a[1] = min(a[1], lsm_data[0]);  
            else if (description[0] == 'Y')
                a[1] = min(a[1], lsm_data[1]);
            else if (description[0] == 'Z')
                a[1] = min(a[1], lsm_data[2]); 
        }
    }
}

void calibrateGyroscope(Adafruit_LSM9DS1 &imu_object) 
{
    Serial.println("Keep the sensor still for gyroscope offset calibration. Press Enter to start.");
    waitForUserInput();
    float sumX = 0, sumY = 0, sumZ = 0;
    
    for (int i = 0; i < numSamples; i++) {
        get_lsm9ds1_data(imu_object, lsm_data);

        // Print gyro values every 50th reading
        if (i % 50 == 0) {
            Serial.print("Gyro X: "); Serial.print(lsm_data[3], 4);
            Serial.print("\tGyro Y: "); Serial.print(lsm_data[4], 4);
            Serial.print("\tGyro Z: "); Serial.println(lsm_data[5], 4);
        }

        sumX += lsm_data[3];
        sumY += lsm_data[4];
        sumZ += lsm_data[5];
    }

    gyroOffsets[0] = sumX / numSamples;
    gyroOffsets[1] = sumY / numSamples;
    gyroOffsets[2] = sumZ / numSamples;
    
    // Getting Gyro Slope
    // Measure integrated gyro angle for each axis (X, Y, Z)
    measureGyroAxis('X', imu_object);
    measureGyroAxis('Y', imu_object);
    measureGyroAxis('Z', imu_object);

    // Display Results
    Serial.println("Full Gyroscope Calibration complete. Results:");
    Serial.println("\nCalculated Offsets:");
    Serial.print("Offset X: "); Serial.println(gyroOffsets[0], 4);
    Serial.print("Offset Y: "); Serial.println(gyroOffsets[1], 4);
    Serial.print("Offset Z: "); Serial.println(gyroOffsets[2], 4);

    Serial.println("\nCalculated Slopes:");
    Serial.print("Slope X: "); Serial.println(gyroSlopes[0], 4);
    Serial.print("Slope Y: "); Serial.println(gyroSlopes[1], 4);
    Serial.print("Slope Z: "); Serial.println(gyroSlopes[2], 4);
}

void measureGyroAxis(char axis, Adafruit_LSM9DS1 &imu_object) 
{
    float sum = 0;
    float deltaTime = 1.0 / samplingRate;
    bool measurementInProgress = false;
    float angle = 0.0; // To store the integrated angle

    // Start measuring
    Serial.print("Rotate ");
    Serial.print(knownAngle);
    Serial.print(" degrees around ");
    Serial.print(axis);
    Serial.println("-axis. Press Enter to start measurement.");

    waitForUserInput();  // Wait for user input to start
    measurementInProgress = true;
    
    Serial.println("Measuring... Press Enter to stop.");
    while (measurementInProgress) {
        get_lsm9ds1_data(imu_object, lsm_data);

        // Sum gyro values based on the axis and accumulate the angle
        if (axis == 'X') sum += (lsm_data[3] - gyroOffsets[0]) * deltaTime;
        else if (axis == 'Y') sum += (lsm_data[4] - gyroOffsets[1]) * deltaTime;
        else if (axis == 'Z') sum += (lsm_data[5] - gyroOffsets[2]) * deltaTime;

        // Stop measuring if the user presses any key
        if (Serial.available() > 0) {
            Serial.read();  // Consume the input and stop the measurement
            measurementInProgress = false;
        }
    }

    // Convert the integrated value from radians to degrees
    angle = sum * (180.0 / PI);  // Convert from radians to degrees

    // Print the integrated angle after rotation
    Serial.print("Measure integrated angle of ");
    Serial.print(angle, 4);
    Serial.print(" degrees on ");
    Serial.print(axis);
    Serial.println("-axis.");

    // Calculate gyro slope: slope = known angle / integrated angle
    if (axis == 'X') gyroSlopes[0] = (knownAngle * (PI / 180.0)) / sum;
    else if (axis == 'Y') gyroSlopes[1] = (knownAngle * (PI / 180.0)) / sum;
    else if (axis == 'Z') gyroSlopes[2] = (knownAngle * (PI / 180.0)) / sum;
}

void calibrateMagnetometer(Adafruit_LSM9DS1 &imu_object, float mX[2], float mY[2], float mZ[2]) 
{
    Serial.println("Magnetometer calibration: Rotate the sensor in all directions.");
    Serial.println("Press Enter to start and press Enter again to stop.");

    waitForUserInput(); // Wait for the first Enter press to start

    int count = 0;
    bool calibrating = true;

    while (calibrating) {
        get_lsm9ds1_data(imu_object, lsm_data);
        // Update min and max values for each axis
        mX[0] = max(mX[0], lsm_data[6]);
        mX[1] = min(mX[1], lsm_data[6]);
        mY[0] = max(mY[0], lsm_data[7]);
        mY[1] = min(mY[1], lsm_data[7]);
        mZ[0] = max(mZ[0], lsm_data[8]);
        mZ[1] = min(mZ[1], lsm_data[8]);

        count++;
        if (count % 50 == 0) {
            // Print on the same line using carriage return
            Serial.print("\rCurrent Min and Max Values: ");
            Serial.print("Mag X: Min = "); Serial.print(mX[1], 4); Serial.print(", Max = "); Serial.print(mX[0], 4);
            Serial.print(" | Mag Y: Min = "); Serial.print(mY[1], 4); Serial.print(", Max = "); Serial.print(mY[0], 4);
            Serial.print(" | Mag Z: Min = "); Serial.print(mZ[1], 4); Serial.print(", Max = "); Serial.print(mZ[0], 4);
            Serial.print("          "); // Add extra spaces to clear any leftover characters
        }

        if (Serial.available() > 0) {
            Serial.read(); // Clear the input buffer
            calibrating = false; // Stop calibration on the second Enter press
        }
    }

    //Offset: (max + min) / 2
    magOffsets[0] = (mX[0] + mX[1]) / 2;
    magOffsets[1] = (mY[0] + mY[1]) / 2;
    magOffsets[2] = (mZ[0] + mZ[1]) / 2;
    //Slope: (2 * earth intensity) / (max - min)
    magSlopes[0]= (2 * mag_intensity) / (mX[0] - mX[1]);
    magSlopes[1] = (2 * mag_intensity) / (mY[0] - mY[1]);
    magSlopes[2] = (2 * mag_intensity) / (mZ[0] - mZ[1]);

    // Print the final results on a new line
    Serial.println("\nMagnetometer calibration complete. Final Min and Max Values:");
    Serial.print("Mag X: Min = "); Serial.print(mX[1], 4); Serial.print(", Max = "); Serial.println(mX[0], 4);
    Serial.print("Mag Y: Min = "); Serial.print(mY[1], 4); Serial.print(", Max = "); Serial.println(mY[0], 4);
    Serial.print("Mag Z: Min = "); Serial.print(mZ[1], 4); Serial.print(", Max = "); Serial.println(mZ[0], 4);

    Serial.println("\nCalculated Offsets:");
    Serial.print("Offset X: "); Serial.println(magOffsets[0], 4);
    Serial.print("Offset Y: "); Serial.println(magOffsets[1], 4);
    Serial.print("Offset Z: "); Serial.println(magOffsets[2], 4);

    Serial.println("\nCalculated Slopes:");
    Serial.print("Slope X: "); Serial.println(magSlopes[0], 4);
    Serial.print("Slope Y: "); Serial.println(magSlopes[1], 4);
    Serial.print("Slope Z: "); Serial.println(magSlopes[2], 4);
}