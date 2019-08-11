#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include "quaternion.h"
#include "euler.h"
#include "vector3.h"

//TODO: implement rotation and translation of sensor

class Accelerometer {
  public:    
    void step(double yaw = 0);
    Vector3d calibrate(int samplesize = 1000, bool changeOffset = true);
    Euler calcEuler(Vector3i raw);
    Euler calcEuler();
    Euler calcEuler(Vector3i raw, double yaw);
    Euler calcEuler(double yaw);
    Quaternion calcQuaternion(Vector3i raw, double yaw = 0);
    Quaternion calcQuaternion(double yaw = 0);

    //setters and getters
    void setZeroReading(double zeroX, double zeroY, double zeroZ);
    void setZeroReading(Vector3d zero);
    Vector3d getZeroReading();
    Quaternion getQuaternion();
    Quaternion getQuaternion(double yaw);
    void setQuaternion(Quaternion q);
    void setEuler(Euler e);
    Euler getEuler();
    void setYaw(double yaw);
    void setOffset(double offsetX, double offsetY, double offsetZ);

    //not necessary to override
    virtual bool isReady();
    virtual void setOffset(Vector3d offset);
    virtual Vector3d getOffset();
    //necessary to override
    virtual void init() = 0; //don't forget to set the default zero reading for the sensor
    virtual Vector3i read() = 0;  //don't forget to include the offset when reading
  private:
    Euler rotation = Euler();
    Vector3d offset;
    Vector3d zero = Vector3d(0, 0, -255);
};

#endif
