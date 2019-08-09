#ifndef GYRO_H
#define GYRO_H

#include "quaternion.h"
#include "euler.h"
#include "vector3.h"
#include "accelerometer.h"
#include <string>

//TODO: fully replace yaw pitch roll with Euler and fully integrate Vector3


class Gyro {
  public:
    void step();
    Vector3<double> calibrate(int samplesize = 1000, bool changeOffset = true);
    Vector3<double> calcRotation(Vector3<int> raw, long timePast);
    Vector3<double> calcRotation(long timePast);
    void transformRotation(Vector3<double> raw, double& yaw, double& pitch, double& roll);
    void nudgeRotationTowards(Quaternion q);
    void nudgeRotationTowards(Accelerometer& a);

    //setters and getters
    void setAxesSwitched(std::string setAxesSwitched);
    void setAxesSwitched(char xAxis, char yAxis, char zAxis);
    void setAxesReversed(bool x, bool y, bool z);
    void setOffset(Vector3<double> offset);
    Vector3<double> getOffset();
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
    virtual Vector3<int> read() = 0; //is int for compatibility
    virtual void calcRotation(int rawX, int rawY, int rawZ, double& rotationX, double& rotationY, double& rotationZ, double offsetX, double offsetY, double offsetZ, long timePast) = 0;
  private:
    long lastMicros;
    int timePast = 0;
    Quaternion rotation = Quaternion(1, 0, 0, 0);
    char axesSwitched[3] = {'X', 'Y', 'Z'};
    bool axesReversed[3] = {false, false, false};
    Vector3<double> offset;
    Accelerometer* accelerometer = NULL;
};

#endif