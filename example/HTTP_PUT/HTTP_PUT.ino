/* Example to put http data to server and read response  */


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
  	lte.PutHTTP("https://run.mocky.io/v3/04a417d2-5288-4daa-aa41-ce4c1d21fbea", "message");  // HTTPS Put
  	String response = lte.HTTPread();
  	Serial.println("Response = " + response);       //Espected Output Response = 200 {"status":"received"}
  	delay(10000);
}
