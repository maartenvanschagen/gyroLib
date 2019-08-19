#include <unity.h>
#include <wrapper.h>

void test();
void test_delayAndMicros();

void test() {
  UNITY_BEGIN();

  wrapper::init();

  RUN_TEST(test_delayAndMicros);

  UNITY_END();
}

void test_delayAndMicros(){
  unsigned int delayMillis = 10;
  uint64_t startMicros = wrapper::getMicros();
  wrapper::delayMillis(delayMillis);
  uint64_t realDelayMicros = wrapper::getMicros() - startMicros;
  TEST_ASSERT_UINT32_WITHIN((delayMillis * 1000) / 25, delayMillis * 1000, realDelayMicros);
}

#ifdef __AVR__
  int main(void) {
    test();
  }
#endif 
#ifdef ESP32
  extern "C" void app_main() {
    test();
  }
#endif