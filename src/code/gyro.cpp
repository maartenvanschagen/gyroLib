#include "gyro.h"

#include "defines.h"
#include "Quaternion.h"
#include "wrapper.h"

#include <algorithm>

#ifdef DEBUG_GYROREADBUFFER
  #include <vector>
#endif


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
    
    double rotationX, rotationY, rotationZ;
    calcRotation(rotationX, rotationY, rotationZ, timePast);

    #ifdef DEBUG_GYROREADBUFFER
      rotationBuffer.push_back(Quaternion(rotationX, rotationY, rotationZ));
      if(rotationBuffer.size() > DEBUG_GYROREADBUFFER_MAXSIZE){rotationBuffer.erase(rotationBuffer.begin());}
    #endif
    
    Quaternion rotationChange = Quaternion();
    rotationChange.setGyro(rotationX, rotationY, rotationZ);
    rotation *= rotationChange; //rotate with the measured amount
    rotation.setMagnitude(1);
  }
}

void Gyro::transformMountRotation(double x, double y, double z, double& yaw, double& pitch, double& roll){
  double val[3] = {x, y, z};
  
  for(short i = 0; i < 3; i++){
    switch(YPR[i]){
      case 'X':
        yaw = val[i];
        if(reverse[0]){yaw=-yaw;}
        break;
      case 'Y':
        pitch = val[i];
        if(reverse[1]){pitch=-pitch;}
        break;
      case 'Z':
        roll = val[i];
        if(reverse[2]){roll=-roll;}
        break;
    }
  }
}

void Gyro::getEuler(double& yaw, double& pitch, double& roll){
  rotation.getEuler(yaw, pitch, roll);
}

//overloads

void Gyro::calibrate(int samplesize, bool changeOffset){
  double offsetX, offsetY, offsetZ;
  calibrate(offsetX, offsetY, offsetZ, samplesize, changeOffset);
}

void Gyro::calcRotation(int rawX, int rawY, int rawZ, double& yaw, double& pitch, double& roll, long timePast){
  calcRotation(rawX, rawY, rawZ, yaw, pitch, roll, offsetX, offsetY, offsetZ, timePast);
}

void Gyro::calcRotation(double& yaw, double& pitch, double& roll, long timePast){
  int rawX, rawY, rawZ;
  read(rawX, rawY, rawZ);
  calcRotation(rawX, rawY, rawZ, yaw, pitch, roll, offsetX, offsetY, offsetZ, timePast);
}

//placeholders

bool Gyro::isReady(){
   return true;
}

//getters and setters
void Gyro::setYPR(std::string YPR){
  this->YPR[0] = YPR[0];
  this->YPR[1] = YPR[1];
  this->YPR[2] = YPR[2];
}

void Gyro::setReverse(bool x, bool y, bool z){
  reverse[0] = x;
  reverse[1] = y;
  reverse[2] = z;
}