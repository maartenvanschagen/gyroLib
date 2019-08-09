#include "gyro.h"

#include "defines.h"
#include "quaternion.h"
#include "wrapper.h"

#include <algorithm>

//functions

Vector3<double> Gyro::calibrate(int samplesize, bool changeOffset){
  Vector3<int> total;
  Vector3<int> raw;

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
    
    Vector3<double> rot = calcRotation(timePast);
    
    Quaternion rotationChange = Quaternion();
    rotationChange.setGyro(rot.x, rot.y, rot.z);
    rotation *= rotationChange;
    rotation.setMagnitude(1);

    if(accelerometer != NULL){
      nudgeRotationTowards(*accelerometer);
    }
  }
}

void Gyro::transformRotation(Vector3<double> raw, double& yaw, double& pitch, double& roll){
  double val[3] = {raw.x, raw.y, raw.z};
  
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

Vector3<double> Gyro::calcRotation(Vector3<int> raw, long timePast){
  Vector3<double> rotation;
  calcRotation(raw.x, raw.y, raw.z, rotation.x, rotation.y, rotation.z, offset.x, offset.y, offset.z, timePast);
  return rotation;
}

Vector3<double> Gyro::calcRotation(long timePast){
  Vector3<int> raw = read();
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

void Gyro::setOffset(Vector3<double> offset){
  this->offset = offset;
}

Vector3<double> Gyro::getOffset(){
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