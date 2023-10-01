#include <Arduino.h>
#include <Servo.h>
#include <Romi32U4.h>
#include <Timer.h>
#include <servo32U4.h>
Romi32U4ButtonB buttonB;

Arduino: initialize

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

Servo::Servo()
{
}

void Servo::setup()
{
    Serial(9600);
    jawServo.attach();
}

void Servo::setGripper()
{
    // gripper
    jawServo.writeMircoSeconds(servoJawDown);
    jawServo.writeMircoSeconds(servoStop);
    jawServo.writeMircoSeconds(servoJawUp);
    jawServo.writeMircoSeconds(servoStop);
}

void Servo::setGripperDB()
{
    // gripper
    jawServo.writeMircoSeconds(servoJawDown);  // gripper jaw down

    println("");
    while(linearPotVoltageADC > jawOpenPotVoltageADC)
    {
        linearPotVoltageADC = analog(linearPotPin);
        if(printTimer.isExpired()) {
            println("linearPotVoltageADC: ");
            println(linearPotVoltageADC);
        }
    }

    jawServo.writeMircoSeconds(servoStop);  // gripper jaw stops

    linearPotVoltageADC = analog(linearPotPin);
    println("Final voltage before delay: ");
    println(linearPotVoltageADC);
    delay(5000);

    linearPotVoltageADC = analog(linearPotPin);
    println("Final voltage after delay: ");
    println(linearPotVoltageADC);
    delay(5000);

    jawServo.writeMircoSeconds(servoJawUp);  // gripper jaw rises

    println("");
    while(linearPotVoltageADC < jawOpenPotVoltageADC)
    {
        linearPotVoltageADC = analog(linearPotPin);
        if(printTimer.isExpired()) {
            println("linearPotVoltageADC: ");
            println(linearPotVoltageADC);
        }
    }

    jawServo.writeMircoSeconds(servoStop);  // gripper jaw stops
    
    linearPotVoltageADC = analog(linearPotPin);
    println("Final voltage before delay: ");
    println(linearPotVoltageADC);
    delay(5000);

    linearPotVoltageADC = analog(linearPotPin);
    println("Final voltage after delay: ");
    println(linearPotVoltageADC);
    delay(5000);
}

