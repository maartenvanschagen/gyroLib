#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include "quaternion.h"
#include "euler.h"
#include "vector3.h"

//TODO: support different callibration schemes, single-point calibration works quite poorly

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
    Vector3d transformRotation(Vector3d raw);

    //getters and setters
    Vector3d getZeroReading();
    void setZeroReading(double zeroX, double zeroY, double zeroZ);
    void setZeroReading(Vector3d zero);
    void setAxesSwitched(char xAxis, char yAxis, char zAxis);
    void setAxesReversed(bool x, bool y, bool z);
    void setOffset(double offsetX, double offsetY, double offsetZ);
    Euler getEuler();
    void setEuler(Euler e);
    Quaternion getQuaternion();
    Quaternion getQuaternion(double yaw);
    void setQuaternion(Quaternion q);
    void setYaw(double yaw);
    Vector3d applyOffset(Vector3i raw);
    Vector3d readAndApplyOffset();
    Vector3d readAndApplyOffset(Vector3d offset);

    //not necessary to override
    virtual bool isReady();
    virtual Vector3d getOffset();
    virtual void setOffset(Vector3d offset);
    virtual Vector3d applyOffset(Vector3i raw, Vector3d offset);
    //necessary to override
    virtual void init() = 0; //don't forget to set the default zero reading for the sensor
    virtual Vector3i read() = 0;
  private:
    Euler rotation = Euler();
    Vector3d offset;
    Vector3d zero = Vector3d(0, 0, -255);
    char axesSwitched[3] = {'X', 'Y', 'Z'};
    bool axesReversed[3] = {false, false, false};
};

#endif
