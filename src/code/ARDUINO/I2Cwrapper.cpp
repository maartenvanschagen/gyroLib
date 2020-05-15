#if defined(ARDUINO)
  #include "I2CWrapper.h"
  #include "Wire.h"

  namespace I2C{
    void init(){ 
      Wire.begin();
      Wire.setClock(400000);
    }

    uint8_t getRegister(uint8_t device, uint8_t reg){
      uint8_t receiveData;
      getRegister(device, reg, &receiveData, 1);
      return receiveData;
    }

    bool getRegister(uint8_t device, uint8_t reg, uint8_t * receiveData, int amount){
      Wire.beginTransmission(device);
      Wire.write(reg);
      Wire.endTransmission();
      Wire.requestFrom(device, (uint8_t)amount);

      if (amount <= Wire.available()) {
        for(int i = 0; i < amount; i++){
          receiveData[i] = Wire.read();
        }
      }
      return false;
    }

    void writeRegister(uint8_t device, uint8_t reg, uint8_t data){
      Wire.beginTransmission(device);
      Wire.write(reg);
      Wire.write(data);
      Wire.endTransmission();
    }

    void writeRegister(uint8_t device, uint8_t reg, uint8_t dataBits, uint8_t data){
      uint8_t writeData = getRegister(device, reg);                                  //first get the register
      uint8_t all1 = dataBits & data;                                                //find all the spots to write a 1 to in the register
      uint8_t all0 = dataBits & ~data;                                               //find all the spots to write a 0 to in the register 
      writeData |= all1;                                                             //write all the 1's
      writeData &= ~all0;                                                            //write all the 0's
      
      writeRegister(device, reg, writeData);
    }
  }
#endif