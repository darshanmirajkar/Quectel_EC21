/* Example to Ping Test  */


#include <Arduino.h>
#include "Quectel_EC21.h"

#define LTE_RX_PIN 5
#define LTE_TX_PIN 2

unsigned long previousMillis = 0;        // will store last time LED was updated
// constants won't change:
const long interval = 10000;           // interval at which to blink (milliseconds)

QuectelEC21module lte;

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

	if (lte.disConnectNetwork())
	{
		Serial.println("Disconnected to internet");
	}

	if (lte.setAPN("move.dataxs.mobi"))
	{
		Serial.println("Set Operater APN");
	}else{
		Serial.println("Failed to set APN");
	}

	if (lte.connectNetwork())
	{
		Serial.println("Connected to Internet");
	}else{
		Serial.println("Failed to connect Internet");
	}

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
