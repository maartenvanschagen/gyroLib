#ifndef EULER_H
#define EULER_H

struct Euler {
  double yaw = 0, pitch = 0, roll = 0;

  Euler(double yaw, double pitch, double roll);
  Euler();

};

#endif
