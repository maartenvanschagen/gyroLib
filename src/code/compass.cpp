#include "compass.h"
#include "wrapper.h"
#include <math.h>


compassCalibrate Compass::calibrate(int samplesize, bool changeValues){  // TODO: add support for 3x3 matrix correction for better accuracy (make choice, this method is quicker)
    // TODO: fix function
    // algoritms from https://github.com/kriswiner/MPU6050/wiki/Simple-and-Effective-Magnetometer-Calibration
    Vector3d maxValues;
    Vector3d minValues;

    for(int i = 0; i < samplesize;){
        if(isReady()){
            i++;

            Vector3d value = calcGaus(read());

            if(value.x > maxValues.x) maxValues.x = value.x;
            if(value.y > maxValues.y) maxValues.y = value.y;
            if(value.z > maxValues.z) maxValues.z = value.z;

            if(value.x < minValues.x) minValues.x = value.x;
            if(value.y < minValues.y) minValues.y = value.y;
            if(value.z < minValues.z) minValues.z = value.z;
            wrapper::delayMillis(15);
        }
    }

    compassCalibrate result;

    result.offset = (maxValues + minValues) / 2;  // Gets center value

    Vector3d averageDelta = (maxValues - minValues) / 2;                                // Sets distance from center the same for every axes, 
    double averageDeltaTotal = (averageDelta.x + averageDelta.y + averageDelta.z) / 3;  // turning a triaxial ellipsoid into a sphere
    result.scale = averageDelta / averageDeltaTotal;                                    // (2d equivilant: turning an ellipse into a circle)

    if(changeValues) calibration = result;
    return result;
}

Vector3d Compass::calcField(Vector3d value, Vector3d offset, Vector3d scale){
    return (value - offset) * scale;
}

Vector3d Compass::calcField(Vector3d value, compassCalibrate calibrateVal){
    return calcField(value, calibrateVal.offset, calibrateVal.scale);
}

Vector3d Compass::calcField(Vector3d value){
    return calcField(value, this->calibration);
}

Vector3d Compass::getField(){
    return calcField(calcGaus(read()));
}

double Compass::getYaw(Vector3d field){  // TODO: add support for 3d
    return atan2(field.x, field.z);
    //return getFieldDirection(field).getEuler().yaw;
}

double Compass::getYaw(){
    return getYaw(getField());
}

Quaternion Compass::getFieldDirection(Vector3d field){//TODO: doesn't work
    return Quaternion(0, field.x, field.y, field.z).setMagnitude(1);
}

Quaternion Compass::getFieldDirection(){
    return getFieldDirection(getField());
}


//not necessary to override

bool Compass::isReady(){
    return true;
}