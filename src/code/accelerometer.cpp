#include "accelerometer.h"

#include "defines.h"
#include "Quaternion.h"
#include <cmath>

//TODO: enable calibration, maybe something better suited then a single-point calibration scheme
//TODO: add gyro offset angle vs thrust angle (manuel or automatic)


double range(double val){
  return fmod((val + M_PI), (2 * M_PI)) - M_PI;
}

void Accelerometer::read(){
  read(accelX, accelY, accelZ);
}

void Accelerometer::calibrate(short& accelX, short& accelY, short& accelZ, int samplesize, bool changeOffset){
  long accelXTotal = 0;
  long accelYTotal = 0;
  long accelZTotal = 0;
  short accelXdata, accelYdata, accelZdata;

  //Set all three axes offsets to 0
  setOffset(0, 0, 0);

  for(int i = 0; i < samplesize; i++){
    while(!isReady()){}
    read(accelXdata, accelYdata, accelZdata);
    accelXTotal += accelXdata;
    accelYTotal += accelYdata;
    accelZTotal += accelZdata;
  }

  accelX = accelXTotal / samplesize;
  accelY = accelYTotal / samplesize;
  accelZ = accelZTotal / samplesize;

  //Set all three axes offsets
  if(changeOffset){
    setOffset(zeroX-accelX, zeroY-accelY, zeroZ-accelZ);
  }
}

void Accelerometer::calibrate(int samplesize, bool changeOffset){
  short accelX, accelY, accelZ;
  calibrate(accelX, accelY, accelZ, samplesize, changeOffset);
}

Quaternion Accelerometer::calcRotation(double yaw){
  double roll, pitch;
  calcRotation(roll, pitch);
  
  return Quaternion(roll, yaw, pitch);
}

void Accelerometer::calcRotation(double& roll, double& pitch){
  roll = range(atan2(-accelY, accelZ)+M_PI);                      //from the internet
  pitch = -atan2(accelX, sqrt(accelY*accelY + accelZ*accelZ));    //from the internet
}

void Accelerometer::step(double yaw){
  if(isReady()){
    read(accelX, accelY, accelZ);

    //TODO: Trim
    calcRotation(roll, pitch);
    
    rotation = calcRotation(yaw);
  }
}

void Accelerometer::setZeroReading(short accelX, short accelY, short accelZ){
  zeroX = accelX;
  zeroY = accelY;
  zeroZ = accelZ;
}

void Accelerometer::setOffset(short offsetX, short offsetY, short offsetZ){
  this->offsetX = offsetX;
  this->offsetY = offsetY;
  this->offsetZ = offsetZ;
}

bool Accelerometer::isReady(){
  return true;
}