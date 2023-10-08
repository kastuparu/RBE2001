#pragma once

#include <servo32u4.h>

class LinearGripper
{
    public:
        LinearGripper();
        void setup();
        int getPosition();
        bool isOpen();
        bool isClosed();
        void open();
        void close();

    private:
        const int SERVO_PIN = 5;            // needs to be pin 5
        const int LINEAR_POT_PIN = A4;

        const int SERVO_STOP = 1500;
        const int SERVO_JAW_DOWN = 1300;
        const int SERVO_JAW_UP = 1700;

        const int LINEAR_POT_JAW_UP = 950;
        const int LINEAR_POT_JAW_DOWN = 750;

        Servo32U4 servo;
};