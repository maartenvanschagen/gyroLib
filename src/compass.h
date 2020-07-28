#ifndef COMPASS_H
#define COMPASS_H

#include "vector3.h"
#include "quaternion.h"

struct compassCalibrate{
  Vector3d offset = Vector3d(0, 0, 0);
  Vector3d scale = Vector3d(0, 0, 0);

  compassCalibrate(Vector3d offset, Vector3d scale){
    this->offset = offset;
    this->scale = scale;
  }

  compassCalibrate(double offsetX, double offsetY, double offsetZ, double scaleX, double scaleY, double scaleZ){
    this->offset = Vector3d(offsetX, offsetY, offsetZ);
    this->scale = Vector3d(scaleX, scaleY, scaleZ);
  }

  compassCalibrate(){}
};

class Compass {
  public:
    compassCalibrate calibrate(int samplesize = 1000, bool changeValues = true);  // TODO: fix function  // TODO: add calibrate in seconds
    Vector3d calcField(Vector3d value, Vector3d offset, Vector3d scale);
    Vector3d calcField(Vector3d value, compassCalibrate calibrate);
    Vector3d calcField(Vector3d value);
    Vector3d getField();
    Vector3d getCorrectedField(Quaternion pitchRoll); //Subtract pitch and roll
    Vector3d correctField(Vector3d field, Quaternion pitchRoll); //Subtract pitch and roll
    double getYaw(Vector3d field); // TODO: add support for 3d
    double getYaw();
    Quaternion getFieldDirection(Vector3d field, Quaternion accelerometer); //Needs quaternion with yaw = 0
    Quaternion getFieldDirection(Quaternion accelerometer); //Needs quaternion with yaw = 0

    //not necessary to override
    virtual bool isReady();
    //necessary to override
    virtual void init() = 0;
    virtual Vector3i read() = 0;
    virtual Vector3d calcGaus(Vector3i raw) = 0;  // Scale our values to Gaus , No calibration

  private:
    compassCalibrate calibration;
};

#endif