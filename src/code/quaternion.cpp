#include "quaternion.h"

#include <math.h>

Quaternion::Quaternion(double w, double x, double y, double z){
  setValue(w, x, y, z);
  Quaternion();
}

Quaternion::Quaternion(double yaw, double pitch, double roll){
  setEuler(yaw, pitch, roll);
  Quaternion();
}

Quaternion::Quaternion(Euler e){
  setEuler(e);
  Quaternion();
}

Quaternion::Quaternion(){
  
}

Quaternion::~Quaternion(){
}


double* Quaternion::getValue(){
  static double temp[4] = {w, x, y, z};
  return temp;
}

void Quaternion::getValue(double& w, double& x, double& y, double& z){
  w = this->w;
  x = this->x;
  y = this->y;
  z = this->z;
}

void Quaternion::setValue(double w, double x, double y, double z){
  this->w = w;
  this->x = x;
  this->y = y;
  this->z = z;
}

void Quaternion::getEuler(double& yaw, double& pitch, double& roll){   //~90 microseconds ESP32  ,  euler in XYZ     ///Modified version of WIKIPEDIA code
  // roll (x-axis rotation)
  double sinr_cosp = +2.0 * (w * x + y * z);
  double cosr_cosp = w*w - x*x - y*y + z*z;
  roll = atan2(sinr_cosp, cosr_cosp);

  // pitch (y-axis rotation)
  double sinp = +2.0 * ( z * x - w * y);
  if (fabs(sinp) >= 1)
    pitch = -copysign(M_PI / 2, sinp); // use 90 degrees if out of range
  else
    pitch = -asin(sinp);

  // yaw (z-axis rotation)
  double siny_cosp = +2.0 * (w * z + x * y);
  double cosy_cosp = w*w + x*x - y*y - z*z;  
  yaw = atan2(siny_cosp, cosy_cosp);
}

Euler Quaternion::getEuler(){
  Euler e;
  getEuler(e.yaw, e.pitch, e.roll);
  return e;
}

void Quaternion::setEuler(double yaw, double pitch, double roll){   //euler in XYZ
  double cy = cos(yaw/2), cp = cos(pitch/2), cr = cos(roll/2);
  double sy = sin(yaw/2), sp = sin(pitch/2), sr = sin(roll/2);

  w = cy * cp * cr + sy * sp * sr;        //euler in XYZ
  x = cy * cp * sr - sy * sp * cr;
  y = sy * cp * sr + cy * sp * cr;
  z = sy * cp * cr - cy * sp * sr;
}

void Quaternion::setEuler(Euler e){
  setEuler(e.yaw, e.pitch, e.roll);
}

void Quaternion::setGyro(double gx, double gy, double gz){
  double cx = cos(gx/2), cy = cos(gy/2), cz = cos(gz/2);
  double sx = sin(gx/2), sy = sin(gy/2), sz = sin(gz/2);

  w = (cx * cy * cz) - (sy * sy * sz);
  x = (sx * cy * cz) + (cy * sy * sz);
  y = (cx * sy * cz) - (sy * cy * sz);
  z = (sx * sy * cz) + (cy * cy * sz);
}

void Quaternion::setMagnitude(double const dist){   //~40 microseconds ESP32
  double factor = dist/sqrt((w*w + x*x + y*y + z*z));
  *this *= factor;
}

double Quaternion::roughDistance(Quaternion q2){ //from https://math.stackexchange.com/questions/90081/quaternion-distance
  double innerProduct = w * q2.w + x * q2.x + y * q2.y + z * q2.z;
  return (1 - (innerProduct * innerProduct));
}

//operators

Quaternion Quaternion::operator* (Quaternion const& q2){  //~80 microseconds ESP32
  Quaternion qnew = Quaternion();
  qnew.w = (w * q2.w) - (x * q2.x) - (y * q2.y) - (z * q2.z);
  qnew.x = (x * q2.w) + (w * q2.x) - (z * q2.y) + (y * q2.z);
  qnew.y = (y * q2.w) + (z * q2.x) + (w * q2.y) - (x * q2.z);
  qnew.z = (z * q2.w) - (y * q2.x) + (x * q2.y) + (w * q2.z);
  return qnew;
}

Quaternion& Quaternion::operator*= (Quaternion const& q2){  //~80 microseconds ESP32
  w = (w * q2.w) - (x * q2.x) - (y * q2.y) - (z * q2.z);
  x = (x * q2.w) + (w * q2.x) - (z * q2.y) + (y * q2.z);
  y = (y * q2.w) + (z * q2.x) + (w * q2.y) - (x * q2.z);
  z = (z * q2.w) - (y * q2.x) + (x * q2.y) + (w * q2.z);
  return *this;
}

Quaternion Quaternion::operator* (double const& val){
  Quaternion qnew = Quaternion();
  qnew.w = w * val;
  qnew.x = x * val;
  qnew.y = y * val;
  qnew.z = z * val;
  return qnew;
}

Quaternion& Quaternion::operator*= (double const& val){
  w *= val;
  x *= val;
  y *= val;
  z *= val;
  return *this;
}

Quaternion Quaternion::operator+ (Quaternion const& q2){
  Quaternion qnew = Quaternion();
  qnew.w = w + q2.w;
  qnew.x = x + q2.x;
  qnew.y = y + q2.y;
  qnew.z = z + q2.z;
  return qnew;
}

Quaternion& Quaternion::operator+= (Quaternion const& q2){
  w += q2.w;
  x += q2.x;
  y += q2.y;
  z += q2.z;
  return *this;
}

Quaternion Quaternion::operator- (Quaternion const& q2){
  Quaternion qnew = Quaternion();
  qnew.w = w - q2.w;
  qnew.x = x - q2.x;
  qnew.y = y - q2.y;
  qnew.z = z - q2.z;
  return qnew;
}

Quaternion& Quaternion::operator-= (Quaternion const& q2){
  w -= q2.w;
  x -= q2.x;
  y -= q2.y;
  z -= q2.z;
  return *this;
}

Quaternion Quaternion::operator- (){
  Quaternion qnew;
  qnew.w = -w;
  qnew.x = -x;
  qnew.y = -y;
  qnew.z = -z;
  return qnew;
}