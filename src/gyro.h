#ifndef GYRO_H
#define GYRO_H

#include "Quaternion.h"

//TODO: fix public private protected
class Gyro {
  public:
    double offsetX, offsetY, offsetZ;
    Quaternion rotation = Quaternion(1, 0, 0, 0); //TODO: fix y and z switched

    #ifdef DEBUG_GYROREADBUFFER
      std::vector<Quaternion> rotationBuffer;
    #endif

    void calcRotation(int rawX, int rawY, int rawZ, double& rotationX, double& rotationY, double& rotationZ, long timePast);
    void calcRotation(double& rotationX, double& rotationY, double& rotationZ, long timePast);
    void calibrate(double& offsetX, double& offsetY, double& offsetZ, int samplesize = 1000, bool changeOffset = true);
    void calibrate(int samplesize = 1000, bool changeOffset = true);
    void step();
    void getEuler(double& yaw, double& pitch, double& roll);

    //not necessary to override
    virtual bool isReady();
    //necessary to override
    virtual void init() = 0;
    virtual void read(int& rawX, int& rawY, int& rawZ) = 0; //is int for compatibility
    virtual void calcRotation(int rawX, int rawY, int rawZ, double& rotationX, double& rotationY, double& rotationZ, double offsetX, double offsetY, double offsetZ, long timePast) = 0;
  private:
    long lastMicros;
    int timePast = 0;
};

#endif