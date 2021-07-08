/* Example to Module Information  */


#include <Arduino.h>
#include "LTE.h"

#define LTE_RX_PIN 5
#define LTE_TX_PIN 2

LTEmodule lte;

void setup() {
	delay(5000); // To avoid ESP32 boot Fail
  	Serial.begin(115200);
	lte.SelectSerial(&Serial1); //Select the serial port
	lte.begin(115200, SERIAL_8N1, LTE_RX_PIN, LTE_TX_PIN);
	lte.initilizeModule();
	Serial.println("\nSearching For network..");
	if (lte.checkforNetwork())
	{
		Serial.println("Network Found");
	}
	else
	{
		Serial.print(".");
	}

	if (lte.enableECHO())
	{
		Serial.println("Echo Enabled");
	}

}



void loop() {
	lte.getModuleInfo();
  	while(1);
}
