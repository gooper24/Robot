int servo = 7;

int angle;
int pwm;

void setup()
{
 pinMode(servo, OUTPUT);
}

void loop ()
{
 for (angle = 0; angle <= 140; angle += 1)  {
   servoPulse(servo, angle);  }
 for (angle = 140; angle >= 0; angle -= 1)  {
   servoPulse(servo, angle);  }
}
 
void servoPulse (int servo, int angle)
{
 pwm = (angle*11) + 500;      // Convert angle to microseconds
 digitalWrite(servo, HIGH);
 delayMicroseconds(pwm);
 digitalWrite(servo, LOW);  
 delay(50);// Refresh cycle of servo
}
