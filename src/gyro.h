#ifndef GYRO_H
#define GYRO_H

#include "Quaternion.h"

class Gyro {
  public:
    short gyroX, gyroY, gyroZ;  //2 bytes (just like the register)
    double trimX, trimY, trimZ;
    double roll, yaw, pitch;
    Quaternion rotation = Quaternion(1, 0, 0, 0);

    void calcRot(double& rotX, double& rotY, double& rotZ, int trimX, int trimY, int trimZ, long timePast);
    void setTrim(double& TrimXI, double& TrimYI, double& TrimZI, int samplesize);
    void step();
    virtual void init();
    virtual bool isReady();
    virtual void read(short& gyroX, short& gyroY, short& gyroZ);

    #ifdef DEBUG_GYROREADBUFFER
      std::vector<Quaternion> rotationBuffer;
    #endif
  private:
    long lastMicros;
    int timePast = 0;
};

#endif