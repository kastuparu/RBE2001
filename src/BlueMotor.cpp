#include <Arduino.h>
#include <BlueMotor.h>
#include <Romi32U4.h>

long oldValue = 0;
long newValue;
long volatile count = 0;
unsigned time = 0;

BlueMotor::BlueMotor()
{
}

void BlueMotor::setup()
{
    pinMode(PWMOutPin, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(AIN1, OUTPUT);
    pinMode(ENCA, INPUT);
    pinMode(ENCB, INPUT);
    TCCR1A = 0xA8; //0b10101000; //gcl: added OCR1C for adding a third PWM on pin 11
    TCCR1B = 0x11; //0b00010001;
    ICR1 = 400;
    OCR1C = 0;

    attachInterrupt(digitalPinToInterrupt(ENCA), isrA, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCB), isrB, CHANGE);
    reset();
}

/**
 * @brief Get the encoder count.
 * 
 * @return long the encoder count.
 */
long BlueMotor::getPosition()
{
    long tempCount = 0;
    noInterrupts();
    tempCount = count;
    interrupts();
    return tempCount;
}

/**
 * @brief Reset the encoder count.
 * 
 */
void BlueMotor::reset()
{
    noInterrupts();
    count = 0;
    interrupts();
}

/**
 * @brief Interrupt service routine (ISR) for encoder A.
 * 
 */
void BlueMotor::isrA()
{
    if (digitalRead(ENCB) == digitalRead(ENCA)) {
        count++;
    }
    else {
        count--;
    }
}

/**
 * @brief Interrupt service routine (ISR) for encoder B.
 * 
 */
void BlueMotor::isrB()
{
    if (digitalRead(ENCA) == digitalRead(ENCB)) {
        count--;
    }
    else {
        count++;
    }
}

/**
 * @brief Set the motor effort (without dead band correction).
 * Set the effort to 400 to lift the fourbar with maximum speed.
 * Set the effort to values between 0 and 400 to lift the fourbar.
 * Set the effort to 0 to stop the motor.
 * Set the effort to values between 0 and -400 to lower the fourbar.
 * Set the effort to -400 to lower the fourbar with maximum speed.
 * 
 * @param effort the effort, between -400 and 400.
 */
void BlueMotor::setEffort(int effort)
{
    if (effort < 0)
        setEffort(-effort, true);
    else
        setEffort(effort, false);
}

/**
 * @brief Set the motor effort (without dead band correction).
 * Used by the public setEffort() method to control direction.
 * 
 * @param effort the effort, between 0 and 400.
 * @param clockwise true for negative effort or lowering the fourbar, false for positive effort or lifting the fourbar.
 */
void BlueMotor::setEffort(int effort, bool clockwise)
{
    if (clockwise)
    {
        digitalWrite(AIN1, HIGH);
        digitalWrite(AIN2, LOW);
    }
    else
    {
        digitalWrite(AIN1, LOW);
        digitalWrite(AIN2, HIGH);
    }
    OCR1C = constrain(effort, 0, 400);
}

/**
 * @brief Set the motor effort with dead band correction.
 * Removing dead band is useful for proportional control.
 * The effort where the dead band ends for lifting the fourbar (CCW) is 229.
 * The effort where the dead band ends for lowering the fourbar (CW) is -184.
 * 
 * Set the effort to 400 to lift the fourbar with maximum speed.
 * Set the effort to values between 0 and 400 to lift the fourbar.
 * Set the effort to 0 to stop the motor.
 * Set the effort to values between 0 and -400 to lower the fourbar.
 * Set the effort to -400 to lower the fourbar with maximum speed.
 * 
 * @param effort the effort, between 0 and 400.
 * @return int 
 */
int BlueMotor::setEffortWithoutDB(int effort)
{
    int effortAdjusted;
    if (effort < 0)
    {
        effortAdjusted = DEAD_BAND_CW + effort * 1.0 / EFFORT_MAX * (EFFORT_MAX + DEAD_BAND_CW);
        setEffort(-effortAdjusted, true);
    }
    else
    {
        effortAdjusted = DEAD_BAND_CCW + effort * 1.0 / EFFORT_MAX * (EFFORT_MAX - DEAD_BAND_CCW);
        setEffort(effortAdjusted, false);
    }
    return effortAdjusted;
}

/**
 * @brief Move the motor to a target position using proportional control.
 * This method is non-blocking. Call this method in a loop to move to the target position.
 * There is a tolerance of 3 encoder counts.
 * 
 * @param target the target encoder count.
 * @return true if the motor is finished moving.
 * @return false if the motor is not finished moving.
 */
bool BlueMotor::moveTo(long target)
{                                    
    float kp = 0.4;
    long currentPos = getPosition();

    if (abs(currentPos - target) > tolerance) {
        float effort = kp * (target - currentPos);
        setEffortWithoutDB(effort);
        return false;
    }
    else {
        setEffort(0);
        return true;
    }
}
