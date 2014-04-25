//Board = Arduino Duemilanove w/ ATmega328
#define __AVR_ATmega328P__
#define ARDUINO 104
#define F_CPU 16000000L
#define __AVR__
extern "C" void __cxa_pure_virtual() {;}

//
//
byte DecToBcd(byte val);
byte BcdToDec(byte val);
int getTemperature();
int getTemperature2();
void formatTemp(int Temp, boolean Sensor);
void expanderWrite(const byte reg, const byte data );
unsigned int expanderRead(const byte reg);
void keypress();
void handleKeypress();
void writeMaxMin();
void writeWrites();

#include "C:\arduino-1.0.4windows\hardware\arduino\variants\standard\pins_arduino.h" 
#include "C:\arduino-1.0.4windows\hardware\arduino\cores\arduino\arduino.h"
#include "C:\DATA\Dropbox\arduino\veglegeshomero1\veglegeshomero1.pde"
