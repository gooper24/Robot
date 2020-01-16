#include <Wire.h>

const int servo = 5;
const int pingPin = 6;
const int echoPin = 7;

int pwm, angle, distance;

void setup() {
  pinMode(servo, OUTPUT);
  pinMode(pingPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  Wire.begin(9);
  Wire.onRequest(requestEvent);

}

void loop() {
  for (int angle = 0; angle <= 180; angle++)
  {
    angle = angle % 180;
    servoPulse(angle);
    digitalWrite(pingPin, LOW);
    delayMicroseconds(2);
    digitalWrite(pingPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(pingPin, LOW);
    int duration = pulseIn(echoPin, HIGH);
    distance = microsecondsToCentimeters(duration);
    delay(20);
    if(angle % 5 == 0)
    {
      Serial.print(distance);
      Serial.print(" cm bij ");
      Serial.print(angle);
      Serial.print("graden \n");
    }
  }

}

void servoPulse(int angle)
{
  int pulseLength = (angle*11) + 500;
  digitalWrite(servo, HIGH);
  delayMicroseconds(pulseLength);
  digitalWrite(servo, LOW);
}

long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}

void requestEvent()
{
  Wire.write(angle);
  Wire.write(distance);
}
