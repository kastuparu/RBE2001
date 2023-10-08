#include <Arduino.h>
#include <Romi32U4.h>
#include "BlueMotor.h"
#include "BottomOutGripper.h"
#include "Timer.h"

BlueMotor motor;
BottomOutGripper gripper;
Romi32U4ButtonA buttonA;
Romi32U4ButtonB buttonB;
Romi32U4ButtonC buttonC;
Timer timer(500);
bool closed = true;

void platform();
void roof25Degrees();
void roof45Degrees();

void setup()
{
    // hardware: set fourbar to platform position so that encoders are zeroed

    Serial.begin(9600);
    motor.setup();
    motor.reset();
    gripper.setup();
}

void loop()
{
    // note: the methods platform(), roof25Degrees(), and roof45Degrees() are blocking for now, just to get the code working

    if (buttonA.isPressed())
        platform();
    if (buttonB.isPressed())
        roof25Degrees();
    if (buttonC.isPressed())
        roof45Degrees();

    
}

void platform()
{
    // places and removes a solar collector panel on the staging platform

    while (motor.getPosition() > 0)
        motor.moveTo(0);
    motor.setEffort(0);

    delay(200);

    gripper.close();

    delay(200);

    while (motor.getPosition() < 1000)
        motor.moveTo(1000);
    motor.setEffort(0);

    delay(200);

    while (motor.getPosition() > 0)
        motor.moveTo(0);
    motor.setEffort(0);

    delay(200);

    gripper.open();

    delay(200);
}

void roof45Degrees()
{
    // places and removes a solar collector panel on the 45 degree roof

    while (motor.getPosition() < 3500)
        motor.moveTo(3500);
    motor.setEffort(0);

    delay(200);

    gripper.close();

    delay(200);

    while (motor.getPosition() < 5000)
        motor.moveTo(5000);
    motor.setEffort(0);

    delay(200);

    while (motor.getPosition() > 3500)
        motor.moveTo(3500);
    motor.setEffort(0);

    delay(200);

    gripper.open();

    delay(200);
}

void roof25Degrees()
{
    // places and removes a solar collector panel on the 25 degree roof

    while (motor.getPosition() < 8000)
        motor.moveTo(8000);
    motor.setEffort(0);

    delay(200);

    gripper.close();

    delay(200);

    while (motor.getPosition() > 5500)
        motor.moveTo(5500);
    motor.setEffort(0);

    delay(200);

    while (motor.getPosition() < 8000)
        motor.moveTo(8000);
    motor.setEffort(0);

    delay(200);

    gripper.open();

    delay(200);
}