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
int motorEffort = 400;
unsigned char on = 0;

void setup()
{
  Serial.begin(9600);
  motor.setup();
  motor.reset();
  
  while (!Serial);
  Serial.print("Time (ms)");
  Serial.print("   ");
  Serial.print("Position");
  Serial.print("    ");
  Serial.println("speedInRPM");
  delay(3000);
}


void loop()
{
  timeToPrint = millis() + sampleTime;
  oldPosition = motor.getPosition();

  while (1)
  {
    // The button is currently pressed.
    motor.setEffort(motorEffort);
    if ((now = millis()) > timeToPrint)
    {
      timeToPrint = now + sampleTime;
      newPosition = motor.getPosition();
      speedInRPM = (newPosition - oldPosition) * 600 / CPR;

      if (speedInRPM != 0) {
      Serial.print(now);
      Serial.print("          ");
      Serial.print(newPosition);
      Serial.print("          ");
      Serial.println(speedInRPM);
      oldPosition = newPosition;
      }
    }
  }
    
  motor.setEffort(0);
  motor.moveTo(50, 45, true);
  motor.moveTo(50, 45, true);
  motor.moveTo(50, 25, true);
  motor.moveTo(50, 25, true);
}