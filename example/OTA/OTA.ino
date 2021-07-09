/* Example to OTA */


#include <Arduino.h>
#include "Quectel_EC21.h"

#define LTE_RX_PIN 5
#define LTE_TX_PIN 2

char* downloadURL = "https://2005.filemail.com/api/file/get?filekey=8sj-7kgCWq5aq_GY1s_vM7DScpoMSZKTrHcXvcmp2j5RD6o7RId3JilWCDFfJQ";
char* checksum = "MD5 checksum_of_file";

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
  	/* OTA Example */
  	lte.downloadUpdate(downloadURL, checksum);
  	while(1);
  }
