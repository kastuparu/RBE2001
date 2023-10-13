#include "Fourbar.h"
#include "states.h"

Fourbar::Fourbar()
{
    if (ROOF_TYPE == DEG_25)
    {
        ENC_CNT_ROOF_NO_PANEL = 5500;
        ENC_CNT_ROOF_PICKUP = 8000;
        ENC_CNT_ROOF_YES_PANEL = 5500;
    }
    else
    {
        ENC_CNT_ROOF_NO_PANEL = 3250;
        ENC_CNT_ROOF_PICKUP = 3600;
        ENC_CNT_ROOF_YES_PANEL = 6000;
    }
}

void Fourbar::setup()
{
    motor.setup();
}

void Fourbar::stateMachine()
{
    if (paused)
        motor.setEffort(0);
    else
    {
        switch (state)
        {
            case PREP_ROOF_REMOVAL:
                moveTo(ENC_CNT_ROOF_NO_PANEL);
                break;
            case GRIP_ROOF_PANEL:
                moveTo(ENC_CNT_ROOF_PICKUP);
                break;
            case REMOVE_ROOF_PANEL:
                moveTo(ENC_CNT_ROOF_YES_PANEL);
                break;
            case DRIVE_ROOF_TO_CROSS:
                moveTo(ENC_CNT_PRE_PLATFORM);
                break;
            case PLACE_PLATFORM_PANEL:
                moveTo(ENC_CNT_PLATFORM);
                break;
            case GRIP_PLATFORM_PANEL:
                moveTo(ENC_CNT_PRE_PLATFORM);
                break;
            case DRIVE_CROSS_TO_ROOF:
                moveTo(ENC_CNT_ROOF_YES_PANEL);
                break;
            case PLACE_ROOF_PANEL:
                moveTo(ENC_CNT_ROOF_PICKUP);
                break;
            case RELEASE_ROOF_PANEL:
                moveTo(ENC_CNT_ROOF_NO_PANEL);
                break;
            case DRIVE_2_ROOF_TO_CROSS:
                moveTo(ENC_CNT_PRE_PLATFORM);
                break;
            default:
                finished = true;
                break;
            }
    }
}

bool Fourbar::getFinished()
{
    return finished;
}

void Fourbar::switchState(StateRobot1 newState)
{
    state = newState;
    finished = false;
}

void Fourbar::togglePause()
{
    paused = !paused;
}

void Fourbar::moveTo(long encoderCount)
{
    if (motor.moveTo(encoderCount))
        finished = true;
}