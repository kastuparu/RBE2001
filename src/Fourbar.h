#pragma once

#include "BlueMotor.h"
#include "states.h"

#include <string.h>

class Fourbar
{
    public:
        Fourbar();
        void setup();
        void stateMachine();
        bool getFinished();
        void switchState(StateRobot1 newState);
        void togglePause();

    private:
        BlueMotor motor;

        StateRobot1 state;
        bool finished = false;
        bool paused = false;

        int ENC_CNT_PLATFORM = 0;
        int ENC_CNT_PRE_PLATFORM = 1500;
        int ENC_CNT_ROOF_NO_PANEL;
        int ENC_CNT_ROOF_PICKUP;
        int ENC_CNT_ROOF_YES_PANEL;

        void moveTo(long encoderCount);
};