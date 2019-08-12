#ifndef L3G4200D_H
#define L3G4200D_H

#include "gyro.h"

//L3G4200D
#define GYRO            0x69
#define OUT_X_L       0x28
#define OUT_X_H       0x29
#define OUT_Y_L       0x2a
#define OUT_Y_H       0x2b
#define OUT_Z_L       0x2c
#define OUT_Z_H       0x2d

#define WHOAMI        0x0f
#define CTRL_REG1     0x20
#define CTRL_REG2     0x21
#define CTRL_REG3     0x22
#define CTRL_REG4     0x23
#define CTRL_REG5     0x24
#define REFERENCE     0x25
#define INT1_CFG      0x30
#define INT1_DURATION 0x38
#define STATUS_REG    0x27

#define INT1_THS_XH   0x32
#define INT1_THS_XL   0x33
#define INT1_THS_YH   0x34
#define INT1_THS_YL   0x35
#define INT1_THS_ZH   0x36
#define INT1_THS_ZL   0x37

class L3G4200D: public Gyro {
  public:
    void init() override;
    Vector3i read() override;
    virtual Vector3d calcRotation(Vector3i raw, Vector3d offset, long timePast) override;

    bool isReady() override;
};

#endif
