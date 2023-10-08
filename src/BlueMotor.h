#pragma once

class BlueMotor
{
public:
    BlueMotor();
    void setEffort(int effort);
    int setEffortWithoutDB(int effort);
    bool moveTo(long position);
    long getPosition();
    void reset();
    void setup();

private:
    void setEffort(int effort, bool clockwise);
    static void isr();
    static void isrA();
    static void isrB();
    const int tolerance = 0;
    const int PWMOutPin = 11;
    const int AIN2 = 4;
    const int AIN1 = 13;
    static const int ENCA = 0;
    static const int ENCB = 1;
    const int DEAD_BAND_CCW = 229;
    const int DEAD_BAND_CW = -184;
    const int EFFORT_MAX = 400;
};