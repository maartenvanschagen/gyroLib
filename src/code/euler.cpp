#include "euler.h"

Euler::Euler(){
  
}

Euler::Euler(double yaw, double pitch, double roll){  //creates quaternion from euler angles
  this->yaw = yaw;
  this->pitch = pitch;
  this->roll = roll;
  Euler();
}

