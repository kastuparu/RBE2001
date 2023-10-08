/*
#include <Arduino.h>
#include <Romi32U4.h>
#include "BlueMotor.h"
#include "LinearGripper.h"
#include "Timer.h"

Romi32U4ButtonA buttonA;
Romi32U4ButtonB buttonB;

LinearGripper gripper;
Timer timer(300);
bool closed = false;
int previousPosition = 0;

void setup()
{
  Serial.begin(9600);
  gripper.setup();
  timer.reset();
  previousPosition = gripper.getPosition();
}

void loop()
{
  if (buttonA.isPressed())
    closed = true;

  if (buttonB.isPressed())
    closed = false;

  if (closed)
    gripper.close();
  else
    gripper.open();

  if (timer.isExpired())
  {
    Serial.print("closed: ");
    Serial.print(closed);
    Serial.print("\t");
    Serial.println(gripper.getPosition());
    int currentPosition = gripper.getPosition();
    if (abs(currentPosition - previousPosition) == 0)
      closed = false;
    previousPosition = currentPosition;
    timer.reset();
  }
  
}
*/