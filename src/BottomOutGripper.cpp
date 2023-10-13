#include <Arduino.h>
#include <BottomOutGripper.h>
#include <Romi32U4.h>
#include <servo32u4.h>

BottomOutGripper::BottomOutGripper()
{
}

void BottomOutGripper::setup()
{
    servo.attach();
    close();
}

/**
 * @brief opens the gripper.
 * Sets the gripper to the open position.
 */
void BottomOutGripper::open()
{
    servo.writeMicroseconds(SERVO_OPEN);
}

/**
 * @brief closes the gripper.
 * Sets the gripper to the closed position.
 */
void BottomOutGripper::close()
{
    servo.writeMicroseconds(SERVO_CLOSED);
}