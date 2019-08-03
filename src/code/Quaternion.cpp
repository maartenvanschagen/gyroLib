#include "Quaternion.h"

#include "defines.h"

#include <cmath>

Quaternion::Quaternion(double wi, double xi, double yi, double zi){
  w = wi; x = xi; y = yi; x = xi;
}

Quaternion::Quaternion(){
  w = 1; x = 0; y = 0; x = 0;
}

Quaternion::Quaternion(double yaw, double pitch, double roll){  //creates quaternion from euler angles
  setEuler(yaw, pitch, roll);
}

Quaternion::~Quaternion(){
}


double* Quaternion::getValue(){
  static double temp[4] = {w, x, y, z};
  return temp;
}

/*
void Quaternion::getEuler(double& yaw, double& pitch, double& roll){   //~90 microseconds  ,  euler in XYZ     ///Modified version of WIKIPEDIA code
  // roll (x-axis rotation)
  double sinr_cosp = +2.0 * (w * x + y * z);
  double cosr_cosp = +1.0 - 2.0 * (x * x + y * y);
  roll = atan2(sinr_cosp, cosr_cosp);

  // pitch (y-axis rotation)
  double sinp = +2.0 * (w * y - z * x);
  if (fabs(sinp) >= 1)
    pitch = copysign(M_PI / 2, sinp); // use 90 degrees if out of range
  else
    pitch = asin(sinp);

  // yaw (z-axis rotation)
  double siny_cosp = +2.0 * (w * z + x * y);
  double cosy_cosp = +1.0 - 2.0 * (y * y + z * z);  
  yaw = atan2(siny_cosp, cosy_cosp);
}*/

void Quaternion::getEuler(double& yaw, double& pitch, double& roll){   //~90 microseconds  ,  euler in XYZ     ///Modified version of WIKIPEDIA code
  // roll (x-axis rotation)
  double sinr_cosp = +2.0 * (w * x + y * z);
  double cosr_cosp = w*w - x*x - y*y + z*z;
  roll = atan2(sinr_cosp, cosr_cosp);

  // pitch (y-axis rotation)
  double sinp = +2.0 * ( z * x - w * y);
  if (fabs(sinp) >= 1)
    pitch = copysign(M_PI / 2, sinp); // use 90 degrees if out of range
  else
    pitch = asin(sinp);

  // yaw (z-axis rotation)
  double siny_cosp = +2.0 * (w * z + x * y);
  double cosy_cosp = w*w + x*x - y*y - z*z;  
  yaw = atan2(siny_cosp, cosy_cosp);
}

void Quaternion::setEuler(double yaw, double pitch, double roll){   //euler in ZYX
  double cy = cos(yaw/2), cp = cos(pitch/2), cr = cos(roll/2);
  double sy = sin(yaw/2), sp = sin(pitch/2), sr = sin(roll/2);

/*w = (cy * cp * cr) - (sy * sp * sr);    //euler in XYZ
  x = (sy * cp * cr) + (cy * sp * sr);
  y = (cy * sp * cr) - (sy * cp * sr);
  z = (sy * sp * cr) + (cy * cp * sr);*/
  w = (cy * cp * cr) + (sy * sp * sr);    //euler in ZYX
  x = (sy * cp * cr) - (cy * sp * sr);
  y = (cy * sp * cr) + (sy * cp * sr);
  z = (cy * cp * sr) - (sy * sp * cr);
}

void Quaternion::setMagnitude(double const dist){   //~40 microseconds
  double factor = dist/sqrt((w*w + x*x + y*y + z*z));
  w *= factor;
  x *= factor;
  y *= factor;
  z *= factor;
}

Quaternion Quaternion::operator* (Quaternion const& q2){  //~80 microseconds
  Quaternion qnew = Quaternion();
  qnew.w = (w * q2.w) - (x * q2.x) - (y * q2.y) - (z * q2.z);
  qnew.x = (x * q2.w) + (w * q2.x) - (z * q2.y) + (y * q2.z);
  qnew.y = (y * q2.w) + (z * q2.x) + (w * q2.y) - (x * q2.z);
  qnew.z = (z * q2.w) - (y * q2.x) + (x * q2.y) + (w * q2.z);
  return qnew;
}

Quaternion& Quaternion::operator*= (Quaternion const& q2){  //~80 microseconds
  w = (w * q2.w) - (x * q2.x) - (y * q2.y) - (z * q2.z);
  x = (x * q2.w) + (w * q2.x) - (z * q2.y) + (y * q2.z);
  y = (y * q2.w) + (z * q2.x) + (w * q2.y) - (x * q2.z);
  z = (z * q2.w) - (y * q2.x) + (x * q2.y) + (w * q2.z);
  return *this;
}

