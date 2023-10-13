#include "Drivetrain.h"
#include <Romi32U4.h>
#include <Chassis.h>
#include <Rangefinder.h>
#include "states.h"

Chassis chassis(WHEEL_DIAM, TICKS_PER_REVOLUTION, TRACK_DIAMETER);
Rangefinder rangefinder(RANGEFINDER_ECHO, RANGEFINDER_TRIG);

Drivetrain::Drivetrain()
{
    if (ROOF_TYPE == DEG_25)
    {
        TARGET_DISTANCE_ROOF_NO_PANEL = 3;
        TARGET_DISTANCE_ROOF_PICKUP = 1.5;
        TARGET_DISTANCE_ROOF_YES_PANEL = 3;
    }
    else
    {
        TARGET_DISTANCE_ROOF_NO_PANEL = 15;
        TARGET_DISTANCE_ROOF_PICKUP = 10.15;
        TARGET_DISTANCE_ROOF_YES_PANEL = 12;
    }
}

void Drivetrain::setup()
{
    chassis.init();
    rangefinder.init();
}

void Drivetrain::stateMachine()
{
    if (paused)
        chassis.setMotorEfforts(0, 0);
    else
    {
        switch (state)
        {
            case PREP_ROOF_REMOVAL:
                driveUntilDistanceFrom(TARGET_DISTANCE_ROOF_NO_PANEL);
                break;
            case GRIP_ROOF_PANEL:
                driveUntilDistanceFrom(TARGET_DISTANCE_ROOF_PICKUP);
                break;
            case REMOVE_ROOF_PANEL:
                driveUntilDistanceFrom(TARGET_DISTANCE_ROOF_YES_PANEL);
                chassis.getLeftEncoderCount(true);
                break;
            case REMOVE_ROOF_PANEL_TURN:
                turnRightUntilLine();
                break;
            case DRIVE_ROOF_TO_CROSS:
                driveUntilIntersection();
                break;
            case DRIVE_ROOF_TO_CROSS_2:
                chassis.setWheelSpeeds(TARGET_DRIVING_SPEED, TARGET_DRIVING_SPEED);
                if (abs(chassis.getLeftEncoderCount()) >= cmToTicks(TRACK_DIAMETER / 2.0))
                {
                    chassis.setMotorEfforts(0, 0);
                    finished = true;
                }
                break;
            case DRIVE_ROOF_TO_CROSS_3:
                if (FIELD_SIDE == LEFT)
                    turnRightUntilLine();
                else
                    turnLeftUntilLine();
                break;
            case DRIVE_CROSS_TO_PLATFORM:
                driveUntilDistanceFrom(TARGET_DISTANCE_PRE_PLATFORM);
                break;
            case PLACE_PLATFORM_PANEL:
                finished = true;
                driveUntilDistanceFrom(TARGET_DISTANCE_PLATFORM);
                break;
            case RELEASE_PLATFORM_PANEL:
                finished = true;
                break;
            case GRIP_PLATFORM_PANEL:
                driveUntilDistanceFrom(TARGET_DISTANCE_PRE_PLATFORM);
                break;
            case REMOVE_PLATFORM_PANEL:
                turnRightUntilLine();
                break;
            case DRIVE_PLATFORM_TO_CROSS:
                driveUntilIntersection();
                break;
            case DRIVE_PLATFORM_TO_CROSS_2:
                chassis.setWheelSpeeds(TARGET_DRIVING_SPEED, TARGET_DRIVING_SPEED);
                if (abs(chassis.getLeftEncoderCount()) >= cmToTicks(TRACK_DIAMETER / 2.0))
                {
                    chassis.setMotorEfforts(0, 0);
                    finished = true;
                }
                break;
            case DRIVE_PLATFORM_TO_CROSS_3:
                if (FIELD_SIDE == LEFT)
                    turnLeftUntilLine();
                else
                    turnRightUntilLine();
                break;
            case DRIVE_CROSS_TO_ROOF:
                driveUntilDistanceFrom(TARGET_DISTANCE_ROOF_YES_PANEL);
                break;
            case PLACE_ROOF_PANEL:
                driveUntilDistanceFrom(TARGET_DISTANCE_ROOF_PICKUP);
                break;
            case RELEASE_ROOF_PANEL:
                driveUntilDistanceFrom(TARGET_DISTANCE_ROOF_NO_PANEL);
                break;
            case RELEASE_ROOF_PANEL_TURN:
                turnLeftUntilLine();
                break;
            case DRIVE_2_ROOF_TO_CROSS:
                driveUntilIntersection();
                break;
            case DRIVE_2_ROOF_TO_CROSS_2:
                chassis.setWheelSpeeds(TARGET_DRIVING_SPEED, TARGET_DRIVING_SPEED);
                if (abs(chassis.getLeftEncoderCount()) >= cmToTicks(5))
                {
                    chassis.setMotorEfforts(0, 0);
                    finished = true;
                }
                break;
            case DRIVE_2_ROOF_TO_CROSS_3:
                if (FIELD_SIDE == LEFT)
                    turnRightUntilLine();
                else
                    turnLeftUntilLine();
                break;
            case DRIVE_2_CROSS_TO_PLATFORM:
                driveUntilDistanceFrom(TARGET_DISTANCE_PRE_PLATFORM);
                break;
            case DRIVE_2_CROSS_TO_PLATFORM_TURN:
                if (FIELD_SIDE == LEFT)
                    chassis.turnFor(-90, TARGET_TURNING_SPEED, true);
                else
                    chassis.turnFor(90, TARGET_TURNING_SPEED, true);
                finished = true;
                break;
            case DRIVE_ACROSS_FIELD:
                driveUntilIntersection();
                break;
        }
    }
}

