  //These displays use I2C to communicate, 2 pins are required to 
  //interface. There are multiple selectable I2C addresses. For backpacks
  //with 2 Address Select pins: 0x70, 0x71, 0x72 or 0x73. For backpacks
  //with 3 Address Select pins: 0x70 thru 0x77
//
  //Adafruit invests time and resources providing this open source code, 
  //please support Adafruit and open-source hardware by purchasing 
  //products from Adafruit!
//
  //Written by Limor Fried/Ladyada for Adafruit Industries.  
  //BSD license, all text above must be included in any redistribution
 //****************************************************/

#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"
#include "RTClib.h"

Adafruit_7segment matrix = Adafruit_7segment();
RTC_DS1307 RTC;

void setup() 
{
	
	RTC.begin();
	matrix.begin(0x70);
	matrix.setBrightness(4);
}

void loop() 
{
	DateTime now = RTC.now(); 
	static int oldPerc = 0;
	
if (oldPerc!=now.minute())
{
	
	oldPerc=now.minute();
	
	if (now.hour()<10)
	{
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
