#include <unity.h>
#include <wrapper.h>

void test();
void test_delayAndMicros();

void test() {
  UNITY_BEGIN();
  wrapper::delayMillis(100);

  wrapper::init();

  RUN_TEST(test_delayAndMicros);

  UNITY_END();
}

void test_delayAndMicros(){
  unsigned int delayMillis = 100;
  uint64_t startMicros = wrapper::getMicros();
  wrapper::delayMillis(delayMillis);
  uint64_t realDelayMicros = wrapper::getMicros() - startMicros;
  TEST_ASSERT_UINT32_WITHIN((delayMillis * 1000) / 25, delayMillis * 1000, realDelayMicros);
}

#if defined(ARDUINO)
  #include "Arduino.h"
  void setup(){
    test();
  }
  void loop(){

  }
#elif defined(__AVR__)
  int main(void) {
    test();
  }
#elif defined(IDF_VER)
  extern "C" void app_main() {
    test();
  }
#endif