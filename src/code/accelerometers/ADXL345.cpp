#include "accelerometers/ADXL345.h"

#include "defines.h"
#include "I2C.h"

void ADXL345::init(){
  //change settings
    //Set all three axes offsets to 0    because calibration is disabled
  I2C::writeRegister(ACCEL, OFSX, 0b00000000);
  I2C::writeRegister(ACCEL, OFSY, 0b00000000);
  I2C::writeRegister(ACCEL, OFSZ, 0b00000000);
    //Activity/inactivity
  I2C::writeRegister(ACCEL, THRESH_ACT,   4);           // 250mg    62.5 mg/LSB
  I2C::writeRegister(ACCEL, THRESH_INACT, 4);           // 250mg    62.5 mg/LSB
  I2C::writeRegister(ACCEL, TIME_INACT,   10);          // 10sec    1 sec/LSB
  I2C::writeRegister(ACCEL, ACT_INACT_CTL,0b11111111);  // enable all axis, and set to not fixed values, but reletive values
    //Freefall detection
  I2C::writeRegister(ACCEL, THRESH_FF,    6);           //312.5mg   62.5 mg/LSB
  I2C::writeRegister(ACCEL, TIME_FF,      6);           //312.5mg   62.5 mg/LSB
    //Power control
  I2C::writeRegister(ACCEL, POWER_CTL,    0b00101000);  //enabke activity and inactivity link, disable autosleep on activity, enable measuring mode, disable sleep
    //Interupts
  I2C::writeRegister(ACCEL, INT_ENABLE,   0b00000000);  //disable interrupts for now
  I2C::writeRegister(ACCEL, INT_MAP,      0b00000000);
    //Data settings
  I2C::writeRegister(ACCEL, BW_RATE,      0b00001101);  //disable low power mode and set datarate to 800 Hz
  I2C::writeRegister(ACCEL, DATA_FORMAT,  0b00001011);  //disable self test, set full res high 16g data range         //TODO: enable and setup self test
  I2C::writeRegister(ACCEL, FIFO_CTL,     0b00000000);  //disable FIFO

  setZeroReading(0, 0, -255);
}

bool ADXL345::isReady(){
  return ((I2C::getRegister(ACCEL, INT_SOURCE) & 0b10000000) == 0b10000000);  //check if data is ready
}

void ADXL345::read(short& accelX, short& accelY, short& accelZ){
  uint8_t accelData[6];
  I2C::getRegister(ACCEL, DATAX0, &accelData[0], 6);//put data in accelData with pointer address, increment is automatic
  accelX = accelData[1] << 8 | accelData[0]; //DATAX1 .. DATAX0
  accelY = accelData[3] << 8 | accelData[2]; //DATAY1 .. DATAY0
  accelZ = accelData[5] << 8 | accelData[4]; //DATAZ1 .. DATAZ0
}

void ADXL345::setOffset(short offsetX, short offsetY, short offsetZ){
  I2C::writeRegister(ACCEL, OFSX, (uint8_t)(offsetX/4));
  I2C::writeRegister(ACCEL, OFSY, (uint8_t)(offsetY/4));
  I2C::writeRegister(ACCEL, OFSZ, (uint8_t)(offsetZ/4));
}
