#include "gyro.h"

#include "defines.h"
#include "quaternion.h"
#include "wrapper.h"

#include <algorithm>

//functions

void Gyro::calibrate(double& offsetX, double& offsetY, double& offsetZ, int samplesize, bool changeOffset){
  long totalX = 0, totalY = 0, totalZ = 0;
  int rawX, rawY, rawZ;

  for(int i = 0; i < samplesize;){
    if(isReady()){
      i++;
      read(rawX, rawY, rawZ);
      totalX += rawX;
      totalY += rawY;
      totalZ += rawZ;
    }
  }
  offsetX = (totalX / samplesize);
  offsetY = (totalY / samplesize);
  offsetZ = (totalZ / samplesize);
  
  if(changeOffset){
    this->offsetX = offsetX;
    this->offsetY = offsetY;
    this->offsetZ = offsetZ;
  }
}

void Gyro::step(){
  if(isReady()){
    timePast = wrapper::getMicros() - lastMicros;
    lastMicros = wrapper::getMicros();
    
    Euler e = calcEuler(timePast);
    
    Quaternion rotationChange = Quaternion();
    rotationChange.setGyro(e.yaw, e.pitch, e.roll);
    rotation *= rotationChange;
    rotation.setMagnitude(1);

    if(accelerometer != NULL){
      nudgeRotationTowards(*accelerometer);
    }
  }
}

void Gyro::transformRotation(double x, double y, double z, double& yaw, double& pitch, double& roll){
  double val[3] = {x, y, z};
  
  for(short i = 0; i < 3; i++){
    switch(axesSwitched[i]){
      case 'X':
        yaw = val[i];
        if(axesReversed[0]){yaw=-yaw;}
        break;
      case 'Y':
        pitch = val[i];
        if(axesReversed[1]){pitch=-pitch;}
        break;
      case 'Z':
        roll = val[i];
        if(axesReversed[2]){roll=-roll;}
        break;
    }
  }
}

Quaternion Gyro::getQuaternion(){
  return rotation;
}

void Gyro::setQuaternion(Quaternion q){
  rotation = q;
}

//overloads

void Gyro::calibrate(int samplesize, bool changeOffset){
  double offsetX, offsetY, offsetZ;
  calibrate(offsetX, offsetY, offsetZ, samplesize, changeOffset);
}

Euler Gyro::calcEuler(int rawX, int rawY, int rawZ, long timePast){
  Euler e;
  calcRotation(rawX, rawY, rawZ, e.yaw, e.pitch, e.roll, offsetX, offsetY, offsetZ, timePast);
  return e;
}

Euler Gyro::calcEuler(long timePast){
  int rawX, rawY, rawZ;
  read(rawX, rawY, rawZ);
  return calcEuler(rawX, rawY, rawZ, timePast);
}

void Gyro::nudgeRotationTowards(Quaternion q){
  double reverseFactor = abs(rotation.w - q.w) + abs(rotation.x - q.x) + abs(rotation.y - q.y) + abs(rotation.z - q.z);
  if(reverseFactor > 0.5) q = -q;  //TODO: find a more scientific method of doing this
  rotation *= 0.999;
  rotation += q * .001;
}

void Gyro::nudgeRotationTowards(Accelerometer& a){
  nudgeRotationTowards(a.getQuaternion(rotation.getEuler().yaw));
}

//getters and setters
void Gyro::setAxesSwitched(std::string axesSwitched){
  setAxesSwitched(axesSwitched[0], axesSwitched[1], axesSwitched[2]);
}

void Gyro::setAxesSwitched(char xAxis, char yAxis, char zAxis){
  this->axesSwitched[0] = xAxis;
  this->axesSwitched[1] = yAxis;
  this->axesSwitched[2] = zAxis;
}

void Gyro::setAxesReversed(bool x, bool y, bool z){
  axesReversed[0] = x;
  axesReversed[1] = y;
  axesReversed[2] = z;
}

void Gyro::setOffset(double offsetX, double offsetY, double offsetZ){
  this->offsetX = offsetX;
  this->offsetY = offsetY;
  this->offsetZ = offsetZ;
}

void Gyro::getOffset(double& offsetX, double& offsetY, double& offsetZ){
  offsetX = this->offsetX;
  offsetY = this->offsetY;
  offsetZ = this->offsetZ;
}

void Gyro::setEuler(Euler e){
  rotation.setEuler(e);
}

Euler Gyro::getEuler(){
  return rotation.getEuler();
}

void Gyro::setAccelerometer(Accelerometer* a){
  accelerometer = a;
}

Accelerometer* Gyro::getAccelerometer(){
  return accelerometer;
}

//placeholders

bool Gyro::isReady(){
   return true;
}