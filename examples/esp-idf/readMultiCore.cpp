#include "I2CWrapper.h"
#include "gyros/L3G4200D.h"
#include "accelerometers/ADXL345.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define M_PI (double)(3.14159265358979323846)  //replacement for #include <math.h>

L3G4200D gyro = L3G4200D();
ADXL345 accel = ADXL345();

void loop();

TaskHandle_t xHandleLoop2 = NULL;
void loop2( void * );

extern "C" void app_main() {
  I2C::init();
  wrapper::init();
  accel.setAxesSwitched('X', 'Y', 'Z');
  accel.setAxesReversed(true, true, false);
  accel.init();
  gyro.setAxesSwitched('X', 'Y', 'Z');
  gyro.setAxesReversed(true, true, false);
  gyro.setAccelerometer(&accel);
  gyro.init();

  vTaskDelay(100 / portTICK_PERIOD_MS);
  gyro.calibrate(1000);
  accel.calibrate(1000);

  xTaskCreate(
    loop2,             // Task function.
    "gyroLoop",        // String with name of task.
    10000,             // Stack size in bytes. 
    NULL,              // Parameter passed as input of the task
    5,                 // Priority of the task.
    &xHandleLoop2);             // Task handle.

  while(true){
    loop();
  }
}

void loop() {
  Quaternion gyroQuaternion = gyro.getQuaternion();
  Euler gyroEuler = gyro.getEuler();

  Quaternion accelQuaternion = accel.getQuaternion(gyroEuler.yaw);
  Euler accelQuaternionEuler = accelQuaternion.getEuler();
  Euler accelEuler = accel.getEuler();


  printf("\tyaw\t\tpitch\t\troll\n");     //print euler angle info
  printf("\tw\t\tx\t\ty\t\tz\n");         //print quaternion info

  printf("gyro:\t%f\t%f\t%f\n", gyroEuler.yaw * 180/M_PI, gyroEuler.pitch * 180/M_PI, gyroEuler.roll * 180/M_PI);                         //print euler angles
  printf("gyro:\t%f\t%f\t%f\t%f\n", gyroQuaternion.w, gyroQuaternion.x, gyroQuaternion.y, gyroQuaternion.z);                //print quaternion
  //printf("gyro:\t%i\t%i\t%i\n", gyro.rawX, gyro.rawY, gyro.rawZ);                                                      //print raw data

  printf("accel:\t%f\t%f\t%f\n", accelQuaternionEuler.yaw * 180/M_PI, accelQuaternionEuler.pitch * 180/M_PI, accelQuaternionEuler.roll * 180/M_PI);                                    //print converted to quaternion and back to Euler
  printf("accel:\t\t\t%f\t%f\n", accelEuler.pitch * 180/M_PI, accelEuler.roll * 180/M_PI);                                        //print euler angles
  printf("accel:\t%f\t%f\t%f\t%f\n", accelQuaternion.w, accelQuaternion.x, accelQuaternion.y, accelQuaternion.z);               //print quaternion
  //printf("accel:\t%i\t%i\t%i\n", accel.rawX, accel.rawY, accel.rawZ);                                                   //print raw data

  printf("-------\n");

  vTaskDelay(1000 / portTICK_PERIOD_MS);
}



void loop2( void * parameter ){
  while(true){
    accel.step();
    gyro.step();
  }
}