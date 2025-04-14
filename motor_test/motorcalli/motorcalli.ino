#include <Servo.h>

Servo esc1;  // ESC an Pin 10
Servo esc2;  // ESC an Pin 11

void setup() {
  esc1.attach(13);
  esc2.attach(14);
  Serial.begin(9600);

  Serial.println(">>> JETZT Stromversorgung der ESCs anschließen!");
  delay(5000);  // Zeit, um Akku anzuschließen

  Serial.println("Sende MAX-Gas (2000 µs) an beide ESCs...");
  esc1.writeMicroseconds(2000);
  esc2.writeMicroseconds(2000);
  delay(3000);  // ESC piept und erkennt Max-Gas

  Serial.println("Sende MIN-Gas (1000 µs) an beide ESCs...");
  esc1.writeMicroseconds(1000);
  esc2.writeMicroseconds(1000);
  delay(3000);  // ESC speichert die Werte

  Serial.println("Kalibrierung abgeschlossen.");
}

void loop() {
  // ESC-Testcode kann hier eingefügt werden, z.B. Gassteuerung
}
