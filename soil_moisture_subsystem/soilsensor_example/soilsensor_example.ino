#include "Adafruit_seesaw.h"

Adafruit_seesaw ss;

void setup() {
  Serial.begin(115200);
  delay(4000);

  Serial.println("seesaw Soil Sensor example!");

  if (!ss.begin(0x36)) {
    Serial.println("ERROR! seesaw not found");
    while (1) delay(1);
  } else {
    Serial.print("seesaw started! version: ");
    Serial.println(ss.getVersion(), HEX);
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

  Serial.print("Capacitive: ");
  Serial.println(percent_soil);
  delay(100);
}
