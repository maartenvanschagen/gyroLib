#ifdef ESP32
  #include "wrapper.h"

  #include <stdint.h>
  #include <freertos/FreeRTOS.h>
  #include <freertos/task.h>

  namespace wrapper{
    void delayMillis(const uint64_t milliseconds){
      vTaskDelay(milliseconds / portTICK_PERIOD_MS);
    }
    uint64_t getMicros(){
      return (uint64_t)esp_timer_get_time();
    }
}
#endif