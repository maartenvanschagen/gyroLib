#if defined(IDF_VER) && !defined(ARDUINO)
  #include "wrapper.h"

  #include <stdint.h>
  #include <freertos/FreeRTOS.h>
  #include <freertos/task.h>

  namespace wrapper{
    void init(){
      
    }
    void delayMillis(const uint64_t milliseconds){
      vTaskDelay(pdMS_TO_TICKS(milliseconds));
    }
    uint64_t getMicros(){
      return esp_timer_get_time();
    }
}
#endif