#include "gyros/MPU6050.h"

#include "I2CWrapper.h"
#include "gyro.h"
#define M_PI (double)(3.14159265358979323846)  //replacement for #include <math.h>

void MPU6050_Gyro::init(){
    I2C::writeRegister(ADDRESS, SMPRT_DIV,   7);  //Set samplerate to 1 kHz, this sets it to the same speed as the accelerometer, there is not really a good reason for this number
    I2C::writeRegister(ADDRESS, CONFIG,      0b00000000);  //Set the Low Pass Filter to bandwidth: accelerometer 260 Hz, gyro 265 Hz
    I2C::writeRegister(ADDRESS, GYRO_CONFIG, 0b00001000);  //Set range to 500 degrees per second
    I2C::writeRegister(ADDRESS, FIFO_EN,     0b00000000);  //Disable FIFO
    //I2C::writeRegister(ADDRESS, INT_PIN_CFG, 0b00000000);  //Don't care about interrupt pin
    I2C::writeRegister(ADDRESS, INT_ENABLE,  0b00000001);  //Enable data ready interrupt
    //I2C::writeRegister(ADDRESS, USER_CTRL,   0b00000000);  //No controll needed
    I2C::writeRegister(ADDRESS, PWR_MGMT_1,  0b00000001);  //Clear power management register to start measurements
    //I2C::writeRegister(ADDRESS, PWR_MGMT_2,  0b00000001);  //Not required
}

Vector3i MPU6050_Gyro::read(){
  uint8_t gyroData[6];
  I2C::getRegister(ADDRESS, GYRO_XOUT_H | (0b10000000), gyroData, 6);//put data in gyroData with pointer address  -  increment the address after read register (with 0b10000000)
  Vector3i raw;
  raw.x = (int16_t)(gyroData[0] << 8 | gyroData[1]); //GYRO_XOUT_H .. GYRO_XOUT_L
  raw.y = (int16_t)(gyroData[2] << 8 | gyroData[3]); //GYRO_YOUT_H .. GYRO_YOUT_L
  raw.z = (int16_t)(gyroData[4] << 8 | gyroData[5]); //GYRO_ZOUT_H .. GYRO_ZOUT_L

  return raw;
}

Vector3d MPU6050_Gyro::calcRotation(Vector3i raw, Vector3d offset, long timePast){
  Vector3d rotation;
  rotation.x =  (raw.x - offset.x) * (500.0/32767) * (timePast / 1000000.0) * (M_PI/180.0);    // (gyro trimmed) * sensitivity at 500 dps * timePast(s) * degToRad
  rotation.y =  (raw.y - offset.y) * (500.0/32767) * (timePast / 1000000.0) * (M_PI/180.0);    // all static numbers gets calculated by compiler
  rotation.z =  (raw.z - offset.z) * (500.0/32767) * (timePast / 1000000.0) * (M_PI/180.0);    // 500.0/32767 is more accurate then datasheet number
  return transformRotation(rotation);
}

bool MPU6050_Gyro::isReady(){
  return ((I2C::getRegister(ADDRESS, INT_STATUS) & 0b00000001) == 0b00000001); // check if data is ready
}