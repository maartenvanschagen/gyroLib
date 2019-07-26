#include "gyros/L3G4200D.h"

#include "defines.h"
#include "Quaternion.h"
#include "I2C.h"
#include "gyro.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void L3G4200D::init(){                                          //TODO: make settings dependant on defines
  //change settings
  I2C::writeRegister(GYRO, CTRL_REG2, 0b00100001); //set high pass filter freq to 8Hz
  I2C::writeRegister(GYRO, CTRL_REG3, 0b00000000); //interrups default
  I2C::writeRegister(GYRO, CTRL_REG4, 0b00010000); //500 dps
  I2C::writeRegister(GYRO, REFERENCE, 0b00000000);

  I2C::writeRegister(GYRO, INT1_THS_XH, 0b00000000); //sets thresholds to 100
  I2C::writeRegister(GYRO, INT1_THS_XL, 0b01100100);
  I2C::writeRegister(GYRO, INT1_THS_YH, 0b00000000);
  I2C::writeRegister(GYRO, INT1_THS_YL, 0b01100100);
  I2C::writeRegister(GYRO, INT1_THS_ZH, 0b00000000);
  I2C::writeRegister(GYRO, INT1_THS_ZL, 0b01100100);

  I2C::writeRegister(GYRO, INT1_DURATION, 0b00000000); //disable wait on default
  I2C::writeRegister(GYRO, INT1_CFG , 0b00000000); //interrups default
  I2C::writeRegister(GYRO, CTRL_REG5, 0b00011000); //enable high pass filter (for interrupt?)
  I2C::writeRegister(GYRO, CTRL_REG1, 0b11101111); //turn on 800Hz, 50Hz cut-off, X, Y, Z enabled


  //set trim
  vTaskDelay(10 / portTICK_PERIOD_MS);  //TODO:: check if this is necessary
  Gyro::setTrim(Gyro::trimX, Gyro::trimY, Gyro::trimZ, 1000); //sample size 1000 (takes 5/4 seconds)
}

void L3G4200D::read(short& gyroX, short& gyroY, short& gyroZ){ // ~280 microseconds
  uint8_t gyroData[6];
  I2C::getRegister(GYRO, OUT_X_L | (0b10000000), &gyroData[0], 6);//put data in gyroData with pointer address  -  incement the address after read register (with 0b10000000)
  gyroX = gyroData[1] << 8 | gyroData[0]; //OUT_X_H .. OUT_X_L
  gyroY = gyroData[3] << 8 | gyroData[2]; //OUT_Y_H .. OUT_Y_L
  gyroZ = gyroData[5] << 8 | gyroData[4]; //OUT_Z_H .. OUT_Z_L
}

bool L3G4200D::isReady(){
  return ((I2C::getRegister(GYRO, STATUS_REG) & 0b00001000) == 0b00001000); // check if data is ready
                                                                            // TODO: use STATUS_REG 7 to check if data has been overwritten and if so freq must be too high so set or lower
}