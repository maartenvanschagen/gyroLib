#ifndef GYRO_H
#define GYRO_H

#include "quaternion.h"
#include "euler.h"
#include "accelerometer.h"
#include <string>

//TODO: fully replace yaw pitch roll with Euler

class Gyro {
  public:
    void step();
    void calibrate(double& offsetX, double& offsetY, double& offsetZ, int samplesize = 1000, bool changeOffset = true);
    void calibrate(int samplesize = 1000, bool changeOffset = true);
    void calcRotation(int rawX, int rawY, int rawZ, double& yaw, double& pitch, double& roll, long timePast);
    Euler calcRotation(int rawX, int rawY, int rawZ, long timePast);
    void calcRotation(double& yaw, double& pitch, double& roll, long timePast);
    Euler calcRotation(long timePast);
    void transformRotation(double x, double y, double z, double& yaw, double& pitch, double& roll);
    void nudgeRotationTowards(Quaternion q);
    void nudgeRotationTowards(Accelerometer& a);

    //setters and getters
    void setAxesSwitched(std::string setAxesSwitched);
    void setAxesSwitched(char xAxis, char yAxis, char zAxis);
    void setAxesReversed(bool x, bool y, bool z);
    void setOffset(double offsetX, double offsetY, double offsetZ);
    void getOffset(double& offsetX, double& offsetY, double& offsetZ);
    void getEuler(double& yaw, double& pitch, double& roll);
    Euler getEuler();
    void setEuler(double yaw, double pitch, double roll);
    void setEuler(Euler e);
    Quaternion getQuaternion();
    void setQuaternion(Quaternion q);
    void setAccelerometer(Accelerometer* a);
    Accelerometer* getAccelerometer();

    //not necessary to override
    virtual bool isReady();
    //necessary to override
    virtual void init() = 0;
    virtual void read(int& rawX, int& rawY, int& rawZ) = 0; //is int for compatibility
    virtual void calcRotation(int rawX, int rawY, int rawZ, double& rotationX, double& rotationY, double& rotationZ, double offsetX, double offsetY, double offsetZ, long timePast) = 0;
  private:
    long lastMicros;
    int timePast = 0;
    Quaternion rotation = Quaternion(1, 0, 0, 0);
    char axesSwitched[3] = {'X', 'Y', 'Z'};
    bool axesReversed[3] = {false, false, false};
    double offsetX, offsetY, offsetZ;
    Accelerometer* accelerometer = NULL;
};

#endif