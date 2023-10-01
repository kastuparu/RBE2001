#include <Arduino.h>
#include <BlueMotor.h>
#include <Romi32U4.h>
#include <Timer.h>

Arduino: initialize

long oldValue = 0;
long newValue;
long volatile count = 0;
unsigned time = 0;
long finalCount = 0;
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

void BlueMotor::setEffort(int effort, int degree, bool clockwise)
{
    // deadband with Encoder A
    OCR1C = constrain(effort, 0, 400);

    while(clockwise) {
        digitalWrite(AIN1, HIGH);
        digitalWrite(AIN2, LOW);
        
        driveTimer.reset(540*degree);
        while(effort < 401) {
            for(ini i = 0; i < 401; i++) {
                Serial.println("Effort: " + effort);
                Serial.println("Encoder Count: " + analogRead(ENCA));
                effort = effort + i;

                if(driverTimer.isExpired()) {
                    finalCount = i;
                    stop();
                    break;
                }
            }
            break;
        }
        clockwise = false;       
    }

    reset();
}

void BlueMotor::setEffortWithoutDB(int effort, int degree, bool clockwise)
{
    // deadband with Encoder A
    clockwise = true;
    OCR1C = constrain(effort, 0, 400);
    int finalCount = 0;
    
    while(clockwise) {
        digitalWrite(AIN1, HIGH);
        digitalWrite(AIN2, LOW);
        
        driveTimer.reset(540*degree);
        while(effort < 401) {
            for(ini i = 0; i < 401; i++) {
                Serial.println("Effort: " + effort);
                Serial.println("Encoder Count: " + analogRead(ENCA));
                effort = effort + i;

                if(driverTimer.isExpired()) {
                    stop();
                    break;
                }
            }
            break;
        }
        clockwise = false;
    }

    // deadband with Encoder B
    OCR1C = constrain(effort, -400, 0);
    while(!clockwise) {
        digitalWrite(AIN1, LOW);
        digitalWrite(AIN2, HIGH);
        
        driveTimer.reset(540*degree);
        while(effort > -401) {
            for(ini i = 0; i > -401; i--) {
                Serial.println("Effort: " + effort);
                Serial.println("Encoder Count: " + analogRead(ENCB));
                effort = effort + i;

                if(driverTimer.isExpired()) {
                    finalCount = 401 + i;
                    stop();
                    break;
                }
            }
            break;
        }
        clockwise = true;
    }

    reset();
}

void stop()
{
    digitalWrite(PWMA, LOW);
}

void sweep(int target)
{
    for (int i = 0; i < target; i++) {
        digitalWrite(AIN1, LOW);
        digitalWrite(AIN2, HIGH);
        OCR1C = i;
        delay(10);
    }
}

void BlueMotor::moveTo(long target, int degree, bool clockwise)  //Move to this encoder position within the specified
{                                    //tolerance in the header file using proportional control
                                     //then stop
    float kp = 1;
    float effort = kp * (target - tolerance);

    

    if(abs((getPosition() - target) < tolerance) || abs((getPosition() - target) > tolerance)) {
        switch(clockwise)
        {
        case true:
          setEffort(effort, degree, clockwise);
        break;

        case false:
          setEffortWithoutDB(effort, degree, clockwise);
        break;

        default:
          stop();
        }
        
    } else {
        sweep(finalCount);
        finalCount = 0;
        stop();
        delay(10000);
    }
    setEffort(0);
}

/*
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
*/

