#include "Adafruit_seesaw.h"
#include <RTClib.h>

Adafruit_seesaw ss;
RTC_PCF8523 rtc; 


void setup() {
  Serial.begin(115200);
  delay(4000);

  // real time clock
    if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
  }

  rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //sets time automaticalay from rtc

  // soil sensor
  if (!ss.begin(0x36)) {
    Serial.println("ERROR! seesaw not found");
  } 
}

void loop() {
  int percent_soil = map(ss.touchRead(0), 0, 1023, 0, 100); 

  if (percent_soil < 60) {
    Serial.println("Plant needs water");
  } else if (percent_soil == 6406) {
    ss.begin(0x36);
    Serial.println("Soil Sensor RESTART.");
  } else {
    Serial.println("The plant does not need water");
  }

  Serial.print("Time: ");
  Serial.println(rtc.now().secondstime());



  Serial.print("Capacitive: ");
  Serial.println(percent_soil);
  delay(100);
}
