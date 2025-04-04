#include "Adafruit_seesaw.h"

Adafruit_seesaw ss;
bool waterLow = false;
int desiredSaturation = 60;
bool flowerOrHerb = false; // true for flower; false for herb;

void setup() {
  Serial.begin(115200);

  // confirm the connection
  if (!ss.begin(0x36)) {
    Serial.println("ERROR! Soil Sensor not found");
    while(1) delay(1);
  } else {
    Serial.print("Soil Sensor started!");
  }
}

void loop() {
	// set desired saturation percentage based on flower or herb
	if (flower = false){
		desiredSaturation = 40;
	}else{
		desiredSaturation = 60;
	}

	// read soil moisture and determine if water is true.
	uint16_t soilMoisture = map(ss.touchRead(0), 0, 1023, 0, 100);
	if (soilMoisture < desiredSaturation){
		waterLow = true;
	}
	else {
		waterLow = false;
	}

  delay(100);
}
