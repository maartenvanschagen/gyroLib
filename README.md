# gyroLib V0.3 (beta)
'gyroLib' is an easy to use modular gyro library, first created for the 'ESP32 Quadcopter' project (https://github.com/maartenvanschagen/ESP32-Quadcopter).
At this moment gyroLib is a multi-platform library with support for the devices found on the GY-80 board.

## Supported platforms
The gyroLib library can run on multiple platforms. Platforms that are supported at this moment are:
- Arduino
- esp-idf
- avr

## Supported sensors
At this moment, sensor support is a bit sparse. However, support for more sensors is on the way! At this moment it supports the following sensors:
- L3G4200D
- ADXL345
Support for magnetometers and barometers is also being worked on.

## Supported IDE's
This version of the library is fully tested in the Arduino IDE. It should also run on Platform.io. However, this hasn't been tested. Full support for Platform.io should be here by version 1.0.

## Roadmap to Release
Are there any features you would like to be added? Please create an issue on GitHub! Feel free to ask for any features you require! Even if it's on the roadmap already!
#### Version 0.4
Version 0.4 will add full support for magnetometers and barometers. These sensors will help make gyroscope sensor-readings more accurate. Also, when a magnetometer is installed you will be able to fix the yaw zero point to the magnetic north.
#### Version 0.5
Some backend optimizations will be done to get everything ready for version 0.6. I2C pins used on the ESP32 should, for example, be configurable. And, when rotation calculations are not used, all floating-point operations should be optimized out by the compiler. This will make for significantly smaller program sizes when using 8-bit processors.
#### Version 0.6
Documentation should be available after the release of version 0.6. This will also come with some ease-of-use optimizations. Every platform will also be getting a few examples.
#### Version 1.0
Version 1.0 marks the official release of the library. Full support for every sensor on the GY-80 and GY-521 boards should be here! Documentation will be polished.
#### Version 2.0
Version 2.0 will allow for other libraries to built upon the basic functionality of this library. This means third-party libraries will be able to add both sensor and platform support. We will also look into the feasibility of custom filters and calculations.