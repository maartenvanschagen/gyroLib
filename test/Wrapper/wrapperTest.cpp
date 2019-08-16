#include <unity.h>
#include <wrapper.h>

void test();
void test_delayAndMicros();

void test() {
  UNITY_BEGIN();

  RUN_TEST(test_delayAndMicros);

  UNITY_END();
}

void test_delayAndMicros(){
  unsigned int delayMillis = 100;
  unsigned int startMicros = wrapper::getMicros();
  wrapper::delayMillis(delayMillis);
  unsigned int realDelayMicros = wrapper::getMicros() - startMicros;
  TEST_ASSERT_INT_WITHIN(delayMillis / 25, delayMillis, realDelayMicros / 1000);
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