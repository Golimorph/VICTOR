#include <Wire.h> // Include the required Wire library for I2C
#include <victorUltrasonic.h> //ultrasonic sensor

VictorUltrasonic VU(12,13);

void setup() {
  Wire.begin(); // Start the I2C Bus as Master
  Serial.begin(9600);
}

int distance=20;
void loop() {
  //I2C
  int newDistance=VU.getDistance();
  if(abs(newDistance-distance)>10)
  {
    delay(50);
    newDistance=VU.getDistance();
  }
  distance=(newDistance+distance)/2;
  Wire.beginTransmission(9); // transmit to device #9
  Wire.write(distance);              // sends x 
  Wire.endTransmission();    // stop transmitting
  delay(50);
  //Serial.println(distance);
}
