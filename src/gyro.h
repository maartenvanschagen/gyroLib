#ifndef GYRO_H
#define GYRO_H

#include "quaternion.h"
#include "euler.h"
#include "vector3.h"
#include "accelerometer.h"
#include <string>

class Gyro {
  public:
    void step();
    Vector3d calibrate(int samplesize = 1000, bool changeOffset = true);
    Vector3d calcRotation(Vector3i raw, long timePast);
    Vector3d calcRotation(long timePast);
    Vector3d transformRotation(Vector3d raw);
    void nudgeRotationTowards(Quaternion q);
    void nudgeRotationTowards(Accelerometer& a);

    //setters and getters
    void setAxesSwitched(std::string setAxesSwitched);
    void setAxesSwitched(char xAxis, char yAxis, char zAxis);
    void setAxesReversed(bool x, bool y, bool z);
    void setOffset(Vector3d offset);
    void setOffset(double offsetX, double offsetY, double offsetZ);
    Vector3d getOffset();
    Euler getEuler();
    void setEuler(Euler e);
    Quaternion getQuaternion();
    void setQuaternion(Quaternion q);
    void setAccelerometer(Accelerometer* a);
    Accelerometer* getAccelerometer();

    //not necessary to override
    virtual bool isReady();
    //necessary to override
    virtual void init() = 0;
    virtual Vector3i read() = 0; //is int for compatibility
    virtual Vector3d calcRotation(Vector3i raw, Vector3d offset, long timePast) = 0;
  private:
    long lastMicros;
    int timePast = 0;
    Quaternion rotation = Quaternion(1, 0, 0, 0);
    char axesSwitched[3] = {'X', 'Y', 'Z'};
    bool axesReversed[3] = {false, false, false};
    Vector3d offset;
    Accelerometer* accelerometer = NULL;
};

#endif