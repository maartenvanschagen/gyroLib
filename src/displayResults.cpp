#include "defines.h"
#include "gyros/L3G4200D.h"
#include "accelerometers/ADXL345.h"
#include "I2C.h"
#include "wrapper.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

L3G4200D gyro = L3G4200D();
ADXL345 accel = ADXL345();

void loop();

TaskHandle_t xHandleLoop2 = NULL;
void loop2( void * );

extern "C" void app_main() {
  I2C::init();
  gyro.setYPR("XYZ");
  gyro.setReverse(false, false, false);
  gyro.init();
  accel.init();

  wrapper::delayMillis(100);
  gyro.calibrate(1000);
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
  double gyroYaw, gyroPitch, gyroRoll;
  gyro.getEuler(gyroYaw, gyroPitch, gyroRoll);

  Quaternion accelRotation = accel.getQuaternion(gyroYaw);
  double yaw, pitch, roll;
  accelRotation.getEuler(yaw, pitch, roll);


  printf("\tyaw\t\tpitch\t\troll");     //print euler angle info
  printf("\n");

  printf("gyro:\t%f\t%f\t%f", gyroYaw * 180/M_PI, gyroPitch * 180/M_PI, gyroRoll * 180/M_PI);                         //print euler angles
  //printf("gyro:\t%f\t%f\t%f\t%f", gyro.rotation.w, gyro.rotation.x, gyro.rotation.y, gyro.rotation.z);                //print quaternion
  //printf("accel:\t%i\t%i\t%i", gyro.rawX, gyro.rawY, gyro.rawZ);                                                      //print raw data
  printf("\n");

  //printf("accel:\t%f\t%f\t%f", yaw * 180/M_PI, pitch * 180/M_PI, roll * 180/M_PI);                                    //print converted to quaternion and back to Euler
  printf("accel:\t\t\t%f\t%f", accel.pitch * 180/M_PI, accel.roll * 180/M_PI);                                        //print euler angles
  //printf("accel:\t%f\t%f\t%f\t%f", accelRotation.w, accelRotation.x, accelRotation.y, accelRotation.z);               //print quaternion
  //printf("accel:\t%i\t%i\t%i", accel.rawX, accel.rawY, accel.rawZ);                                                   //print raw data
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

  wrapper::delayMillis(150);
}



void loop2( void * parameter ){
  while(true){
    gyro.step();
    accel.step();
  }
}

