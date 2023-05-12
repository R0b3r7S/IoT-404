//Include Libraries
#include <SPI.h>
//#include <nRF24L01.h>
#include "RF24.h"

//create an RF24 object
RF24 radio(9, 8);  // CE, CSN

//address through which two modules communicate.
const byte address[6] = "00001";

typedef struct payload {
  float temperature;
  float humidity;
} payload_t;

payload_t payload = { 0 };

void setup() {

  Serial.begin(115200);
  while (!Serial) {
    // some boards need to wait to ensure access to serial over USB
  }
  // initialize the transceiver on the SPI bus
  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {}  // hold in infinite loop
  }
  radio.setChannel(1);
  // Set the PA Level low to try preventing power supply related problems
  // because these examples are likely run with nodes in close proximity to
  // each other.
  radio.setPALevel(RF24_PA_LOW);  // RF24_PA_MAX is default.
  // save on transmission time by setting the radio to only transmit the
  // number of bytes we need to transmit a float
  radio.setPayloadSize(sizeof(payload));  // float datatype occupies 4 bytes
  //set the address
  radio.openReadingPipe(0, address);
  //Set module as receiver
  radio.startListening();
}

void loop() {

  uint8_t pipe;

  if (radio.available(&pipe)) {              // is there a payload? get the pipe number that recieved it
    uint8_t bytes = radio.getPayloadSize();  // get the size of the payload
    radio.read(&payload, bytes);             // fetch payload from FIFO
    Serial.print(F("Received "));
    Serial.print(bytes);  // print the size of the payload
    Serial.print(F(" bytes on pipe "));
    Serial.print(pipe);  // print the pipe number
    Serial.print(F(": "));
    Serial.print(F("Transmission successful! "));  // payload was delivered
    Serial.print(F("Humidity: "));
    Serial.print(payload.humidity);
    Serial.print(F("%  Temperature: "));
    Serial.print(payload.temperature);
    Serial.println(F("°C "));
  }
}
