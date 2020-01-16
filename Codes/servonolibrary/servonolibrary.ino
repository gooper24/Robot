int servo = 6;

int angle;
int pwm;

void setup()
{
 pinMode(servo, OUTPUT);
}

void loop ()
{
 for (angle = 0; angle <= 180; angle += 1)  {
   servoPulse(servo, angle);  }
 for (angle = 180; angle >= 0; angle -= 1)  {
   servoPulse(servo, angle);  }
}
 
void servoPulse (int servo, int angle)
{
 pwm = (angle*11) + 500;      // Convert angle to microseconds
 digitalWrite(servo, HIGH);
 delayMicroseconds(pwm);
 digitalWrite(servo, LOW);  
 delay(1000);// Refresh cycle of servo
}
