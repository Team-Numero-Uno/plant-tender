#include "Adafruit_seesaw.h"

Adafruit_seesaw ss;

void setup() {
  Serial.begin(115200);

  Serial.println("seesaw Soil Sensor example!");
  
  if (!ss.begin(0x36)) {
    Serial.println("ERROR! seesaw not found");
    while(1) delay(1);
  } else {
    Serial.print("seesaw started! version: ");
    Serial.println(ss.getVersion(), HEX);
  }
}

void loop() {
  //float tempC = ss.getTemp();
  uint16_t capread = ss.touchRead(0);
  capread = map(capread, 0, 1023, 0, 100);

  if (capread < 60){
    Serial.println("Plant needs water");
  } else{
    Serial.println("The plant does not need water");
  }

  //Serial.print("Temperature: "); Serial.print(tempC); Serial.println("*C");
  Serial.print("Capacitive: "); Serial.println(capread);
  delay(100);
}
