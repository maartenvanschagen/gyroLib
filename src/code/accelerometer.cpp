#include "accelerometer.h"

#include "defines.h"
#include "quaternion.h"
#include <cmath>

//TODO: support different callibration schemes, single-point calibration works quite poorly


double range(double val){
  return fmod((val + M_PI), (2 * M_PI)) - M_PI;
}

Vector3i Accelerometer::calibrate(int samplesize, bool changeOffset){
  Vector3i accelTotal = Vector3i(0, 0, 0);
  Vector3i accelData;

  //Set all three axes offsets to 0
  setOffset(0, 0, 0);

  for(int i = 0; i < samplesize; i++){
    while(!isReady()){}
    accelData = read();
    accelTotal += accelData;
  }

  Vector3i raw;
  raw.x = accelTotal.x / samplesize;
  raw.y = accelTotal.y / samplesize;
  raw.z = accelTotal.z / samplesize;

  //Set all three axes offsets
  if(changeOffset){
    setOffset(zero.x-raw.x, zero.y-raw.y, zero.z-raw.z);
  }
  return raw;
}

Euler Accelerometer::calcEuler(Vector3i raw){
  Euler e;
  e.roll = range(atan2(-raw.y, raw.z)+M_PI);                      //from the internet
  e.pitch = -atan2(raw.x, sqrt(raw.y*raw.y + raw.z*raw.z));       //from the internet
  return e;
}

Euler Accelerometer::calcEuler(Vector3i raw, double yaw){
  Euler e = calcEuler(raw);
  e.yaw = yaw;
  return e;
}

Euler Accelerometer::calcEuler(){
  return calcEuler(read());
}

Euler Accelerometer::calcEuler(double yaw){
  Euler e = calcEuler();
  e.yaw = yaw;
  return e;
}

void Accelerometer::step(double yaw){
  if(isReady()){
    rotation = calcEuler(yaw);
  }
}

Quaternion Accelerometer::calcQuaternion(Vector3i raw, double yaw){
  Euler e = calcEuler(raw);
  
  return Quaternion(yaw, e.pitch, e.roll);
}

Quaternion Accelerometer::calcQuaternion(double yaw){
  Euler e = calcEuler();
  
  return Quaternion(yaw, e.pitch, e.roll);
}

void Accelerometer::setZeroReading(Vector3d zero){
  this->zero = zero;
}

void Accelerometer::setZeroReading(double zeroX, double zeroY, double zeroZ){
  this->zero = Vector3d(zeroX, zeroY, zeroZ);
}

Vector3d Accelerometer::getZeroReading(){
  return zero;
}

void Accelerometer::setOffset(Vector3d offset){
  this->offset = offset;
}

void Accelerometer::setOffset(double offsetX, double offsetY, double offsetZ){
  setOffset(Vector3d(offsetX, offsetY, offsetZ));
}

Vector3d Accelerometer::getOffset(){
  return offset;
}

Quaternion Accelerometer::getQuaternion(){
  return Quaternion(rotation.yaw, rotation.pitch, rotation.roll);
}

Quaternion Accelerometer::getQuaternion(double yaw){
  return Quaternion(yaw, rotation.pitch, rotation.roll);
}

void Accelerometer::setQuaternion(Quaternion q){
  q.getEuler(rotation.yaw, rotation.pitch, rotation.roll);
}

void Accelerometer::setEuler(Euler e){
  rotation = e;
}

Euler Accelerometer::getEuler(){
  return rotation;
}

void Accelerometer::setYaw(double yaw){
  rotation.yaw = yaw;
}

bool Accelerometer::isReady(){
  return true;
}