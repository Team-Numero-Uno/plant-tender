#include "Adafruit_seesaw.h"

// soil moisture subsystem
Adafruit_seesaw ss;
bool waterLow = false;
int desiredSaturation = 60;
bool flowerOrHerb = false; // true for flower; false for herb;

// Watering mechanism
const int pumpPin = 9;
const int waterLevelSensorPin = A0;
const int lowWaterVal = 10;
bool tankLow = false;

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
  /*
    Soil Moisture Subsystem
  */
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
  
  /*
    Watering Mechanism
  */
  int sensorValue = analogRead(waterLevelSensorPin);
  if (sensorValue >= lowWaterVal){
    tankLow = false;
  }else {
    tankLow = true;
  }
  
  if (waterLow && !tankLow){
    analogWrite(pumpPin, 255);
  }else{
    analogWrite(pumpPin, 0);
  }

  delay(100);
}
