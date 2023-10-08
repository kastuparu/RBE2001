#include <Arduino.h>
#include <LinearGripper.h>
#include <Romi32U4.h>
#include <servo32u4.h>

LinearGripper::LinearGripper()
{ 
}

void LinearGripper::setup()
{
    pinMode(LINEAR_POT_PIN, INPUT);
    servo.attach();
}

/**
 * @brief get the position of the gripper.
 * Returns a 10-bit ADC value from the linear potentiometer.
 * A value of 600 indicates that the gripper is closed.
 * A value of 940 indicates that the gripper is open.
 * 
 * @return int an ADC value from the linear potentiometer.
 */
int LinearGripper::getPosition() 
{
    return analogRead(LINEAR_POT_PIN);
}

/**
 * @brief determine if the gripper is open.
 * A position of 750 or less would indicate that the gripper is open.
 * A position of greater than 750 would indicate that the gripper is not open.
 * 
 * @return true if the gripper is open.
 * @return false if the gripper is not open.
 */
bool LinearGripper::isOpen()
{
    return getPosition() <= LINEAR_POT_JAW_DOWN;
}

/**
 * @brief determine if the gripper is closed.
 * A position of 950 or greater would indicate that the gripper is closed.
 * A value of less than 950 would indicate that the gripper is not closed.
 * 
 * @return true if the gripper is closed.
 * @return false if the gripper is not closed.
 */
bool LinearGripper::isClosed()
{
    return getPosition() >= LINEAR_POT_JAW_UP;
}

/**
 * @brief opens the gripper.
 * Turns on the servo if the gripper is not open, stops the servo if the gripper is open.
 * This method is non-blocking. Call this method in a loop to open the gripper.
 * Does not currently use proportional control.
 * 
 */
void LinearGripper::open() 
{
    if (!isOpen())
        servo.writeMicroseconds(SERVO_JAW_DOWN);
    else
        servo.writeMicroseconds(SERVO_STOP);
}

/**
 * @brief closes the gripper.
 * Turns on the servo if the gripper is not closed, stops the servo if the gripper is closed.
 * This method is non-blocking. Call this method in a loop to close the gripper.
 * Does not currently use proportional control.
 * 
 */
void LinearGripper::close()
{
    if (!isClosed())
        servo.writeMicroseconds(SERVO_JAW_UP);
    else
        servo.writeMicroseconds(SERVO_STOP);
}
