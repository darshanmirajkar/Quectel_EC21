/* Example to SIM Information  */


#include <Arduino.h>
#include "Quectel_EC21.h"

QuectelEC21module lte;

void setup() {
	delay(5000); // To avoid ESP32 boot Fail
  	Serial.begin(115200);
	lte.basicSetup();
}



void loop() {
	lte.getSimInfo();
  	while(1);
}
