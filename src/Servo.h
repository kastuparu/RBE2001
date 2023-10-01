#pragma once

class Servo
{
public:
    Servo();
    void setGripper();
    void setGripperDB();
    void setup();

private:
    const int linearPotPin = 18;
    const int servoStop = 1480;
    const int sevroJawDown = 1300;
    const int servoJawUp = 1700;
    const int printDelay = 500;
    const int linearPotVoltageADC = 500;
    const int jawOpenPotVoltageADC = 600;
    const int jawClosedPotVoltageADC = 940;
};