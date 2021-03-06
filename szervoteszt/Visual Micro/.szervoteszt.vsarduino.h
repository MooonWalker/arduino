/* 
	Editor: http://www.visualmicro.com
	        arduino debugger, visual micro +, free forum and wiki
	
	Hardware: ATmega328 on a breadboard (8 MHz internal clock), Platform=avr, Package=arduino
*/

#define __AVR_ATmega328p__
#define __AVR_ATmega328P__
#define ARDUINO 101
#define ARDUINO_MAIN
#define F_CPU 8000000L
#define __AVR__
#define __cplusplus
extern "C" void __cxa_pure_virtual() {;}

//
//
void balServo(signed char velocity);
void jobbServo(signed char velocity);
void doSonarping();
void doVoltmeasure();
long readVcc();

#include "C:\Arduino\hardware\arduino\variants\standard\pins_arduino.h" 
#include "C:\Arduino\hardware\arduino\cores\arduino\arduino.h"
#include "C:\DATA\Dropbox\arduino\szervoteszt\szervoteszt.ino"
