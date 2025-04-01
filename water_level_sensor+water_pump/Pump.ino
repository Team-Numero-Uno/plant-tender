//Pins can be changed
const int pumpPin = 9;
const int waterSensorPin = A0;

//Place holder variables
bool needsWater = true;
bool waterLow = true;

void setup(){

  //Used for serial.print
  Serial.begin(9600);
}

void loop(){
  //reads value from water level sensor
  int sensorValue = analogRead(waterSensorPin);

  //Serial.print is for testing and isn't required
  Serial.print ("Water Level: ");
  Serial.println(sensorValue);

  //makes code easier to understand but can be removed
  if (sensorValue >= 10){
    waterLow = false;
  }
  else {
    waterLow = true;
  }

  //turns pump on and off
  if (needsWater == true && waterLow == false){
  	analogWrite(pumpPin, 255);
  }
  else {
    analogWrite(pumpPin, 0);
  }

  delay (1000);
}