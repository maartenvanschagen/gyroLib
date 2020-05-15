#include "gyros/L3G4200D.h"

#include "quaternion.h"
#include "I2CWrapper.h"
#include "gyro.h"
#define M_PI (double)(3.14159265358979323846)  //replacement for #include <math.h>

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
}

Vector3i L3G4200D::read(){ // ~280 microseconds
  uint8_t gyroData[6];
  I2C::getRegister(GYRO, OUT_X_L | (0b10000000), gyroData, 6);//put data in gyroData with pointer address  -  increment the address after read register (with 0b10000000)
  Vector3i raw;
  raw.x = (int16_t)(gyroData[1] << 8 | gyroData[0]); //OUT_X_H .. OUT_X_L     //shorts are to handle negatives (last bit)
  raw.y = (int16_t)(gyroData[3] << 8 | gyroData[2]); //OUT_Y_H .. OUT_Y_L
  raw.z = (int16_t)(gyroData[5] << 8 | gyroData[4]); //OUT_Z_H .. OUT_Z_L
  return raw;
}

Vector3d L3G4200D::calcRotation(Vector3i raw, Vector3d offset, long timePast){ // ~15 microseconds
  Vector3d rotation;
  rotation.x = -(raw.x - offset.x) * 0.0175 * (timePast / 1000000.0) * (M_PI/180.0);    // (gyro trimmed) * sensitivity op 500 dps * timePast(s) * degToRad
  rotation.y = -(raw.y - offset.y) * 0.0175 * (timePast / 1000000.0) * (M_PI/180.0);    // all static numbers gets calculated by compiler
  rotation.z =  (raw.z - offset.z) * 0.0175 * (timePast / 1000000.0) * (M_PI/180.0);
  return transformRotation(rotation);
}

bool L3G4200D::isReady(){
  return ((I2C::getRegister(GYRO, STATUS_REG) & 0b00001000) == 0b00001000); // check if data is ready
                                                                            // TODO: use STATUS_REG 7 to check if data has been overwritten and if so freq must be too high so set or lower
}