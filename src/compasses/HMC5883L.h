#ifndef HMC5883L_H
#define HMC5883L_H

#include "compass.h"

#define COMPASS         0x1E

#define CONFIG_A       0
#define CONFIG_B       1
#define MODE           2
#define STATUS         9
#define ID_A          10
#define ID_B          11
#define ID_C          12


#define OUT_X_H        3
#define OUT_X_L        4
#define OUT_Y_H        5
#define OUT_Y_L        6
#define OUT_Z_H        7
#define OUT_Z_L        8

class HMC5883L: public Compass {
  public:
    void init() override;
    Vector3i read() override;
    Vector3d calcGaus(Vector3i raw) override;

    bool isReady() override;  // TODO: This function doesn't wait for new data, it only tells us if we are able to read, fix this
};

#endif