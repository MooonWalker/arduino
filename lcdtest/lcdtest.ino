/*********************

Example code for the Adafruit RGB Character LCD Shield and Library

This code displays text on the shield, and also reads the buttons on the keypad.
When a button is pressed, the backlight changes color.

Sárga drót INT1!!
Billentyûmegszakítás

SCL zöld drót
SDA fehér drót.

**********************/

// include the library code:
#include <Wire.h>
#include "Adafruit_MCP23017.h"
#include "Adafruit_RGBLCDShield.h"
#include "RTClib.h"


// The shield uses the I2C SCL and SDA pins. On classic Arduinos
// this is Analog 4 and 5 so you can't use those for analogRead() anymore
// However, you can connect other I2C sensors to the I2C bus and share
// the I2C bus.
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
RTC_DS1307 RTC;
// These #defines make it easy to set the backlight color
#define RED 0x1

volatile int isTriggered;
uint8_t buttons;

void setup() 
{
	// Debugging outputt
	//Serial.begin(9600);
	// set up the LCD's number of columns and rows:
	lcd.begin(20, 4);	
	RTC.begin();
	
	//This signal is active low, so HIGH-to-LOW when interrupt
	lcd.enableButtonInterrupt();

	lcd.setBacklight(RED);
	//lcd.blink();
	if (! RTC.isrunning())
	{
		lcd.println("RTC is NOT running!");
		// following line sets the RTC to the date & time this sketch was compiled
		RTC.adjust(DateTime(__DATE__, __TIME__));
	}
	//Catch the external interrupt from the button input
	attachInterrupt(1,ISR_Button,FALLING);
}


void loop() 
{
	DateTime now = RTC.now();
	
	// set the cursor to column 0, line 1
	// (note: line 1 is the second row, since counting begins with 0):
	lcd.setCursor(0, 0);
	lcd.print(now.year(), DEC);
	lcd.print('.');
	if (now.month()<10)
	{
		lcd.print("0");
		lcd.print(now.month(), DEC);
	} 
	else
	{
		lcd.print(now.month(), DEC);
	}
	
	lcd.print('.');
	if (now.day()<10)
	{	
		lcd.print("0");
		lcd.print(now.day(), DEC);
	} 
	else
	{
		lcd.print(now.day(), DEC);	
	}
	
	
	lcd.setCursor(0,1);
	if (now.hour()<10)
	{
		lcd.print(" ");
		lcd.print(now.hour(), DEC);
	} 
	else
	{
		lcd.print(now.hour(), DEC);
	}
	
	lcd.print(':');
	if (now.minute()<10)
	{
		lcd.print("0");
		lcd.print(now.minute(), DEC);
	} 
	else
	{
		lcd.print(now.minute(), DEC);
	}
	
	lcd.print(':');
	if (now.second()<10)
	{
		lcd.print("0");
		lcd.print(now.second(), DEC);
	} 
	else
	{
		lcd.print(now.second(), DEC);
	}
	// was there an interrupt?
	if (isTriggered)	handleKeypress();	 
	if (buttons) 
	{		
		//lcd.clear();
		lcd.setCursor(0,3);
		if (buttons & BUTTON_UP) 
		{
			lcd.print("UP        ");
			
		}
		if (buttons & BUTTON_DOWN) 
		{
			lcd.print("DOWN      ");
			//lcd.setBacklight(YELLOW);
		}
		if (buttons & BUTTON_LEFT) 
		{
			lcd.print("LEFT      ");
			//lcd.setBacklight(GREEN);
		}
		if (buttons & BUTTON_RIGHT) 
		{
			lcd.print("RIGHT ");
			//lcd.setBacklight(TEAL);
		}
		if (buttons & BUTTON_SELECT) 
		{
			lcd.print("SELECT    ");
			//lcd.setBacklight(VIOLET);
		}
	}
}

//ISR to service the button interrupt
void ISR_Button() 
{
	isTriggered = true; //Also flag we triggered	
}

void handleKeypress()
{
	isTriggered=false;
	buttons = lcd.readButtons();
}