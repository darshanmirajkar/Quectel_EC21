/* Example to Ping Test  */


#include <Arduino.h>
#include "Quectel_EC21.h"

unsigned long previousMillis = 0;        // will store last time LED was updated
// constants won't change:
const long interval = 10000;           // interval at which to blink (milliseconds)

QuectelEC21module lte;

void setup() {
	delay(5000); // To avoid ESP32 boot Fail
  	Serial.begin(115200);
	lte.setup();

  	delay(2000);
}



void loop() {
  	/* Ping */
  	unsigned long currentMillis = millis();

  	if (currentMillis - previousMillis >= interval) {
    		previousMillis = currentMillis;
    		if (lte.ping("8.8.8.8")) {
      			Serial.println("Ping Success!");
    		} else {
      			Serial.println("Ping Failed");
    		}
  	}
}
