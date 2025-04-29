#include "Adafruit_seesaw.h"
#include <LiquidCrystal.h>
#include <RTClib.h>
#include <Wire.h>

// soil moisture subsystem
Adafruit_seesaw ss;
bool waterLow = true;
const int setSaturation = 1000;

// Watering mechanism
const int pumpPin = 9;
const int waterLevelSensorPin = A0;
const int lowWaterVal = 10;
bool tankLow = false;
bool pumpOn = false;

int pumpTime = 0;

// Light Detection and Emission
const int lightSensor = A1;
const int relaySwitchPin = 8;

// User interface
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal myLCD(rs, en, d4, d5, d6, d7);
int bVal = 0;
int mode = 0;
const int button = 7;
unsigned long previousMillis = 0;
const unsigned long interval = 500;

// Real-time clock
RTC_PCF8523 rtc;
const int sunrise = 6;
const int sunset = 18;

void setup() {
  Serial.begin(115200);
  // UI
  myLCD.begin(16, 2);
  pinMode(button, INPUT_PULLUP);


  // Soil
  if (!ss.begin(0x36)) {
    Serial.println("ERROR! Soil Sensor not found");
  }

  // Light
  pinMode(relaySwitchPin, OUTPUT);  // set relaySwitchPin for light emission to output

  // Time
  if (!rtc.begin()) {
    myLCD.print("Couldn't find RTC");
    Serial.println("Couldn't find RTC");
  }
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //sets time automaticalay from rtc
}

void loop() {
  /*
    Clock
  */
  DateTime now = rtc.now();
  /*
    Soil Moisture Subsystem
  */
  // read soil moisture
  uint16_t percent_soil = ss.touchRead(0);

  // determine if water is needed
  if (percent_soil < setSaturation) {
    waterLow = true;
  } else if (percent_soil == 65535) {
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
  if (sensorValue <= lowWaterVal) {
    tankLow = true;
  } else {
    tankLow = false;
  }

  if (waterLow && !tankLow && !pumpOn) {
    analogWrite(pumpPin, 255);
    pumpOn = true;
    pumpTime = millis();
  } else if (millis()-pumpTime > 5000 && pumpOn){
    analogWrite(pumpPin, 0);
  }

  if (millis()-pumpTime > 30000) pumpOn = false;


  /*
		Light detection and Emission
	*/
  int raw_light = analogRead(lightSensor);      // read the raw value from lightSensor pin (A1)
  int light = map(raw_light, 0, 1023, 0, 100);  // map the value from 0, 1023 to 0, 100

  // if conditions are right, turn the light on
  if (light <= 70 && now.hour() > sunrise && now.hour() < sunset) {
    digitalWrite(relaySwitchPin, HIGH);
  }

  // else, turn lights off
  else {
    digitalWrite(relaySwitchPin, LOW);
  }

  /*
    UI
  */
  bVal = !digitalRead(button);
  if (bVal == 1) {
    mode = (mode + 1) % 4;
    delay(200);  // debounce
  }

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    myLCD.clear();

    if (mode == 0) {
      myLCD.setCursor(0, 0);
      myLCD.print("Light Level: ");
      myLCD.print(light);

      if (light <= 70 && now.hour() > sunrise && now.hour() < sunset) {
        myLCD.setCursor(0, 1);
        myLCD.print("Lights: ON");
      } else {
        myLCD.setCursor(0, 1);
        myLCD.print("Lights: OFF");
      }


    } else if (mode == 1) {
        myLCD.setCursor(0, 0);
        myLCD.print("Water Tank Level:");
        myLCD.setCursor(0, 1);
        myLCD.print(tankLow ? "Low" : "Adequate");

    } else if (mode == 2) {
        myLCD.setCursor(0, 0);
        myLCD.print("Soil Moisture:");
        myLCD.setCursor(0, 1);
        myLCD.print(percent_soil >= setSaturation ? "Adequate" : "Low");

    } else if (mode == 3) {
        myLCD.setCursor(0, 0);
        myLCD.print("Time: ");
        myLCD.setCursor(0, 1);

       myLCD.setCursor(0, 1);

if (now.hour() < 10) {
  myLCD.print('0');
}
myLCD.print(now.hour());
myLCD.print(':');

if (now.minute() < 10) {
  myLCD.print('0');
}
myLCD.print(now.minute());
myLCD.print(':');

if (now.second() < 10) {
  myLCD.print('0');
}
myLCD.print(now.second());

    }
  }
  delay(100);
}
