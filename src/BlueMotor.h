#pragma once

class BlueMotor
{
public:
    BlueMotor();
    void setEffort(int effort, int degree, bool clockwise);
    void moveTo(long position, int degree, bool clockwise);
    long getPosition();
    void reset();
    void setup();

private:
    static void isrA();
    static void isrB();
    const int tolerance = 3;
    const int PWMOutPin = 11;
    const int AIN2 = 4;
    const int AIN1 = 13;
    static const int ENCA = 0;
    static const int ENCB = 1;
};