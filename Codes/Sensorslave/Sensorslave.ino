#include <Wire.h> //Voor I2C

const int pingPin = 7;
const int echoPin = 6;
long cm;

void setup() {
  pinMode(pingPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Wire.begin(8);
  Wire.onRequest(requestEvent);
}

void loop() {
   long duration;
   digitalWrite(pingPin, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin, LOW);
   duration = pulseIn(echoPin, HIGH);
   cm = microsecondsToCentimeters(duration);
}

long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}

void requestEvent()
{
  bool edge = cm >= 5;
  if(edge)
  {
    Wire.write('1');
  }
  else(Wire.write('0'));
}
