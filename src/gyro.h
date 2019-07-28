#ifndef GYRO_H
#define GYRO_H

#include "Quaternion.h"

//TODO: fix public private protected
class Gyro {
  public:
    short gyroX, gyroY, gyroZ;  //2 bytes (just like the register)
    double trimX, trimY, trimZ;
    double roll, yaw, pitch;
    Quaternion rotation = Quaternion(1, 0, 0, 0);

    #ifdef DEBUG_GYROREADBUFFER
      std::vector<Quaternion> rotationBuffer;
    #endif

    void calcRotation(double& rotX, double& rotY, double& rotZ, int trimX, int trimY, int trimZ, long timePast);
    void setTrim(double& TrimXI, double& TrimYI, double& TrimZI, int samplesize = 1000, bool changeOffset = true);
    void setTrim(int samplesize = 1000, bool changeOffset = true);
    void step();

    //not necessary to override
    virtual bool isReady();
    //necessary to override
    virtual void init() = 0;
    virtual void read(short& gyroX, short& gyroY, short& gyroZ) = 0;
  private:
    long lastMicros;
    int timePast = 0;
};

#endif