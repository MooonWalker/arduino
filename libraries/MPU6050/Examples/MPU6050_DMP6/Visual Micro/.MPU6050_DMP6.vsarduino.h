//Board = Arduino Duemilanove w/ ATmega328
#define __AVR_ATmega328P__
#define ARDUINO 104
#define F_CPU 16000000L
#define __AVR__
extern "C" void __cxa_pure_virtual() {;}

void dmpDataReady();
//
//

#include "C:\arduino-1.0.4windows\hardware\arduino\variants\standard\pins_arduino.h" 
#include "C:\arduino-1.0.4windows\hardware\arduino\cores\arduino\arduino.h"
#include "C:\arduino-1.0.4windows\libraries\MPU6050\Examples\MPU6050_DMP6\MPU6050_DMP6.ino"
