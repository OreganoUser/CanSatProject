#include <SPI.h>
#include <RH_RF95.h>

const int _SPI_RX = 4;
const int _SPI_TX = 7;
const int _SPI_SCK = 2;

#define RF95_CS 8
#define RF95_RST 9
#define RF95_G0 0

#define RF95_FREQ 433.4  // Frequency must match the sender's frequency

// sender/receiver address
#define SENDER_ADDRESS 42
#define RECEIVER_ADDRESS 211

// Singleton instance of the radio driver
RH_RF95 rf95(RF95_CS, RF95_G0);

void setup() 
{
  SPI.setRX(_SPI_RX);
  SPI.setTX(_SPI_TX);
  SPI.setSCK(_SPI_SCK);

  while (!Serial);
  Serial.begin(115200);
  delay(100);

  pinMode(RF95_RST, OUTPUT);
  digitalWrite(RF95_RST, HIGH);
  delay(100);
  // Manual reset
  digitalWrite(RF95_RST, LOW);
  delay(10);
  digitalWrite(RF95_RST, HIGH);
  delay(10);

  bool rf95_init_status;
  rf95_init_status = rf95.init();
  if (!rf95_init_status)
    Serial.println("RF95 init failed!");
  else
    Serial.println("RF95 init success.");
  if (!rf95.setFrequency(RF95_FREQ))
      Serial.println("RF95 setFrequency failed!");


  // power from 5 to 23 dB. Always set false!
  rf95.setTxPower(23, false);
  //Serial.println("LoRa radio init OK!");
  //Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
  rf95.setSpreadingFactor(8);  // Adjust based on your needs
  rf95.setSignalBandwidth(125E3);  // Adjust bandwidth for your needs
  rf95.setCodingRate4(5);  // Coding rate for range vs message length
  rf95.setTxPower(23, false);  // Set transmission power

  // Set the receiver address (the receiver listens for messages sent to this address)
  rf95.setThisAddress(RECEIVER_ADDRESS);

}

void loop() {
  if (rf95.available()) {  // Check if data is available
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];  // Create a buffer for received data
    uint8_t len = sizeof(buf);  // Set the length of the buffer

    // If data is received, print it
    if (rf95.recv(buf, &len)) {
      buf[len] = '\0';  // Null-terminate to ensure correct string output
      Serial.print((char*)buf);
      Serial.print(",");
      Serial.println(rf95.lastRssi(), DEC);  // Print the signal strength
    }
  }
}
