#include "accelerometer.h"

#include "defines.h"
#include "Quaternion.h"
#include <cmath>

//TODO: support different callibration schemes, single-point calibration works quite poorly


double range(double val){
  return fmod((val + M_PI), (2 * M_PI)) - M_PI;
}

void Accelerometer::calibrate(int& rawX, int& rawY, int& rawZ, int samplesize, bool changeOffset){
  long accelXTotal = 0;
  long accelYTotal = 0;
  long accelZTotal = 0;
  int accelXdata, accelYdata, accelZdata;

  //Set all three axes offsets to 0
  setOffset(0, 0, 0);

  for(int i = 0; i < samplesize; i++){
    while(!isReady()){}
    read(accelXdata, accelYdata, accelZdata);
    accelXTotal += accelXdata;
    accelYTotal += accelYdata;
    accelZTotal += accelZdata;
  }

  rawX = accelXTotal / samplesize;
  rawY = accelYTotal / samplesize;
  rawZ = accelZTotal / samplesize;

  //Set all three axes offsets
  if(changeOffset){
    setOffset(zeroX-rawX, zeroY-rawY, zeroZ-rawZ);
  }
}

void Accelerometer::calibrate(int samplesize, bool changeOffset){
  int rawX, rawY, rawZ;
  calibrate(rawX, rawY, rawZ, samplesize, changeOffset);
}

void Accelerometer::calcRotation(int rawX, int rawY, int rawZ, double& pitch, double& roll){
  roll = range(atan2(-rawY, rawZ)+M_PI);                      //from the internet
  pitch = -atan2(rawX, sqrt(rawY*rawY + rawZ*rawZ));          //from the internet
}

void Accelerometer::calcRotation(double& pitch, double& roll){
  int rawX, rawY, rawZ;
  read(rawX, rawY, rawZ);
  calcRotation(rawX, rawY, rawZ, pitch, roll);
}

void Accelerometer::step(){
  if(isReady()){
    calcRotation(pitch, roll);
  }
}

Quaternion Accelerometer::calcRotation(int rawX, int rawY, int rawZ, double yaw){
  double pitch, roll;
  calcRotation(rawX, rawY, rawZ, pitch, roll);
  
  return Quaternion(yaw, pitch, roll);
}

Quaternion Accelerometer::calcRotation(double yaw){
  double pitch, roll;
  calcRotation(pitch, roll);
  
  return Quaternion(yaw, pitch, roll);
}

void Accelerometer::setZeroReading(double zeroX, double zeroY, double zeroZ){
  this->zeroX = zeroX;
  this->zeroY = zeroY;
  this->zeroZ = zeroZ;
}

void Accelerometer::setOffset(double offsetX, double offsetY, double offsetZ){
  this->offsetX = offsetX;
  this->offsetY = offsetY;
  this->offsetZ = offsetZ;
}

Quaternion Accelerometer::getQuaternion(double yaw){
  return Quaternion(yaw, pitch, roll);
}

bool Accelerometer::isReady(){
  return true;
}