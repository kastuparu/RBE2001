#pragma once

#include <Romi32U4.h>
#include <Chassis.h>
#include <Rangefinder.h>
#include "states.h"
#include "BlueMotor.h"

const float WHEEL_DIAM = 7;                 // cm
const float TICKS_PER_REVOLUTION = 1440;
const float TRACK_DIAMETER = 14.1;          // cm

const float TARGET_DRIVING_SPEED = 10;      // cm/s
const float TARGET_TURNING_SPEED = 90;      // deg/s
const int LINE_LEFT = A4;
const int LINE_RIGHT = A3;
const int RANGEFINDER_ECHO = 17;
const int RANGEFINDER_TRIG = 12;

class Drivetrain
{  
    public:
        
        Drivetrain();
        void setup();
        void stateMachine();
        bool getFinished();
        void switchState(StateRobot1 newState);
        void togglePause();

    private:

        StateRobot1 state;
        bool finished = false;
        bool paused = false;

        const int KP_LINE = 5;
        const int KP_RANGEFINDER = 2;

        int TARGET_DISTANCE_PRE_PLATFORM = 5;
        int TARGET_DISTANCE_PLATFORM = 4;
        int TARGET_DISTANCE_ROOF_NO_PANEL;
        int TARGET_DISTANCE_ROOF_PICKUP;
        int TARGET_DISTANCE_ROOF_YES_PANEL;

        /**
         * @brief Follow the line until the chassis reachces an intersection.
         * Sets finished to true to indicate that the operation is complete.
         */
        void driveUntilIntersection();

        /**
         * @brief Follow the line until the chassis reaches the target distance.
         * Sets finished to true to indicate that the operation is complete.
         * 
         * @param targetDistance the target distance, in cm.
         */
        void driveUntilDistanceFrom(float targetDistance);

        /**
         * @brief Turn left and stop when the line is detected.
         * Sets finished to true to indicate that the operation is complete.
         */
        void turnLeftUntilLine();

        /**
         * @brief Turn right and stop when the line is detected.
         * Sets finished to true to indicate that the operation is complete.
         */
        void turnRightUntilLine();

        /**
         * @brief detects an intersection (cross of black tape).
         *
         * @return true if the intersection is reached.
         * @return false if the intersection is not reached.
         */
        bool detectIntersection();

        /**
         * @brief detects a line (black tape).
         * 
         * @return true if a line is detected.
         * @return false if a line is not detected.
         */
        bool detectLine();

        bool detectLineTurnLeft();

        bool detectLineTurnRight();

        /**
         * @brief detects whether the target distance from an object is reached.
         * 
         * @param targetDistance the target distance, in cm.
         * @param tolerance the tolerance, in cm.
         * @return true if the target distance is reached.
         * @return false if the target distance is not reached.
         */
        bool detectDistance(float targetDistance, float tolerance);

        /**
         * @brief Follow the line (black tape) with proportional control.
         */
        void lineFollow(float targetSpeed = TARGET_DRIVING_SPEED);

        /**
         * @brief Calculate the effort for line following with proportional control.
         * effort = kp * error
         * error = line effort left reading - line effort right reading
         * 
         * @return float the effort.
         */
        float lineEffort();

        /**
         * @brief Calculate the driving speed for distance proportional control.
         * effort = kp * error
         * error = distance - target distance
         * 
         * @param targetDistance the target distance.
         * @return float the target driving speed.
         */
        float getDrivingSpeed(float targetDistance);

        /**
         * @brief Converts centimeters of distance to encoder ticks.
         * 
         * @param cm the number of centimeters.
         * @return int the number of encoder ticks.
         */
        int cmToTicks(float cm);
};