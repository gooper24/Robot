#include <Wire.h> //Voor I2C

const int pingPin = 7;
const int echoPin = 6;
const int IRLeft = 3;
const int IRRight = 2;
const int hallPin = 4; //hall sensor
int led = 13; // standaard LED op Arduino
int hallSensorValue;
long cm;
bool leftLine;
bool rightLine;

void setup() {
  Serial.begin(74880);
  pinMode(pingPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(IRLeft, INPUT_PULLUP);
  pinMode(IRRight, INPUT_PULLUP);
  pinMode(hallPin, INPUT);
  pinMode (led, OUTPUT);
  Wire.begin(8);
  Wire.onRequest(requestEvent);
}

void loop() {
  detectEdge();
  detectLines();
  Serial.println(state());
  detectPerson();
}

void detectEdge()
{
  long duration;
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  cm = microsecondsToCentimeters(duration);
}

void detectLines()
{
  leftLine = digitalRead(IRLeft);
  rightLine = digitalRead(IRRight);
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}

void requestEvent()
{
  Wire.write(state());
  Serial.println(state());
}

char state()
{
  if (cm >= 15) return '1';
  if (leftLine && rightLine) return 'O';
  if (leftLine) return 'L';
  if (rightLine) return 'R';
  return '0';
}

void detectPerson(){
  hallSensorValue = digitalRead(hallPin) ; 
  if (hallSensorValue == HIGH) // When magnetic field is present, Arduino LED is on
  {
    digitalWrite (led, HIGH);
  }
  else
  {
    digitalWrite (led, LOW);
  }
}
