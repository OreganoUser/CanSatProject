#include <Servo.h>

Servo esc; // Create a Servo object to control the ESC
const int escPin = 14; // PWM signal pin for the ESC

// Define pulse widths
const int minPulseWidth = 1000; // Minimum pulse width (in microseconds)
const int maxPulseWidth = 1500; // Maximum pulse width (in microseconds)

void setup() {
  esc.attach(escPin); // Attach the ESC to the specified pin
  esc.writeMicroseconds(0); // Ensure the motor is off

  Serial.begin(9600); // Initialize serial communication
  while (!Serial) {
    // Wait for the Serial Monitor to open
  }
  Serial.println("Connect the battery and type 'go' to start arming...");

  // Wait for user input to confirm battery connection
  String input = "";
  while (input != "go") {
    if (Serial.available()) {
      input = Serial.readStringUntil('\n'); // Read user input
      input.trim(); // Remove any trailing spaces or newlines
      if (input != "go") {
        Serial.println("Type 'go' to confirm the battery is connected.");
      }
    }
  }

  Serial.println("ESC armed. Enter motor power percentage (0-100) in the loop.");
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n'); // Read user input
    input.trim(); // Remove any trailing spaces or newlines
    int percentage = input.toInt(); // Convert input to integer

    if (percentage >= 0 && percentage <= 100) {
      int pulseWidth = map(percentage, 0, 100, minPulseWidth, maxPulseWidth);
      esc.writeMicroseconds(pulseWidth); // Set motor speed based on percentage
      Serial.print("Motor running at ");
      Serial.print(percentage);
      Serial.println("% power.");
    } else {
      Serial.println("Please enter a valid percentage (0-100).");
    }
  }
}
