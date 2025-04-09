#include "Adafruit_seesaw.h"

// soil moisture subsystem
Adafruit_seesaw ss;
bool waterLow = false;
const int setSaturation = 60;

// Watering mechanism
const int pumpPin = 9;
const int waterLevelSensorPin = A0;
const int lowWaterVal = 10;
bool tankLow = false;

// Light Detection and Emission
const int lightSensor = A1; 
const int relaySwitchPin = 7;

void setup() {
  Serial.begin(115200);

  // confirm the connection
  if (!ss.begin(0x36)) {
	Serial.println("ERROR! Soil Sensor not found");
    while(1) delay(1);
  } else {
    Serial.print("Soil Sensor started!");
  }
	
	pinMode(relaySwitchPin, OUTPUT); // set relaySwitchPin for light emission to output
}

void loop() {
  /*
    Soil Moisture Subsystem
  */
	// read soil moisture 
	uint16_t percent_soil = map(ss.touchRead(0), 0, 1023, 0, 100);

  // determine if water is needed 
  if (percent_soil < setSaturation) {
    waterLow = true;
  } else if (percent_soil == 6406) {
    // soil sensor error; restart
    ss.begin(0x36);
    Serial.println("Soil Sensor RESTART.");
  } else {
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

	/*
		Light detection and Emission
	*/
  int raw_light = analogRead(lightSensor); // read the raw value from lightSensor pin (A1)
  int light = map(raw_light, 0, 1023, 0, 100); // map the value from 0, 1023 to 0, 100
  
  // if conditions are right, turn the light on
  if (light <= 40) 
  {
    digitalWrite(relaySwitchPin, HIGH);
  }

  // else, turn lights off
  else
  {
		digitalWrite(relaySwitchPin, LOW);
  }
 
  Serial.print("Light level: "); 
  Serial.println(light); // print the light value in Serial Monitor

  delay(100);
}
