#ifndef QUATERNION_H
#define QUATERNION_H

#include "euler.h"
#include "wrapper.h"


//euler is always calculated in XYZ

class Quaternion {    
  public:
    double w = 1, x = 0, y = 0, z = 0;
    
    Quaternion(double w, double x, double y, double z);
    Quaternion(double yaw, double pitch, double roll);
    Quaternion(Euler euler);
    Quaternion();
    ~Quaternion();
    
    double* getValue();
    void getValue(double& w, double& x, double& y, double& z);
    void setValue(double w, double x, double y, double z);
    void getEuler(double& yaw, double& pitch, double& roll);
    Euler getEuler();
    void setEuler(double yaw, double pitch, double roll);
    void setEuler(Euler e);
    void setGyro(double gx, double gy, double gz);  //different than Euler because these rotations are simultanious, not in sequence
    void setMagnitude(double const dist);
    double roughDistance(Quaternion q2);
    
    Quaternion operator* (Quaternion const& q2);
    Quaternion& operator*= (Quaternion const& q2);
    Quaternion operator* (double const& val);
    Quaternion& operator*= (double const& val);
    Quaternion operator+ (Quaternion const& q2);
    Quaternion& operator+= (Quaternion const& q2);
    Quaternion operator- (Quaternion const& q2);
    Quaternion& operator-= (Quaternion const& q2);
    Quaternion operator- ();
  
  private:

};

#endif
