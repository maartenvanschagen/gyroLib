#include "accelerometers/MPU6050.h"

#include "I2CWrapper.h"

void MPU6050_Accelerometer::init(){
    I2C::writeRegister(ADDRESS, SMPRT_DIV,   7);  //Set samplerate to 1 kHz, this sets it to the same speed as the accelerometer, there is not really a good reason for this number
    I2C::writeRegister(ADDRESS, CONFIG,      0b00000000);  //Set the Low Pass Filter to bandwidth: accelerometer 260 Hz, gyro 265 Hz
    I2C::writeRegister(ADDRESS, ACCEL_CONFIG, 0b00000000);  //Set range to +- 2 g = 62.5 mg/LSB
    I2C::writeRegister(ADDRESS, FIFO_EN,     0b00000000);  //Disable FIFO
    //I2C::writeRegister(ADDRESS, INT_PIN_CFG, 0b00000000);  //Don't care about interrupt pin
    I2C::writeRegister(ADDRESS, INT_ENABLE,  0b00000001);  //Enable data ready interrupt
    //I2C::writeRegister(ADDRESS, USER_CTRL,   0b00000000);  //No controll needed
    I2C::writeRegister(ADDRESS, PWR_MGMT_1,  0b00000001);  //Clear power management register to start measurements
    //I2C::writeRegister(ADDRESS, PWR_MGMT_2,  0b00000001);  //Not required

    setZeroReading(0, 0, 16384); //Configure the reading we should get when just measuring gravity
}

Vector3i MPU6050_Accelerometer::read(){
  uint8_t gyroData[6];
  I2C::getRegister(ADDRESS, ACCEL_XOUT_H | (0b10000000), gyroData, 6);//put data in gyroData with pointer address  -  increment the address after read register (with 0b10000000)
  Vector3i raw;
  raw.x = (int16_t)(gyroData[0] << 8 | gyroData[1]); //ACCEL_XOUT_H .. ACCEL_XOUT_L
  raw.y = (int16_t)(gyroData[2] << 8 | gyroData[3]); //ACCEL_YOUT_H .. ACCEL_YOUT_L
  raw.z = (int16_t)(gyroData[4] << 8 | gyroData[5]); //ACCEL_ZOUT_H .. ACCEL_ZOUT_L

  return raw;
}

bool MPU6050_Accelerometer::isReady(){
  return ((I2C::getRegister(ADDRESS, INT_STATUS) & 0b00000001) == 0b00000001); // check if data is ready
}