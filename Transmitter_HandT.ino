//Include Libraries
#include <SPI.h>
//#include <nRF24L01.h>
#include "RF24.h"
#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT22

//create an RF24 object
RF24 radio(9, 8);  // CE, CSN

//address through which two modules communicate.
const byte address[6] = "00001";

typedef struct payload {
  float temperature;
  float humidity;
} payload_t;

payload_t payload = { 0 };

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

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
  // Set the PA Level low to try preventing power supply related problems
  // because these examples are likely run with nodes in close proximity to
  // each other.
  radio.setPALevel(RF24_PA_LOW);  // RF24_PA_MAX is default.
  // save on transmission time by setting the radio to only transmit the
  // number of bytes we need to transmit a float
  radio.setPayloadSize(sizeof(payload));  // float datatype occupies 4 bytes
  //set the address
  radio.openWritingPipe(address);
  //Set module as transmitter
  radio.stopListening();
  dht.begin();
}
void loop() {

  payload.humidity = dht.readHumidity();
  payload.temperature = dht.readTemperature();

  //Send message to receiver
  bool report = radio.write(&payload, sizeof(payload_t));
  if (report) {
    Serial.print(F("Transmission successful! "));  // payload was delivered
    Serial.print(F("Humidity: "));
    Serial.print(payload.humidity);
    Serial.print(F("%  Temperature: "));
    Serial.print(payload.temperature);
    Serial.println(F("°C "));
  } else {
    Serial.println(F("Transmission failed or timed out"));  // payload was not delivered
  }

  delay(1000);
}
