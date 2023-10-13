#include <Arduino.h>
#include <Romi32U4.h>
#include <Timer.h>
#include <IRdecoder.h>
#include "RemoteConstants.h"

#include "Fourbar.h"
#include "Drivetrain.h"
#include "BottomOutGripper.h"
#include "states.h"

const int IR_REMOTE_PIN = 14;

int irReading;

Fourbar fourbar;
Drivetrain drivetrain;
BottomOutGripper gripper;
IRDecoder decoder(IR_REMOTE_PIN);

StateRobot1 state = PREP_ROOF_REMOVAL;

void stateMachine();
bool getFinished();
void switchState(StateRobot1 state);
void togglePause();

void setup()
{
    Serial.begin(9600);
    // hardware: set fourbar to platform position so that encoders are zeroed
    fourbar.setup();
    drivetrain.setup();
    gripper.setup();
    decoder.init();
}

void loop()
{
    stateMachine();
}

void stateMachine()
{
    irReading = decoder.getKeyCode();
    if (irReading == REMOTE_PLAY_PAUSE)
        togglePause();

    fourbar.stateMachine();
    drivetrain.stateMachine();

    switch (state)
    {
        case PREP_ROOF_REMOVAL:
            gripper.open();
            if (getFinished())
                switchState(GRIP_ROOF_PANEL);
            break;
        case GRIP_ROOF_PANEL:
            if (getFinished())
                gripper.close();
            if (getFinished() && irReading == REMOTE_CONTINUE)
                switchState(REMOVE_ROOF_PANEL);
            break;
        case REMOVE_ROOF_PANEL:
            if (getFinished())
                switchState(REMOVE_ROOF_PANEL_TURN);
            break;
        case REMOVE_ROOF_PANEL_TURN:
            if (getFinished())
                switchState(DRIVE_ROOF_TO_CROSS);
            break;
        case DRIVE_ROOF_TO_CROSS:
            if (getFinished())
                switchState(DRIVE_ROOF_TO_CROSS_2);
            break;
        case DRIVE_ROOF_TO_CROSS_2:
            if (getFinished())
                switchState(DRIVE_ROOF_TO_CROSS_3);
            break;
        case DRIVE_ROOF_TO_CROSS_3:
            if (getFinished())
                switchState(DRIVE_CROSS_TO_PLATFORM);
            break;
        case DRIVE_CROSS_TO_PLATFORM:
            if (getFinished())
                switchState(PLACE_PLATFORM_PANEL);
            break;
        case PLACE_PLATFORM_PANEL:
            if (getFinished() && irReading == REMOTE_CONTINUE)
                switchState(RELEASE_PLATFORM_PANEL);
            break;
        case RELEASE_PLATFORM_PANEL:
            gripper.open();
            if (getFinished() && irReading == REMOTE_CONTINUE)
                switchState(GRIP_PLATFORM_PANEL);
            break;
        case GRIP_PLATFORM_PANEL:
            gripper.close();
            if (getFinished())
                switchState(REMOVE_PLATFORM_PANEL);
            break;
        case REMOVE_PLATFORM_PANEL:
            if (getFinished())
                switchState(DRIVE_PLATFORM_TO_CROSS);
            break;
        case DRIVE_PLATFORM_TO_CROSS:
            if (getFinished())
                switchState(DRIVE_PLATFORM_TO_CROSS_2);
            break;
        case DRIVE_PLATFORM_TO_CROSS_2:
            if (getFinished())
                switchState(DRIVE_PLATFORM_TO_CROSS_3);
            break;
        case DRIVE_PLATFORM_TO_CROSS_3:
            if (getFinished())
                switchState(DRIVE_CROSS_TO_ROOF);
            break;
        case DRIVE_CROSS_TO_ROOF:
            if (getFinished())
                switchState(PLACE_ROOF_PANEL);
            break;
        case PLACE_ROOF_PANEL:
            if (getFinished() && irReading == REMOTE_CONTINUE)
                switchState(RELEASE_ROOF_PANEL);
            break;
        case RELEASE_ROOF_PANEL:
            gripper.open();
            if (getFinished())
                switchState(RELEASE_ROOF_PANEL_TURN);
            break;
        case RELEASE_ROOF_PANEL_TURN:
            if (getFinished())
                switchState(DRIVE_2_ROOF_TO_CROSS);
            break;
        case DRIVE_2_ROOF_TO_CROSS:
            if (getFinished())
                switchState(DRIVE_2_ROOF_TO_CROSS_2);
            break;
        case DRIVE_2_ROOF_TO_CROSS_2:
            if (getFinished())
                switchState(DRIVE_2_ROOF_TO_CROSS_3);
            break;
        case DRIVE_2_ROOF_TO_CROSS_3:
            if (getFinished())
                switchState(DRIVE_2_CROSS_TO_PLATFORM);
            break;
        case DRIVE_2_CROSS_TO_PLATFORM:
            if (getFinished())
                switchState(DRIVE_2_CROSS_TO_PLATFORM_TURN);
            break;
        case DRIVE_2_CROSS_TO_PLATFORM_TURN:
            if (getFinished())
                switchState(DRIVE_ACROSS_FIELD);
            break;
        case DRIVE_ACROSS_FIELD:
            break;
    }
}

bool getFinished()
{
    return fourbar.getFinished() && drivetrain.getFinished();
}

void switchState(StateRobot1 newState)
{
    state = newState;
    fourbar.switchState(newState);
    drivetrain.switchState(newState);
}

void togglePause()
{
    fourbar.togglePause();
    drivetrain.togglePause();
}