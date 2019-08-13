#ifdef __AVR__
  #include "I2CWrapper.h"
  #include <stdint.h>

  namespace I2C{
    void init(){ 
      
    }

    uint8_t getRegister(uint8_t device, uint8_t reg){
      return 0x00;
    }

    bool getRegister(uint8_t device, uint8_t reg, uint8_t * receiveData, int amount){
      return false;
    }

    void writeRegister(uint8_t device, uint8_t reg, uint8_t data){
      
    }

    void writeRegister(uint8_t device, uint8_t reg, uint8_t dataBits, uint8_t data){
      
    }
  }
#endif
