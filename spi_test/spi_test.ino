#include "SPI.h"
#define SS 48  //Slaveselect
#define DATAOUT 51 //MOSI
#define DATAIN 50 //MISO
#define SPICLOCK 52 //SCK
 char resp=0;

void setup()
{
  Serial.begin(9600);
  pinMode(SS, OUTPUT);
  pinMode(52, OUTPUT);
  pinMode(51, OUTPUT);
  pinMode(50, INPUT);
  
  bitSet(DDRG, 1);
  digitalWrite(40, LOW);
  TCCR5A=0;
  TCCR5B =1<<WGM53 | 1<<WGM52 | 1<<CS52 |1<<CS50;
  ICR5=7812; // 1Hz
  bitSet(TIMSK5, ICIE5);

  digitalWrite(SS, HIGH);
  SPI.setDataMode(SPI_MODE3); //Clock pol 1, phase 1
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  SPI.begin ();

}

ISR(TIMER5_CAPT_vect)
{
	bitSet(PING, 1);
	sendHearthBeat();
}
byte transferAndWait (const byte what)
{
	byte r = SPI.transfer (what);
	delayMicroseconds (20);
	return r;
} // end of transferAndWait

void loop()
{
  
}

void sendHearthBeat()
{
	digitalWrite(SS, LOW);    // SS is pin 2
	
	// send test string
	transferAndWait('h');

	// disable Slave Select
	digitalWrite(SS, HIGH);
	
}

