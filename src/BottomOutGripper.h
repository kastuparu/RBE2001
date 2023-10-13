#pragma once

#include <servo32u4.h>

class BottomOutGripper
{
    public:
        BottomOutGripper();
        void setup();
        void open();
        void close();

    private:
        const int SERVO_PIN = 5;            // needs to be pin 5

        const int SERVO_OPEN = 1500;
        const int SERVO_CLOSED = 0;

        Servo32U4 servo;
};