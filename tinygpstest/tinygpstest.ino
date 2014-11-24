#include "SoftwareSerial.h"
#include "TinyGPS++.h"
#include "LiquidCrystal.h"

TinyGPSPlus gps;
SoftwareSerial ss(6, 7);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int ledpin=13;
int val=0;
unsigned long last; 
const int batteryPin = 0; 
const double LONDON_LAT = 51.508131, LONDON_LON = -0.128002;


void setup()
{
  Serial.begin(28800);
  ss.begin(38400);
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.clear();
  lcd.print("GPS app starting...");
  pinMode(ledpin, OUTPUT);
  digitalWrite(ledpin,LOW);
  last=millis();
  lcd.clear(); 
}

// This custom version of delay() ensures that the gps object
// is being "fed".
static void smartDelay(unsigned long ms)
{
	unsigned long start = millis();
	do
	{
		while (ss.available()) gps.encode(ss.read());
	} while (millis() - start < ms);
}



void loop()
{	
	if (millis() >= last+5000)
	{
	  calculateBattery();		
	  last=millis();
	}
			
		smartDelay(1000);
		Serial.print("Soft Serial device overflowed? ");
		Serial.println(ss.overflow() ? "YES!" : "No");
		Serial.print("Sentences that failed checksum=");
		Serial.println(gps.failedChecksum());		
		Serial.print("Is valid? ");
		Serial.println(gps.location.isValid());

		lcd.setCursor(0,0);
		lcd.print(gps.time.hour()+1); // Hour (0-23) (u8)
		lcd.print(":");
		lcd.print(gps.time.minute()); // Minute (0-59) (u8)
		lcd.print(":");
		lcd.print(gps.time.second()); // Second (0-59) (u8)

		lcd.setCursor(10,0);
		lcd.print(gps.location.isValid());		
		
		if (gps.location.isValid())
		{
			digitalWrite(ledpin,HIGH);
			Serial.print("LAT=");  Serial.println(gps.location.lat(), 6);
			Serial.print("LONG="); Serial.println(gps.location.lng(), 6);

			unsigned long distanceKmToLondon =
			(unsigned long)TinyGPSPlus::distanceBetween(
			gps.location.lat(),
			gps.location.lng(),
			LONDON_LAT,
			LONDON_LON) / 1000;
			printInt(distanceKmToLondon, gps.location.isValid(), 9); 
		}
		else
		{
			digitalWrite(ledpin,LOW);
		}	
}

void calculateBattery()
{
	val = analogRead(batteryPin);
	float volts = val*(5/1023.0)*2.96; // calculate the ratio for 0 to 5 V
	lcd.setCursor(0,1);
	if (volts<=7.0)
	{
		lcd.print("Batt LOW.: ");
		lcd.print(volts);
		lcd.print("V");
	}
	else
	{
		lcd.print("Batt.: ");
		lcd.print(volts);
		lcd.print("V    ");
	}
}

static void printDateTime(TinyGPSDate &d, TinyGPSTime &t)
{
	if (!d.isValid())
	{
		Serial.print(F("********** "));
	}
	else
	{
		char sz[32];
		sprintf(sz, "%02d/%02d/%02d ", d.month(), d.day(), d.year());
		Serial.print(sz);
	}
	
	if (!t.isValid())
	{
		Serial.print(F("******** "));
	}
	else
	{
		char sz[32];
		sprintf(sz, "%02d:%02d:%02d ", t.hour(), t.minute(), t.second());
		Serial.print(sz);
	}

	printInt(d.age(), d.isValid(), 5);
	smartDelay(0);
}

static void printInt(unsigned long val, bool valid, int len)
{
	char sz[32] = "*****************";
	if (valid)
	sprintf(sz, "%ld", val);
	sz[len] = 0;
	for (int i=strlen(sz); i<len; ++i)
	sz[i] = ' ';
	if (len > 0)
	sz[len-1] = ' ';
	Serial.print(sz);
	smartDelay(0);
}