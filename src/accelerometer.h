#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include "Quaternion.h"

//TODO: fix public private protected
class Accelerometer {
  public:
    double roll, pitch;
    short accelX, accelY, accelZ;
    short offsetX, offsetY, offsetZ;
    short zeroX = 0, zeroY = 0, zeroZ = 255;
    Quaternion rotation = Quaternion(1, 0, 0, 0);
    
    void step(double yaw = 0);
    void read();
    void calibrate(short& accelX, short& accelY, short& accelZ, int samplesize = 1000, bool changeOffset = true);
    void calibrate(int samplesize = 1000, bool changeOffset = true);
    void calcRotation(double& roll, double& pitch);
    Quaternion calcRotation(double yaw = 0);
    void setZeroReading(short accelX, short accelY, short accelZ);

    //not necessary to override
    virtual bool isReady();
    virtual void setOffset(short offsetX, short offsetY, short offsetZ);
    //necessary to override
    virtual void init() = 0; //don't forget to set the default zero reading for the sensor
    virtual void read(short& accelX, short& accelY, short& accelZ) = 0;  //don't forget to include the offset when reading
  private:
};

#endif
