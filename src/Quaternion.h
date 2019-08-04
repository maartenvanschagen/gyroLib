#ifndef QUATERNION_H
#define QUATERNION_H

class Quaternion {
  private:
    
  public:
    double w, x, y, z;
    Quaternion(double w, double x, double y, double z);
    Quaternion(double yaw, double pitch, double roll);
    Quaternion();
    ~Quaternion();
    
    double* getValue();
    void getEuler(double& yaw, double& pitch, double& roll);
    void setEuler(double yaw, double pitch, double roll);
    void setGyro(double gx, double gy, double gz);
    void setMagnitude(double const dist);
    
    Quaternion operator* (Quaternion const& q2);
    Quaternion& operator*= (Quaternion const& q2);
    Quaternion operator* (double const& val);
    Quaternion& operator*= (double const& val);
    Quaternion operator+ (Quaternion const& q2);
    Quaternion& operator+= (Quaternion const& q2);
    Quaternion operator- (Quaternion const& q2);
    Quaternion& operator-= (Quaternion const& q2);
};

#endif
