#include "gyro.h"

#include "defines.h"
#include "quaternion.h"
#include "wrapper.h"

#include <algorithm>

//functions

Vector3d Gyro::calibrate(int samplesize, bool changeOffset){
  Vector3i total;
  Vector3i raw;

  for(int i = 0; i < samplesize;){
    if(isReady()){
      i++;
      raw = read();
      total += raw;
    }
  }
  offset.x = (total.x / samplesize);
  offset.y = (total.y / samplesize);
  offset.z = (total.z / samplesize);
  
  if(changeOffset){
    this->offset = offset;
  }

  return offset;
}

void Gyro::step(){
  if(isReady()){
    timePast = wrapper::getMicros() - lastMicros;
    lastMicros = wrapper::getMicros();
    
    Vector3d rot = calcRotation(timePast);
    
    Quaternion rotationChange = Quaternion();
    rotationChange.setGyro(rot.x, rot.y, rot.z);
    rotation *= rotationChange;
    rotation.setMagnitude(1);

    if(accelerometer != NULL){
      nudgeRotationTowards(*accelerometer);
    }
  }
}

Vector3d Gyro::transformRotation(Vector3d raw){
  double val[3] = {raw.x, raw.y, raw.z};
  Vector3d result;
  
  for(short i = 0; i < 3; i++){
    switch(axesSwitched[i]){
      case 'X':
        result.x = val[i];
        if(axesReversed[0]){result.x=-result.x;}
        break;
      case 'Y':
        result.y = val[i];
        if(axesReversed[1]){result.y=-result.y;}
        break;
      case 'Z':
        result.z = val[i];
        if(axesReversed[2]){result.z=-result.z;}
        break;
    }
  }
  return result;
}

Quaternion Gyro::getQuaternion(){
  return rotation;
}

void Gyro::setQuaternion(Quaternion q){
  rotation = q;
}

//overloads

Vector3d Gyro::calcRotation(Vector3i raw, long timePast){
  return calcRotation(raw, offset, timePast);
}

Vector3d Gyro::calcRotation(long timePast){
  Vector3i raw = read();
  return calcRotation(raw, timePast);
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

void Gyro::setOffset(Vector3d offset){
  this->offset = offset;
}

void Gyro::setOffset(double offsetX, double offsetY, double offsetZ){
  this->offset.x = offsetX;
  this->offset.y = offsetY;
  this->offset.z = offsetZ;
}

Vector3d Gyro::getOffset(){
  return offset;
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