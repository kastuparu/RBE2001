#include <Arduino.h>
#include <BlueMotor.h>
#include <Romi32U4.h>
#include <Timer.h>
#include <servo32U4.h>
Romi32U4ButtonB buttonB;

Arduino: initialize

long oldValue = 0;
long newValue;
long volatile count = 0;
unsigned time = 0;

int linearPotPin = 18;
int servoStop = 1480;
int sevroJawDown = 1300;
int servoJawUp = 1700;
int printDelay = 500;
int linearPotVoltageADC = 500;
int jawOpenPotVoltageADC = 600;
int jawClosedPotVoltageADC = 940;

Servo32U4 jawServo;
Timer printTimer(delay);
Timer forearmTimer(delay);

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

    attachInterrupt(digitalPinToInterrupt(ENCA), isrA(), CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCB), isrA(), CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCA), isrB(), CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCB), isrB(), CHANGE);

    jawServo.attach();
    reset();
}

long BlueMotor::getPosition()
{
    long tempCount = 0;
    noInterrupts();
    tempCount = count;
    interrupts();
    return tempCount;
}

void BlueMotor::reset()
{
    noInterrupts();
    count = 0;
    interrupts();
}


void BlueMotor::isrA()
{
    if (digitalRead(1)) {
        count++;
    } else {
        count--;
    }
}

void BlueMotor::isrB()
{
    if (digitalRead(1)) {
        count--;
    } else {
        count++;
    }
}

void BlueMotor::setEffortDB(int effort, int degree, bool clockwise)
{
    // forearm
    OCR1C = constrain(effort, 0, 400);
    while(clockwise) {
        digitalWrite(AIN1, HIGH);
        digitalWrite(AIN2, LOW);
        driveTimer.reset(540*degree);
        if(driverTimer.isExpired()) {
            stop();
            clockwise = false;
        }
    }
    
    // gripper
    jawServo.writeMircoSeconds(servoJawDown);

    while(linearPotVoltageADC > jawOpenPotVoltageADC)
    {
        linearPotVoltageADC = analog(linearPotPin);
        if(printTimer.isExpired()) {
            println("linearPotVoltageADC: ");
            println(linearPotVoltageADC);
        }
    }

    jawServo.writeMircoSeconds(servoStop);
    linearPotVoltageADC = analog(linearPotPin);
    println("Final voltage before delay: ");
    println(linearPotVoltageADC);
    delay(5000);

    linearPotVoltageADC = analog(linearPotPin);
    println("Final voltage after delay: ");
    println(linearPotVoltageADC);
    delay(5000);
    

    jawServo.writeMircoSeconds(servoJawUp);

    println("");
    while(linearPotVoltageADC < jawOpenPotVoltageADC)
    {
        linearPotVoltageADC = analog(linearPotPin);
        if(printTimer.isExpired()) {
            println("linearPotVoltageADC: ");
            println(linearPotVoltageADC);
        }
    }

    jawServo.writeMircoSeconds(servoStop);
    linearPotVoltageADC = analog(linearPotPin);
    println("Final voltage before delay: ");
    println(linearPotVoltageADC);
    delay(5000);

    linearPotVoltageADC = analog(linearPotPin);
    println("Final voltage after delay: ");
    println(linearPotVoltageADC);
    delay(5000);
}

void BlueMotor::setEffortWithoutDB(int effort, int degree, bool clockwise)
{
    // forearm
    OCR1C = constrain(effort, 0, 400);
    while(clockwise) {
        digitalWrite(AIN1, HIGH);
        digitalWrite(AIN2, LOW);
        driveTimer.reset(540*degree);
        if(driverTimer.isExpired()) {
            stop();
            clockwise = false;
        }
    }
    
    // gripper
    jawServo.writeMircoSeconds(servoJawDown);
    jawServo.writeMircoSeconds(servoStop);
    jawServo.writeMircoSeconds(servoJawUp);
    jawServo.writeMircoSeconds(servoStop);
}

void BlueMotor::setEffort(int effort)
{
    if (effort < 0)
    {
        setEffort(-effort, true);
    }
    else
    {
        setEffort(effort, false);
    }
}

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

void stop()
{
    digitalWrite(PWMA, LOW);
}

void sweep(int position)
{
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    for (int i = 0; i < position; i++)
    {
        OCR1C = i;
        delay(10);
    }
}

void BlueMotor::moveTo(long target, int degree, bool clockwise)  //Move to this encoder position within the specified
{                                    //tolerance in the header file using proportional control
                                     //then stop
    float kp = 1;
    float effort = kp * (target - tolerance);

    if(true) {
        if(abs((getPosition() - target) < tolerance) || abs((getPosition() - target) > tolerance)) {
            setEffortDB(effort, degree, clockwise);
            sweep(target);
            setEffortWithoutDB(effort, degree, clockwise);
            sweep(target);
        } else {
            stop();
            delay(10000);
            break;
        }
    } else {
        if((abs(getPosition() - target) < tolerance) || (abs(getPosition() - target) > tolerance)) {
            setEffortDB(effort, degree, clockwise);
            sweep(target);
            setEffortWithoutDB(effort, degree, clockwise);
            sweep(target);
        } else {
            stop();
            delay(10000);
            break;
        }
    }
    setEffort(0);
}

void BlueMotor::moveTo(long target)  //Move to this encoder position within the specified
{                                    //tolerance in the header file using proportional control
                                     //then stop
    float kp = 1;
    long currentPos = getPosition();

    if (abs(currentPos - target) > tolerance) {
        float effort = kp * (target - tolerance);
        setEffort(effort);
    }
    else {
        setEffort(0);
    }
}
