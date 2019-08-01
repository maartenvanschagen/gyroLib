#include <stdint.h>

#ifndef WRAPPER_H
#define WRAPPER_H

namespace wrapper{
  void delayMillis(const uint64_t milliseconds);
  uint64_t getMicros();
}

#endif