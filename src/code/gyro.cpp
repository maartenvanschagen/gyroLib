#include "gyro.h"

#include "defines.h"
#include "Quaternion.h"

#ifdef DEBUG_GYROREADBUFFER
  #include <vector>
#endif

#include "freertos/FreeRTOS.h"

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
    timePast = esp_timer_get_time() - lastMicros;
    lastMicros = esp_timer_get_time();
    
    double rotationX, rotationY, rotationZ;
    calcRotation(rotationX, rotationY, rotationZ, timePast);

    #ifdef DEBUG_GYROREADBUFFER
      rotationBuffer.push_back(Quaternion(rotationX, rotationY, rotationZ));
      if(rotationBuffer.size() > DEBUG_GYROREADBUFFER_MAXSIZE){rotationBuffer.erase(rotationBuffer.begin());}
    #endif
    
    rotation *= Quaternion(rotationX, rotationY, rotationZ); //rotate with the measured amount
    rotation.setMagnitude(1);
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

void Gyro::calcRotation(int rawX, int rawY, int rawZ, double& rotationX, double& rotationY, double& rotationZ, long timePast){
  calcRotation(rawX, rawY, rawZ, rotationX, rotationY, rotationZ, offsetX, offsetY, offsetZ, timePast);
}

void Gyro::calcRotation(double& rotationX, double& rotationY, double& rotationZ, long timePast){
  int rawX, rawY, rawZ;
  read(rawX, rawY, rawZ);
  calcRotation(rawX, rawY, rawZ, rotationX, rotationY, rotationZ, offsetX, offsetY, offsetZ, timePast);
}

//placeholders

bool Gyro::isReady(){
   return true;
}
