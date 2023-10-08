/*
#include <Arduino.h>
#include <Romi32U4.h>
#include "BlueMotor.h"
#include "BottomOutGripper.h"
#include "Timer.h"

BlueMotor motor;

void setup()
{
    // hardware: set fourbar to platform position

    Serial.begin(9600);
    motor.setup();
    motor.reset();
}

void loop()
{
    while (1)
    {
        motor.moveTo(7000);
        Serial.println(motor.getPosition());
    }
}

void platform()
{

}

void roof45Degrees()
{
    motor.moveTo(5000);
    // 
}

void roof25Degrees()
{

}
*/