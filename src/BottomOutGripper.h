#pragma once

#include <servo32u4.h>

class BottomOutGripper
{
    public:
        BottomOutGripper();
        void setup();
        int getPosition();
        bool isOpen();
        bool isClosed();
        void open();
        void close();

    private:
        const int SERVO_CONTROL_PIN = 5;        // needs to be pin 5
        const int SERVO_FEEDBACK_PIN = A0;

        const int SERVO_FEEDBACK_OPEN = 285;
        const int SERVO_FEEDBACK_CLOSED = 200;

        const int SERVO_CONTROL_OPEN = 1500;
        const int SERVO_CONTROL_CLOSED = 550;

        Servo32U4 servo;
};