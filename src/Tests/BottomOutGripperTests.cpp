/*
#include <Arduino.h>
#include <Romi32U4.h>
#include "BlueMotor.h"
#include "BottomOutGripper.h"
#include "Timer.h"

Romi32U4ButtonB buttonB;
Romi32U4ButtonA buttonA;
BottomOutGripper gripper;
Timer timer(200);
bool closed = true;

void setup()
{
  Serial.begin(9600);
  gripper.setup();
  timer.reset();
}

void loop()
{
  if (buttonB.isPressed())
    closed = false;
  if (buttonA.isPressed())
    closed = true;

  if (closed)
    gripper.close();
  else
    gripper.open();

  if (timer.isExpired())
  {
    int current_position = gripper.getPosition();
    if (current_position > 200)
      closed = false;
    timer.reset();
  }
}
*/