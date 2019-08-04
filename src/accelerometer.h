#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include "Quaternion.h"

//TODO: fix public private protected
class Accelerometer {
  public:
    double pitch, roll;
    double offsetX, offsetY, offsetZ;
    double zeroX = 0, zeroY = 0, zeroZ = -255;
    
    void step();
    void calibrate(int& rawX, int& rawY, int& rawZ, int samplesize = 1000, bool changeOffset = true);
    void calibrate(int samplesize = 1000, bool changeOffset = true);
    void calcRotation(int rawX, int rawY, int rawZ, double& pitch, double& roll);
    void calcRotation(double& pitch, double& roll);
    Quaternion calcRotation(int rawX, int rawY, int rawZ, double yaw = 0);
    Quaternion calcRotation(double yaw = 0);
    void setZeroReading(double zeroX, double zeroY, double zeroZ);
    Quaternion getQuaternion(double yaw = 0);

    //not necessary to override
    virtual bool isReady();
    virtual void setOffset(double offsetX, double offsetY, double offsetZ);
    //necessary to override
    virtual void init() = 0; //don't forget to set the default zero reading for the sensor
    virtual void read(int& rawX, int& rawY, int& rawZ) = 0;  //don't forget to include the offset when reading
  private:
};

#endif
