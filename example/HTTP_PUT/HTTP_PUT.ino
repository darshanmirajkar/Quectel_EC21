/* Example to put http data to server and read response  */


#include <Arduino.h>
#include "Quectel_EC21.h"

QuectelEC21module lte;

void setup() {
	delay(5000); // To avoid ESP32 boot Fail
  	Serial.begin(115200);
	lte.setup();

  	delay(2000);
}



void loop() {
  	lte.PutHTTP("https://run.mocky.io/v3/04a417d2-5288-4daa-aa41-ce4c1d21fbea", "message",4);  // HTTPS Put
  	String response = lte.HTTPread();
  	Serial.println("Response = " + response);       //Espected Output Response = 200 {"status":"received"}
  	delay(10000);
}
