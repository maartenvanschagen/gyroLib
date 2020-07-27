#ifndef VECTOR3_H
#define VECTOR3_H

#include <stdint.h>

template<class T = double> 
struct Vector3{
  T x = 0, y = 0, z = 0;

  Vector3(T x, T y, T z){
    this->x = x;
    this->y = y;
    this->z = z;
    Vector3();
  }
  
  Vector3(){

  }

  Vector3<T> operator+ (Vector3<T> const& v2){
    Vector3<T> vnew = Vector3<T>();
    vnew.x = x + v2.x;
    vnew.y = y + v2.y;
    vnew.z = z + v2.z;
    return vnew;
  }

  Vector3<T>& operator+= (Vector3<T> const& v2){
    x += v2.x;
    y += v2.y;
    z += v2.z;
    return *this;
  }

  Vector3<T> operator- (Vector3<T> const& v2){
    Vector3<T> vnew = Vector3<T>();
    vnew.x = x - v2.x;
    vnew.y = y - v2.y;
    vnew.z = z - v2.z;
    return vnew;
  }

  Vector3<T>& operator-= (Vector3<T> const& v2){
    x -= v2.x;
    y -= v2.y;
    z -= v2.z;
    return *this;
  }

  Vector3<T> operator* (Vector3<T> const& v2){
    Vector3<T> vnew = Vector3<T>();
    vnew.x = x * v2.x;
    vnew.y = y * v2.y;
    vnew.z = z * v2.z;
    return vnew;
  }

  Vector3<T>& operator*= (Vector3<T> const& v2){
    x *= v2.x;
    y *= v2.y;
    z *= v2.z;
    return *this;
  }

  Vector3<T> operator* (T const& c2){
    Vector3<T> vnew = Vector3<T>();
    vnew.x = x * c2;
    vnew.y = y * c2;
    vnew.z = z * c2;
    return vnew;
  }

  Vector3<T>& operator*= (T const& c2){
    x *= c2;
    y *= c2;
    z *= c2;
    return *this;
  }

  Vector3<T> operator/ (Vector3<T> const& v2){
    Vector3<T> vnew = Vector3<T>();
    vnew.x = x / v2.x;
    vnew.y = y / v2.y;
    vnew.z = z / v2.z;
    return vnew;
  }

  Vector3<T>& operator/= (Vector3<T> const& v2){
    x /= v2.x;
    y /= v2.y;
    z /= v2.z;
    return *this;
  }

  Vector3<T> operator/ (T const& c2){
    Vector3<T> vnew = Vector3<T>();
    vnew.x = x / c2;
    vnew.y = y / c2;
    vnew.z = z / c2;
    return vnew;
  }

  Vector3<T>& operator/= (T const& c2){
    x /= c2;
    y /= c2;
    z /= c2;
    return *this;
  }
};

//shorthand definitions
typedef Vector3<double> Vector3d;  //double
typedef Vector3<float> Vector3f;   //float
typedef Vector3<int32_t> Vector3i; //int
typedef Vector3<int64_t> Vector3l; //long
typedef Vector3<int16_t> Vector3s; //short

#endif
