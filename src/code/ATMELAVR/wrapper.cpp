#ifdef __AVR__
  #include "wrapper.h"
  #include <stdint.h>

  namespace wrapper{
    void delayMillis(const uint64_t milliseconds){
      
    }
    uint64_t getMicros(){
      return 0;
    }
}

extern "C" void __cxa_pure_virtual()
{
  //TODO: handle undefined pure function here
}

#endif