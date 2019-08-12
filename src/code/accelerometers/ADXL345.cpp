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

Vector3i ADXL345::read(){
  Vector3i raw;
  uint8_t accelData[6];
  I2C::getRegister(ACCEL, DATAX0, &accelData[0], 6);//put data in accelData with pointer address, increment is automatic
  raw.x = (short)(accelData[1] << 8 | accelData[0]); //DATAX1 .. DATAX0    //shorts are to handle negatives (last bit)
  raw.y = (short)(accelData[3] << 8 | accelData[2]); //DATAY1 .. DATAY0
  raw.z = (short)(accelData[5] << 8 | accelData[4]); //DATAZ1 .. DATAZ0
  return raw;
}

bool ADXL345::isReady(){
  return ((I2C::getRegister(ACCEL, INT_SOURCE) & 0b10000000) == 0b10000000);  //check if data is ready
}

Vector3d ADXL345::getOffset(){
  Vector3d offset;
  offset.x = (double)I2C::getRegister(ACCEL, OFSX)*4;
  offset.y = (double)I2C::getRegister(ACCEL, OFSY)*4;
  offset.z = (double)I2C::getRegister(ACCEL, OFSZ)*4;
  return offset;
}

void ADXL345::setOffset(Vector3d offset){
  I2C::writeRegister(ACCEL, OFSX, (uint8_t)(offset.x/4));
  I2C::writeRegister(ACCEL, OFSY, (uint8_t)(offset.y/4));
  I2C::writeRegister(ACCEL, OFSZ, (uint8_t)(offset.z/4));
}