#include "defines.h"
#include "gyros/L3G4200D.h"
#include "accelerometers/ADXL345.h"
#include "I2C.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

using namespace std;

L3G4200D gyro = L3G4200D();
ADXL345 accel = ADXL345();

void loop();

TaskHandle_t xHandleLoop2 = NULL;
void loop2( void * );

extern "C" void app_main() {
  I2C::init();
  gyro.init();
  accel.init();

  vTaskDelay(100 / portTICK_PERIOD_MS);
  gyro.setTrim(1000);
  accel.calibrate(1000);

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
  printf("gyro:\t%f\t%f\t%f", gyro.roll * 180/M_PI, gyro.yaw * 180/M_PI, gyro.pitch * 180/M_PI);                      //print euler angles
  //printf("gyro:\t%f\t%f\t%f\t%f", gyro.rotation.w, gyro.rotation.x, gyro.rotation.y, gyro.rotation.z);                //print quaternion
  //printf("accel:\t%i\t%i\t%i", gyro.gyroX, gyro.gyroY, gyro.gyroZ);                                                   //print raw data
  printf("\n");


  printf("accel:\t%f\t\t\t%f", accel.roll * 180/M_PI, accel.pitch * 180/M_PI);                                        //print euler angles
  //printf("accel:\t%f\t%f\t%f\t%f", accel.rotation.w, accel.rotation.x, accel.rotation.y, accel.rotation.z);           //print quaternion
  //printf("accel:\t%i\t%i\t%i", accel.accelX, accel.accelY, accel.accelZ);                                             //print raw data
  printf("\n");

  printf("-------\n");
  

  #ifdef DEBUG_GYROREADBUFFER
    printf("________\n");
    for(Quaternion r : gyro.rotationBuffer){
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
    gyro.step();
    accel.step();
  }
}

