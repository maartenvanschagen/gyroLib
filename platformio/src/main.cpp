#include "defines.h"
#include "gyro.h"
#include "accel.h"
#include "I2C.h"

#include <FreeRTOS.h>
#include <freertos/task.h>

using namespace std;

void loop();

TaskHandle_t xHandleLoop2 = NULL;
void loop2( void * );

//TODO: put all debugging in its own file

void setup() {
  I2C::init();

  accel::init();

  gyro::init();
  
  xTaskCreate(
    loop2,             /* Task function. */
    "gyroLoop",        /* String with name of task. */
    10000,             /* Stack size in bytes. */
    NULL,              /* Parameter passed as input of the task */
    5,                 /* Priority of the task. */
    &xHandleLoop2);             /* Task handle. */

  while(true){
    loop();
  }
}

void loop() {
  printf("gyro:\t%f\t%f\t%f", gyro::roll * 180/M_PI, gyro::yaw * 180/M_PI, gyro::pitch * 180/M_PI);
  //printf("%f\t%f\t%f", trimX, trimY, trimZ);
  ////printf("\t%i", gyro::timePast);
  //printf("gyro:\t%f\t%f\t%f\t%f", gyro::rotation.w, gyro::rotation.x, gyro::rotation.y, gyro::rotation.z);
  printf("\n");


  printf("accel:\t%f\t\t\t%f", accel::roll * 180/M_PI, accel::pitch * 180/M_PI);
  //printf("accel:\t%f\t%f\t%f\t%f", accel::rotation.w, accel::rotation.x, accel::rotation.y, accel::rotation.z);
  printf("\n");

  printf("-------\n");
  

  #ifdef DEBUG_GYROREADBUFFER
    printf("________\n");
    for(Quaternion r : gyro::rotationBuffer){
      double roll, yaw, pitch;
      r.getEuler(yaw, roll, pitch);
      printf("%f\t%f\t%f\n", roll * 180/M_PI, yaw * 180/M_PI, pitch * 180/M_PI);
    }
    printf("-------\n");
  #endif

  vTaskDelay(100 / portTICK_PERIOD_MS);
}



void loop2( void * parameter ){
  while(true){
    gyro::gyroLoop();
    accel::accelLoop();
  }
}

