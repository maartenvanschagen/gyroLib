#include "gyro.h"

#include "quaternion.h"
#include "wrapper.h"

#include <stdlib.h>
#include <math.h>

//functions

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

Vector3d Gyro::calibrate(int samplesize, bool changeOffset){
  Vector3i total;
  Vector3i raw;
  Vector3d offset;

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

Vector3d Gyro::calcRotation(Vector3i raw, long timePast){
  return calcRotation(raw, offset, timePast);
}

Vector3d Gyro::calcRotation(long timePast){
  Vector3i raw = read();
  return calcRotation(raw, timePast);
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

void Gyro::nudgeRotationTowards(Quaternion q){
  double reverseFactor = fabs(rotation.w - q.w) + fabs(rotation.x - q.x) + fabs(rotation.y - q.y) + fabs(rotation.z - q.z);
  if(reverseFactor > 0.5) q = -q;  //TODO: find a more scientificly correct method of doing this
  rotation *= 0.999;
  rotation += q * .001;
}

void Gyro::nudgeRotationTowards(Accelerometer& a){
  nudgeRotationTowards(a.getQuaternion(rotation.getEuler().yaw));
}

//getters and setters

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
  setOffset(Vector3d(offsetX, offsetY, offsetZ));
}

Euler Gyro::getEuler(){
  return rotation.getEuler();
}

void Gyro::setEuler(Euler e){
  rotation.setEuler(e);
}

Quaternion Gyro::getQuaternion(){
  return rotation;
}

void Gyro::setQuaternion(Quaternion q){
  rotation = q;
}

Accelerometer* Gyro::getAccelerometer(){
  return accelerometer;
}

void Gyro::setAccelerometer(Accelerometer* a){
  accelerometer = a;
}

//not necessary to override

bool Gyro::isReady(){
   return true;
}

Vector3d Gyro::getOffset(){
  return offset;
}

void Gyro::setOffset(Vector3d offset){
  this->offset = offset;
}