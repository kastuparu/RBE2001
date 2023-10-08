#include <Arduino.h>
#include <BottomOutGripper.h>
#include <Romi32U4.h>
#include <servo32u4.h>

BottomOutGripper::BottomOutGripper()
{
}

void BottomOutGripper::setup()
{
    pinMode(SERVO_FEEDBACK_PIN, INPUT);
    servo.attach();
}

/**
 * @brief Gets the position of the servo from the feedback pin.
 * A position of 285 or greater would indicate that the gripper is open.
 * A position of 144 or less would indicate that the gripper is closed.
 * 
 * @return int the position of the servo.
 */
int BottomOutGripper::getPosition()
{
    return analogRead(SERVO_FEEDBACK_PIN);
}

/**
 * @brief determine if the gripper is open.
 * A position of 285 or greater would indicate that the gripper is open.
 * A position of less than 285 would indicate that the gripper is not open.
 * 
 * @return true if the gripper is open.
 * @return false if the gripper is not open.
 */
bool BottomOutGripper::isOpen()
{
    return getPosition() >= SERVO_FEEDBACK_OPEN;
}

/**
 * @brief determine if the gripper is closed.
 * A position of 144 or less would indicate that the gripper is closed.
 * A position of greater than 144 would indicate that the gripper is not closed.
 * 
 * @return true if the gripper is closed.
 * @return false if the gripper is not closed.
 */
bool BottomOutGripper::isClosed()
{
    return getPosition() <= SERVO_FEEDBACK_CLOSED;
}

/**
 * @brief opens the gripper.
 * Sets the gripper to the open position.
 */
void BottomOutGripper::open()
{
    if (!isOpen())
        servo.writeMicroseconds(SERVO_CONTROL_OPEN);
}

/**
 * @brief closes the gripper.
 * Sets the gripper to the closed position.
 */
void BottomOutGripper::close()
{
    if (!isClosed())
        servo.writeMicroseconds(SERVO_CONTROL_CLOSED);
}