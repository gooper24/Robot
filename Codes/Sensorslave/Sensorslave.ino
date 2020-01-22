#include <Wire.h> //Voor I2C

// SONAR RECHTS
const int pingPin2 = 8;
const int echoPin2 = 9;

// SONAR LINKS
const int pingPin3 = 11;
const int echoPin3 = 10;

const int pingPin = 7;
const int echoPin = 6;
const int IRLeft = 3;
const int IRRight = 2;
const int hallPin = 4; //hall sensor
int led = 13; // standaard LED op Arduino
int hallSensorValue;
long cm;
long cmR; // SONAR RIGHT
long cmL; // SONAR LEFT
bool leftLine;
bool rightLine;

void setup() {
  Serial.begin(74880);
  pinMode(pingPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(pingPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(pingPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
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
  detectObjectRight();
  detectObjectLeft();

  Serial.println(state());
  Serial.println(cmL);
  Serial.println(cmR);
  Serial.println(cm);
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

void detectObjectRight()
{
  long duration;
  digitalWrite(pingPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPin2, LOW);
  duration = pulseIn(echoPin2, HIGH);
  cmR = microsecondsToCentimeters(duration);
}

void detectObjectLeft()
{
  long duration;
  digitalWrite(pingPin3, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin3, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPin3, LOW);
  duration = pulseIn(echoPin3, HIGH);
  cmL = microsecondsToCentimeters(duration);
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
  if (cmR <= 5) return '2';
  if (cmL <= 5) return '3';
  if (leftLine && rightLine) return 'O';
  if (leftLine) return 'L';
  if (rightLine) return 'R';
  return '0';
}

void detectPerson() {
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
