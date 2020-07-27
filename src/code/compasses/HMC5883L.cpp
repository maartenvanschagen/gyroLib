#include "compasses/HMC5883L.h"
#include "I2CWrapper.h"

void HMC5883L::init(){
    I2C::writeRegister(COMPASS, CONFIG_A, 0b00011000); // 75 Hz output  ,  average of 1 measurement
    I2C::writeRegister(COMPASS, CONFIG_B, 0b00100000); // 0.92 mG/LSb
    I2C::writeRegister(COMPASS, MODE    , 0b00000000);
}

Vector3i HMC5883L::read(){
    uint8_t compassData[6];
    I2C::getRegister(COMPASS, OUT_X_H, compassData, 6);
    Vector3i raw;
    raw.x = (int16_t)(compassData[0] << 8 | compassData[1]); //OUT_X_H .. OUT_X_L
    raw.y = (int16_t)(compassData[2] << 8 | compassData[3]); //OUT_Y_H .. OUT_Y_L
    raw.z = (int16_t)(compassData[4] << 8 | compassData[5]); //OUT_Z_H .. OUT_Z_L
    return raw;
}

Vector3d HMC5883L::calcGaus(Vector3i raw){
    Vector3d field;
    field.x = raw.x * 0.92;
    field.y = raw.y * 0.92;
    field.z = raw.z * 0.92;
    return field;
}

bool HMC5883L::isReady(){  // TODO: This function doesn't wait for new data, it only tells us if we are able to read, fix this
    return I2C::getRegister(COMPASS, STATUS) & 0b00000001;
}