bool Drivetrain::getFinished()
{
    return finished;
}

void Drivetrain::switchState(StateRobot1 newState)
{
    state = newState;
    finished = false;
    chassis.getLeftEncoderCount(true);
}

void Drivetrain::togglePause()
{
    paused = !paused;
}

void Drivetrain::driveUntilIntersection()
{
    if (detectIntersection())
    {
        chassis.setMotorEfforts(0, 0);
        finished = true;
    }
    else
        lineFollow();
}

void Drivetrain::driveUntilDistanceFrom(float targetDistance)
{
    if (detectDistance(targetDistance, 0.5))
    {
        chassis.setTwist(0, 0);
        finished = true;
    }
    else
        lineFollow(getDrivingSpeed(targetDistance));
}

void Drivetrain::turnLeftUntilLine()
{
    chassis.turnFor(5, TARGET_TURNING_SPEED, false);
    if (detectLineTurnLeft() && abs(chassis.getLeftEncoderCount()) > 500)
        finished = true;
}

void Drivetrain::turnRightUntilLine()
{
    chassis.turnFor(-5, TARGET_TURNING_SPEED, false);
    if (detectLineTurnRight() && abs(chassis.getLeftEncoderCount()) > 500)
        finished = true;
}

void Drivetrain::lineFollow(float drivingSpeed) 
{
    chassis.setTwist(drivingSpeed, lineEffort());
}

float Drivetrain::lineEffort() {
    return KP_LINE * ((analogRead(LINE_LEFT) - analogRead(LINE_RIGHT))/100);
}

float Drivetrain::getDrivingSpeed(float targetDistance) {
    float error = rangefinder.getDistance() - targetDistance;
    if (error < 2)
        return KP_RANGEFINDER * error;
    return TARGET_DRIVING_SPEED;
}

bool Drivetrain::detectIntersection() {
    return analogRead(LINE_LEFT) > 620 && analogRead(LINE_RIGHT) > 620;
}

bool Drivetrain::detectLine()
{
    return analogRead(LINE_LEFT) > 150 && analogRead(LINE_RIGHT) > 150;
}

bool Drivetrain::detectLineTurnLeft()
{
    return analogRead(LINE_RIGHT) > 150;
}

bool Drivetrain::detectLineTurnRight()
{
    return analogRead(LINE_LEFT) > 150;
}

bool Drivetrain::detectDistance(float targetDistance, float tolerance)
{
    return abs(rangefinder.getDistance() - targetDistance) <= tolerance;
}

int Drivetrain::cmToTicks(float cm) {
    return cm * TICKS_PER_REVOLUTION / (WHEEL_DIAM * 3.14159);
}
