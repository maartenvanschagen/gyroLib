#ifndef WRAPPER_H
#define WRAPPER_H

#include <stdint.h>

namespace wrapper{
  void init();
  void delayMillis(const uint64_t milliseconds);
  uint64_t getMicros();
}

#if defined(__AVR__) && !defined(ARDUINO)
  extern "C" void __cxa_pure_virtual(); //Supress errors on pure virtual functions. This library uses these functions.
#endif

#endif
