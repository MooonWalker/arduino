
#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"
#include "RTClib.h"

#define encoderPinA  19
#define encoderPinB  18

int pulses, A_SIG=0, B_SIG=1;

long debouncing_time = 7; //ms
volatile unsigned long last_microsA;
volatile unsigned long last_microsB;
//volatile unsigned int encoderPos=0;
volatile unsigned int brightness=0;
volatile boolean rotaryChanged=0;

Adafruit_7segment matrix = Adafruit_7segment();
RTC_DS1307 RTC;

void setup() 
{
	//Serial.begin(9600);
	//Serial.println("Start...");
	delay(10);
	pinMode(encoderPinA, INPUT);
	pinMode(encoderPinB, INPUT);
	digitalWrite(encoderPinA, HIGH);
	digitalWrite(encoderPinB, HIGH);
	
	delay(5);
	attachInterrupt(4, doEncoderARise, RISING);
	attachInterrupt(5, doEncoderBRise, RISING);
	
	RTC.begin();
	matrix.begin(0x70);
	matrix.setBrightness(brightness);	
	delay(5);
}

void loop() 
{
	DateTime now = RTC.now(); 
	static int oldPerc = 0;
	
if (rotaryChanged)
{
	matrix.setBrightness(brightness);
	rotaryChanged=false;
	delay(1);
}

if (oldPerc!=now.minute())
{
	
	oldPerc=now.minute();
	
	if (now.hour()<10)
	{
		matrix.clear();
		matrix.writeDigitNum(1, now.hour(), 0);
	}
	else
	{
		matrix.writeDigitNum(0, now.hour()/10, 0);
		matrix.writeDigitNum(1, now.hour() % 10, 0);
	}

	matrix.drawColon(1);

	if (now.minute()<10)
	{
		matrix.writeDigitNum(3,0,0);
		matrix.writeDigitNum(4,now.minute(),0);
	}
	else
	{
		matrix.writeDigitNum(3,now.minute()/10,0);
		matrix.writeDigitNum(4,now.minute()%10,0);
	}

	matrix.writeDisplay();
	
  } //endif
} 

void doEncoderARise()
{
	  // look for a low-to-high on channel A
	   if((long)(micros() - last_microsA) >= debouncing_time * 1000) 
	   {
		rotaryChanged=true;
		detachInterrupt(4);
		A_SIG=1;
		if(B_SIG==0)
		{
			
			pulses++;//moving forward
			if (brightness<15)
			{
				brightness++;
				//Serial.print("BrightnessA: ");
				//Serial.println(brightness);
				
			}
		}
		if(B_SIG==1)
		{
			pulses--;//moving reverse
			
			
		}
		//Serial.println(pulses);
		
		attachInterrupt(4,doEncoderAFall,FALLING);
		   
		last_microsA = micros();
	   }
	  
}

void doEncoderAFall()
{
	 if((long)(micros() - last_microsA) >= debouncing_time * 1000) 
	 {
		detachInterrupt(4);
		A_SIG=0;
		if(B_SIG==1)
		pulses++;//moving forward
		if(B_SIG==0)
		{
			pulses--;//moving reverse
			if (brightness>0)
			{
				brightness--;
				//Serial.print("BrightnessA: ");
				//Serial.println(brightness);
			}
		}
		//Serial.println(pulses);
		delay(5);
		attachInterrupt(4,doEncoderARise, RISING);	
		 last_microsA = micros();
	 }
	
}

void doEncoderBRise()
{
	 if((long)(micros() - last_microsB) >= debouncing_time * 1000) 
	 {
	detachInterrupt(5);
	B_SIG=1;
	
	if(A_SIG==1)
	pulses++;//moving forward
	if(A_SIG==0)
	pulses--;//moving reverse
	//Serial.println(pulses);
	delay(5);
	attachInterrupt(5, doEncoderBFall, FALLING);
		 last_microsB = micros();
	 }
	
}

void doEncoderBFall()
{
	 if((long)(micros() - last_microsB) >= debouncing_time * 1000) 
	 {
		detachInterrupt(5);
		B_SIG=0;
		
		if(A_SIG==0)
		pulses++;//moving forward
		if(A_SIG==1)
		pulses--;//moving reverse
		//Serial.println(pulses);
		delay(5);
		attachInterrupt(5, doEncoderBRise, RISING);
		 last_microsB = micros();
	 }
	
}
