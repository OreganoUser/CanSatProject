#include <Servo.h>

Servo esc1;  // ESC an Pin 10
Servo esc2;  // ESC an Pin 11

int speed1 = 0;  // Geschwindigkeit in Prozent (0–100)
int speed2 = 0;

void setup() {
  esc1.attach(13);
  esc2.attach(14);
  Serial.begin(9600);
  
  Serial.println("ESC Steuerung gestartet.");
  Serial.println("Gib z.B. ein: 75 80  (für 75% auf ESC1, 80% auf ESC2)");
  esc1.writeMicroseconds(0);
  esc2.writeMicroseconds(0);
}

void loop() {
  if (Serial.available()) {
    // Lese zwei Integer-Werte aus dem seriellen Monitor
    speed1 = Serial.parseInt();  // Wert für ESC1
    speed2 = Serial.parseInt();  // Wert für ESC2

    // Begrenzung: 0–100%
    speed1 = constrain(speed1, 0, 100);
    speed2 = constrain(speed2, 0, 100);

    // Umrechnung in Mikrosekunden (PWM für ESCs)
    int pwm1 = map(speed1, 0, 100, 1000, 2000);
    int pwm2 = map(speed2, 0, 100, 1000, 2000);

    // PWM-Signal senden
    esc1.writeMicroseconds(pwm1);
    esc2.writeMicroseconds(pwm2);

    Serial.print("ESC1: "); Serial.print(speed1); Serial.print("% → ");
    Serial.print(pwm1); Serial.print(" µs | ");
    Serial.print("ESC2: "); Serial.print(speed2); Serial.print("% → ");
    Serial.print(pwm2); Serial.println(" µs");
  }
}
