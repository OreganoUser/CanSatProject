#include <Servo.h>

Servo myServo;  // Servo-Objekt erstellen
int servoPin = 11; // Pin, an dem der Servo angeschlossen ist
int servoPosition = 90; // Aktuelle Position des Servos

void setup() {
  myServo.attach(servoPin); // Servo an den Pin anhängen
  myServo.write(servoPosition); // Servo auf die Startposition (90 Grad) setzen
  Serial.begin(9600); // Serielle Kommunikation starten
  Serial.println("Gib '1' fuer 90 Grad oder '0' fuer 0 Grad ein.");
}

void loop() {
  if (Serial.available() > 0) {
    char input = Serial.read();

    if (input == '1') {
      servoPosition = 90;
      myServo.write(servoPosition);
      Serial.println("Servo auf 90 Grad gedreht.");
    } else if (input == '0') {
      servoPosition = 0;
      myServo.write(servoPosition);
      Serial.println("Servo auf 0 Grad gedreht.");
    } else {
      Serial.println("Ungueltige Eingabe. Gib '1' oder '0' ein.");
    }
  }
}