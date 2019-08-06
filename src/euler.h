#ifndef EULER_H
#define EULER_H

struct Euler {    
  public:
    double yaw = 0, pitch = 0, roll = 0;

    Euler(double yaw, double pitch, double roll);
    Euler();
    ~Euler();

    void setEuler(double yaw, double pitch, double roll);
  
  private:

};

#endif
