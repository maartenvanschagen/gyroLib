#include "gyro.h"

#include "defines.h"
#include "Quaternion.h"
#include "I2C.h"

#include <cstdint>
#include <math.h>
#ifdef DEBUG_GYROREADBUFFER
  #include <vector>
#endif

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


 void Gyro::calcRot(double& rotX, double& rotY, double& rotZ, int trimX, int trimY, int trimZ, long timePast){ // ~15 microseconds
   rotX = -(gyroX - trimX) * 0.0175 * (timePast / 1000000.0) * (M_PI/180.0);    // (gyro trimmed) * sensitivity op 500 dps * timePast(s) * degToRad
   rotY = -(gyroY - trimY) * 0.0175 * (timePast / 1000000.0) * (M_PI/180.0);    // formula gets calculated by compiler (I think)
   rotZ = (gyroZ - trimZ) * 0.0175 * (timePast / 1000000.0) * (M_PI/180.0);
 }
 
 void Gyro::setTrim(double& TrimXI, double& TrimYI, double& TrimZI, int samplesize){
   long totX = 0, totY = 0, totZ = 0;
   short gyroX, gyroY, gyroZ;
   for(int i = 0; i < samplesize;){
       if(isReady()){
         i++;
         read(gyroX, gyroY, gyroZ);
         totX += gyroX;
         totY += gyroY;
         totZ += gyroZ;
       }
     }
     TrimXI = (totX / samplesize);
     TrimYI = (totY / samplesize);
     TrimZI = (totZ / samplesize);
     
     TrimXI = 0;
     TrimYI = 0;
     TrimZI = 0;
 }
 
 
 void Gyro::init(){
   //set trim
   vTaskDelay(10 / portTICK_PERIOD_MS);
   setTrim(trimX, trimY, trimZ, 1000); //sample size 1000 (takes 5/4 seconds)
 }
 
 void Gyro::step(){
   if(isReady()){
     read(gyroX, gyroY, gyroZ);
         
     timePast = esp_timer_get_time() - lastMicros;
     lastMicros = esp_timer_get_time();
     
     double rotX, rotY, rotZ;
     calcRot(rotX, rotY, rotZ, trimX, trimY, trimZ, timePast);
 
     #ifdef DEBUG_GYROREADBUFFER
       rotationBuffer.push_back(Quaternion(rotX, rotY, rotZ));
       if(rotationBuffer.size() > DEBUG_GYROREADBUFFER_MAXSIZE){rotationBuffer.erase(rotationBuffer.begin());}
     #endif
     
     rotation *= Quaternion(rotX, rotY, rotZ); //rotate with the measured amount
     rotation.setMagnitude(1);
   
     rotation.getEuler(yaw, roll, pitch);
   }
 }

 //placeholders

bool Gyro::isReady(){
   return true;
}

void Gyro::read(short& gyroX, short& gyroY, short& gyroZ){

}

