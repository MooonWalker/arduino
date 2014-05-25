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


// The shield uses the I2C SCL and SDA pins. On classic Arduinos
// this is Analog 4 and 5 so you can't use those for analogRead() anymore
// However, you can connect other I2C sensors to the I2C bus and share
// the I2C bus.
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
// These #defines make it easy to set the backlight color
#define RED 0x1

volatile int isTriggered;
uint8_t buttons;

void setup()
{
	// Debugging outputt
	//Serial.begin(9600);
	// set up the LCD's number of columns and rows:
	lcd.begin(16, 2);
	
	//This signal is active low, so HIGH-to-LOW when interrupt
	lcd.enableButtonInterrupt();

	lcd.setBacklight(RED);
	//lcd.blink();
	
	//Catch the external interrupt from the button input
	attachInterrupt(1,ISR_Button,FALLING);
}


void loop()
{
	// set the cursor to column 0, line 1
	// (note: line 1 is the second row, since counting begins with 0):
	lcd.setCursor(0, 0);
	lcd.print("kaka");
	// was there an interrupt?
	if (isTriggered)	handleKeypress();
	if (buttons)
	{
		//lcd.clear();
		lcd.setCursor(0,1);
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