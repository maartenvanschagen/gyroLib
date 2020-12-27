#include "boards/GY80.h"
#include "I2CWrapper.h"

GY80_GYRO gyro = L3G4200D();
GY80_ACCELEROMETER accel = ADXL345();

void setup() {
  Serial.begin(115200);
  
  I2C::init();
  accel.setAxesSwitched('X', 'Y', 'Z');
  accel.setAxesReversed(true, true, false);
  accel.init();
  gyro.setAxesSwitched('X', 'Y', 'Z');
  gyro.setAxesReversed(true, true, false);
  gyro.setAccelerometer(&accel); //Use a accelerometer for less accuracy loss over time
  gyro.init();

  delay(100);
  gyro.calibrate(1000);
  accel.calibrate(1000);
}

void loop() {
  accel.step();
  gyro.step();

  Quaternion gyroQuaternion = gyro.getQuaternion();
  Euler gyroEuler = gyro.getEuler();

  Quaternion accelQuaternion = accel.getQuaternion(gyroEuler.yaw);
  Euler accelQuaternionEuler = accelQuaternion.getEuler();
  Euler accelEuler = accel.getEuler();


  Serial.print("\tyaw\tpitch\troll\n");     //print euler angle info
  //Serial.print("\tw\t\tx\t\ty\t\tz\n");         //print quaternion info

  Serial.print("gyro:\t" + String(gyroEuler.yaw * 180/M_PI) + "\t" + String(gyroEuler.pitch * 180/M_PI) + "\t" + String(gyroEuler.roll * 180/M_PI) + "\n"); //print euler angles
  //printf("gyro:\t%f\t%f\t%f\t%f\n", gyroQuaternion.w, gyroQuaternion.x, gyroQuaternion.y, gyroQuaternion.z);                                          //print quaternion
  ////printf("gyro:\t%i\t%i\t%i\n", gyro.rawX, gyro.rawY, gyro.rawZ);                                                                                     //print raw data

  Serial.print("accel:\t" + String(accelQuaternionEuler.yaw * 180/M_PI) + "\t" + String(accelQuaternionEuler.pitch * 180/M_PI) + "\t" + String(accelQuaternionEuler.roll * 180/M_PI) + "\n");   //print converted to quaternion and back to Euler
  //printf("accel:\t\t\t%f\t%f\n", accelEuler.pitch * 180/M_PI, accelEuler.roll * 180/M_PI);                                                            //print euler angles
  //printf("accel:\t%f\t%f\t%f\t%f\n", accelQuaternion.w, accelQuaternion.x, accelQuaternion.y, accelQuaternion.z);                                     //print quaternion
  ////printf("accel:\t%i\t%i\t%i\n", accel.rawX, accel.rawY, accel.rawZ);                                                                                 //print raw data

  Serial.print("-------\n");
  delay(10);
}
