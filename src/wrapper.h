#ifndef WRAPPER_H
#define WRAPPER_H

#include <stdint.h>

namespace wrapper{
  void delayMillis(const uint64_t milliseconds);
  uint64_t getMicros();
}

#endif