#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include "quaternion.h"
#include "euler.h"

//TODO: fully replace yaw pitch roll with Euler

class Accelerometer {
  public:    
    void step();
    void calibrate(int& rawX, int& rawY, int& rawZ, int samplesize = 1000, bool changeOffset = true);
    void calibrate(int samplesize = 1000, bool changeOffset = true);
    void calcRotation(int rawX, int rawY, int rawZ, double& pitch, double& roll);
    Euler calcRotation(int rawX, int rawY, int rawZ);
    void calcRotation(double& pitch, double& roll);
    Euler calcRotation();
    Quaternion calcRotation(int rawX, int rawY, int rawZ, double yaw = 0);
    Quaternion calcRotation(double yaw = 0);

    //setters and getters
    void setZeroReading(double zeroX, double zeroY, double zeroZ);
    void getZeroReading(double& zeroX, double& zeroY, double& zeroZ);
    Quaternion getQuaternion(double yaw = 0);
    void setQuaternion(Quaternion q);
    void setEuler(double pitch, double roll);
    void setEuler(Euler e);
    void getEuler(double& pitch, double& roll);
    void getEuler(double& yaw, double& pitch, double& roll);
    Euler getEuler();

    //not necessary to override
    virtual bool isReady();
    virtual void setOffset(double offsetX, double offsetY, double offsetZ);
    virtual void getOffset(double& offsetX, double& offsetY, double& offsetZ);
    //necessary to override
    virtual void init() = 0; //don't forget to set the default zero reading for the sensor
    virtual void read(int& rawX, int& rawY, int& rawZ) = 0;  //don't forget to include the offset when reading
  private:
    Euler rotation = Euler();
    double offsetX, offsetY, offsetZ;
    double zeroX = 0, zeroY = 0, zeroZ = -255;
};

#endif
