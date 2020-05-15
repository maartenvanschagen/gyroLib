#if defined(ARDUINO)
  #include "Arduino.h"
  #include "wrapper.h"

  namespace wrapper{
    void delayMillis(const uint64_t milliseconds){
      delay(milliseconds); 
    }

    void init(){
        //Arduino handles all the inits
    }

    uint64_t getMicros(){
      return micros();
    }
  }

#endif