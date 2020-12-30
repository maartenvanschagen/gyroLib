/*#include "compasses/HMC5883L.h"
#include "accelerometers/ADXL345.h"
#include <Arduino.h>
#include "I2CWrapper.h"

HMC5883L compass = HMC5883L();
ADXL345 accel = ADXL345();

Quaternion start;

void setup();
void loop();

void setup() {
    Serial.begin(115200);

    I2C::init();
    compass.init();
    accel.setAxesSwitched('X', 'Y', 'Z');
    accel.setAxesReversed(false, false, false);
    accel.init();

    Serial.println("Start calibration");
    compassCalibrate calVal = compass.calibrate();
    Serial.println("Calibration complete, results:");
    Serial.print("offset: ");
    Serial.print(calVal.offset.x);
    Serial.print(" ");
    Serial.print(calVal.offset.y);
    Serial.print(" ");
    Serial.println(calVal.offset.z);
    Serial.print("scale: ");
    Serial.print(calVal.scale.x);
    Serial.print(" ");
    Serial.print(calVal.scale.y);
    Serial.print(" ");
    Serial.println(calVal.scale.z);
}

void loop(){
    accel.step();
    double heading = compass.getYaw(compass.getCorrectedField(accel.getQuaternion(0)));

    Serial.println("----head------");
    Serial.println(heading *180/PI);

    Euler dirAccel = accel.getQuaternion(0).getEuler();

    Serial.println("---accel-dir----");
    Serial.print(dirAccel.yaw);
    Serial.print(" ");
    Serial.print(dirAccel.pitch);
    Serial.print(" ");
    Serial.println(dirAccel.roll);

    /*Euler fieldDir = compass.getFieldDirection().getEuler(); //TODO: getFieldDirection() doesn't work

    Serial.println("---field-dir----");
    Serial.print(fieldDir.yaw);
    Serial.print(" ");
    Serial.print(fieldDir.pitch);
    Serial.print(" ");
    Serial.println(fieldDir.roll);*/
/*
    Vector3d field = compass.getField();

    Serial.println("---field----");
    Serial.print(field.x);
    Serial.print(" ");
    Serial.print(field.y);
    Serial.print(" ");
    Serial.println(field.z);

    Vector3d correctedfield = compass.correctField(field, accel.getQuaternion(0));

    Serial.println("---corrected-field----");
    Serial.print(correctedfield.x);
    Serial.print(" ");
    Serial.print(correctedfield.y);
    Serial.print(" ");
    Serial.println(correctedfield.z);

    delay(250);
}
*/