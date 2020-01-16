#include <Wire.h> //Voor I2C

const int pingPin = 7;
const int echoPin = 6;
const int IRLeft = 2;
const int IRRight = 3;
long cm;
bool leftLine;
bool rightLine;

void setup() {
  Serial.begin(74880);
  pinMode(pingPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(IRLeft, INPUT_PULLUP);
  pinMode(IRRight, INPUT_PULLUP);
  Wire.begin(8);
  Wire.onRequest(requestEvent);
}

void loop() {
  detectEdge();
  detectLines();
      Serial.println(cm);
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
  leftLine = !digitalRead(IRLeft);
  rightLine = !digitalRead(IRRight);
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}

void requestEvent()
{
  Wire.write(state());
  bool edge = cm >= 15;
  if (edge)
  {
    Wire.write('1');
    Serial.println("Rand");
    
  }
  if (leftLine)
  {
    Wire.write('L');
    Serial.println("Links");
  }
  if (rightLine)
  {
    Wire.write('R');
    Serial.println("Rechts");
  }
  else(Wire.write('0'));
}

char state()
{
  bool edge = cm >= 15;
  if (edge) return '1';
  if leftLine && rightLine return 'O';
  if leftLine return 'L';
  if rightLine return 'R';
}
