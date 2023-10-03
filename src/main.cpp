#include <Arduino.h>
#include <Romi32U4.h>
#include "BlueMotor.h"


BlueMotor motor;
Romi32U4ButtonB buttonB;
long timeToPrint = 0;
long now = 0;
long newPosition = 0;
long oldPosition = 0;
long sampleTime = 100;
int speedInRPM = 0;
int CPR = 270;
int motorEffort = 360;
unsigned char on = 0;
int adjustedEffort = 0;

void setup()
{
  Serial.begin(9600);
  motor.setup();
  motor.reset();
  
  while (!Serial);
  Serial.println("Time (ms),User Effort,Adj. Effort,Speed (RPM)");
  delay(3000);

}


void loop()
{

  timeToPrint = millis() + sampleTime;
  oldPosition = motor.getPosition();
  while (abs(motorEffort) <= 400)
  {
    adjustedEffort = motor.setEffortWithoutDB(motorEffort);
    if ((now = millis()) > timeToPrint)
    {
      adjustedEffort = motor.setEffortWithoutDB(motorEffort);
      timeToPrint = now + sampleTime;
      newPosition = motor.getPosition();
      speedInRPM = (newPosition - oldPosition) * 600 / CPR;
      Serial.print(now);
      Serial.print(",");
      Serial.print(motorEffort);
      Serial.print(",");
      Serial.print(adjustedEffort);
      Serial.print(",");
      Serial.println(speedInRPM);

      oldPosition = newPosition;

      motorEffort++;
    }
  }

  motor.setEffort(0);

}