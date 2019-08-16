#include <unity.h>
#include <I2CWrapper.h>
#include <wrapper.h>

//This test needs a L3G4200D gyroscope to be connected

void test();
void test_getRegister();
void test_writeRegister();
void test_writeRegister_single();
void test_getRegister_multiple();

void test() {
  UNITY_BEGIN();
  I2C::init();

  RUN_TEST(test_getRegister);
  RUN_TEST(test_writeRegister);
  RUN_TEST(test_writeRegister_single);
  RUN_TEST(test_getRegister_multiple);
  
  UNITY_END();
}

void test_getRegister(){
  TEST_ASSERT_BITS(0xFF, 0b11010011, I2C::getRegister(0x69, 0x0F));
}

void test_writeRegister(){
  I2C::writeRegister(0x69, 0x20, 0b00001111);
  TEST_ASSERT_BITS(0xFF, 0b00001111, I2C::getRegister(0x69, 0x20));
}

void test_writeRegister_single(){
  I2C::writeRegister(0x69, 0x20, 0b00001111);
  I2C::writeRegister(0x69, 0x20, 0b01010101, 0b00110011);
  TEST_ASSERT_BITS(0xFF, 0b00011011, I2C::getRegister(0x69, 0x20));
}

void test_getRegister_multiple(){
  I2C::writeRegister(0x69, 0x20, 0b00001111);
  I2C::writeRegister(0x69, 0x21, 0b11001100);
  static uint8_t list[2];
  I2C::getRegister(0x69, 0x20 | 0b10000000, list, 2);
  TEST_ASSERT_BITS(0xFFFF, 0b00001111 << 8 | 0b11001100, list[0] << 8 | list[1]);
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