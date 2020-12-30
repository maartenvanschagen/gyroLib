#ifndef MPU6050_ACCEL_H
#define MPU6050_ACCEL_H

#include "accelerometer.h"

//MPU6050
#define ADDRESS         0b1101000 //0b1101000 if AD0 is low, 0b1101001 if AD0 is high

//not all registers have defines
#define SMPRT_DIV     25
#define CONFIG        26
#define GYRO_CONFIG   27
#define ACCEL_CONFIG  28
#define FIFO_EN       35
#define INT_PIN_CFG   55
#define INT_ENABLE    56
#define INT_STATUS    58

#define ACCEL_XOUT_H  59
#define ACCEL_XOUT_L  60
#define ACCEL_YOUT_H  61
#define ACCEL_YOUT_L  62
#define ACCEL_ZOUT_H  63
#define ACCEL_ZOUT_L  64

#define SIGNAL_PATH_RESET 104
#define USER_CTRL     106
#define PWR_MGMT_1    107
#define PWR_MGMT_2    108
#define WHO_AM_I      117

class MPU6050_Accelerometer: public Accelerometer {
  public:
    void init() override;
    Vector3i read() override;

    bool isReady() override;
};

#endif
