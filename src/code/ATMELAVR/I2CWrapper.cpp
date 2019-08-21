#ifdef __AVR__
  #include "I2CWrapper.h"
  #include <stdint.h>
  #include <avr/io.h>

  #define I2C_FREQ 400000 //doens't support different prescales

  //this wrapper is written synchronous
  //TODO: add async support with a define

  namespace I2C{
    void start();
    void writeByte(uint8_t);
    uint8_t readByteAck();
    uint8_t readByteNack();
    bool ready();
    void stop();

    void init(){ 
      TWBR = (((F_CPU / I2C_FREQ) - 16) / 2); //set clock frequency
    }

    uint8_t getRegister(uint8_t device, uint8_t reg){
      uint8_t receiveData;
      getRegister(device, reg, &receiveData, 1);
      return receiveData;
    }

    bool getRegister(uint8_t device, uint8_t reg, uint8_t * receiveData, int amount){
      start();
      writeByte(device << 1 | 0);
      writeByte(reg);
      start();
      writeByte(device << 1 | 1);
      for(int i = 0; i < amount - 1; i++){
        receiveData[i] = readByteAck();
      }
      receiveData[amount - 1] = readByteNack();
      stop();

      return false;
    }

    void writeRegister(uint8_t device, uint8_t reg, uint8_t data){
      start();
      writeByte(device << 1 | 0);
      writeByte(reg);
      writeByte(data);
      stop();
    }

    void writeRegister(uint8_t device, uint8_t reg, uint8_t dataBits, uint8_t data){
      uint8_t writeData = getRegister(device, reg);                                  //first get the register
      uint8_t all1 = dataBits & data;                                                //find all the spots to write a 1 to in the register
      uint8_t all0 = dataBits & ~data;                                               //find all the spots to write a 0 to in the register 
      writeData |= all1;                                                             //write all the 1's
      writeData &= ~all0;                                                            //write all the 0's
      
      writeRegister(device, reg, writeData);   
    }

    //TODO: check ACK/NACK

    void start(){
      TWCR = (1<<TWINT) | (1<<TWSTA) | (0<<TWSTO) | (1<<TWEN);   //send start signal
      while(!ready());
    }

    void writeByte(uint8_t data){
      TWDR = data; //set data to send
      TWCR = (1<<TWINT) | (0<<TWSTA) | (0<<TWSTO) | (1<<TWEN); //load control signals
      while(!ready());
    }

    uint8_t readByteAck(){
      TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
      while(!ready());
      return TWDR;
    }

    uint8_t readByteNack(){
      TWCR = (1<<TWINT)|(1<<TWEN);
      while(!ready());
      return TWDR;
    }

    bool ready(){
      return TWCR & (1<<TWINT);
    }

    void stop(){
      TWCR = (1<<TWINT) | (0<<TWSTA) | (1<<TWSTO) | (1<<TWEN);
    }
  }
#endif
