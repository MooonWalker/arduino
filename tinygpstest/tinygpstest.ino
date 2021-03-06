#include "SoftwareSerial.h"
#include "TinyGPS++.h"
#include "LiquidCrystal.h"


TinyGPSPlus gps;
SoftwareSerial ss(6, 7);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int ledpin=13;
unsigned long battery_last, distance_last; // timers
const int batteryPin = 0; 
double old_LAT = 51.508131, old_LON = 19.128618;
int totalDistance=0;
char once=0;

enum STATE {NOFIX, BATT_LOW, FIX}; 
STATE actualState = NOFIX;
STATE prevState = NOFIX;

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
  battery_last=millis();
  distance_last=millis();
  lcd.setCursor(0,1);
  lcd.print(analogRead(batteryPin)*(5/1023.0)*2.96);
  delay(500);
  lcd.clear(); 
}

void loop()
{
  if (millis() >= battery_last+5000)
  {
	calculateBattery();
	battery_last=millis();
  }	

  smartDelay(1000); // feed the GPS ojjekt
  if (gps.location.isValid())
  {
	
	if (gps.location.age()>1500)
	{
		actualState=NOFIX;
	}
	else
	{
	  actualState=FIX;
	}
  }
  else
  {
	  actualState=NOFIX;
  }
  switch(actualState)	//state machine
  {
    case NOFIX:
	  digitalWrite(ledpin,LOW);
	  lcd.clear();
	  lcd.setCursor(0,0);
	  lcdprintTime(gps.time, 1);  // UTC +1
	break;

	case FIX:
	{
	  digitalWrite(ledpin,HIGH);
	   Serial.print("LAT=");  Serial.println(gps.location.lat(), 6);
	   Serial.print("LONG="); Serial.println(gps.location.lng(), 6);
	  
	  if (!once)
	  {
		old_LAT=gps.location.lat();
		old_LON=gps.location.lng();
		once=1;
	  }
	  
	  lcd.setCursor(0,1);
	  lcd.print("Sats:");
	  lcd.print(gps.satellites.value());
	  lcd.print("HDOP:");
	  lcd.print(gps.hdop.value());
	 
	  if (millis() >= distance_last+1000)
	  {
		unsigned long distanceKmToOld =0; // meters
		distanceKmToOld  =(unsigned long)gps.distanceBetween(
		gps.location.lat(),	gps.location.lng(), old_LAT, old_LON);
		printInt(distanceKmToOld, gps.location.isValid(), 9);  
		totalDistance = totalDistance+distanceKmToOld;
		lcd.setCursor(0,0);
		lcd.print("        ");
		lcd.setCursor(0,0);
		lcd.print(totalDistance);
		lcd.print("m");
		old_LAT=gps.location.lat();
		old_LON=gps.location.lng();
		distance_last=millis(); // timer reset
	  }  
	}// end case FIX
	break;

	case BATT_LOW:
	  lcd.clear();
	  lcd.setCursor(0,1);
	  lcd.print("Batt LOW!: ");
	break;
  } // end switch
			
	Serial.print("Soft Serial device overflowed? ");
	Serial.println(ss.overflow() ? "YES!" : "No");
	Serial.print("Sentences that failed checksum=");
	Serial.println(gps.failedChecksum());		
	Serial.print("Is valid? ");
	Serial.println(gps.location.isValid());
} // end Loop

void calculateBattery()
{
	float volt=calculateVolts();
	
	lcd.setCursor(0,1);
	if (volt<10) // 2 cells LIPO
	{
	  if (volt<=7.0)
	  {
		  lcd.print(volt);
		  lcd.print("V");
		  actualState = BATT_LOW;
	  }
	  else
	  {
		  //lcd.print("Batt.: ");
		  //lcd.print(volt);
		  //lcd.print("V    ");
	  }
	} 
	else  // 3 cells LIPO
	{
	  if (volt<=10.5)
	  {  
		  lcd.print(volt);
		  lcd.print("V");
		  actualState = BATT_LOW;
	  }
	  else
	  {
		  //lcd.print("Batt.: ");
		  //lcd.print(volt);
		  //lcd.print("V    ");
	  }
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

static void lcdprintTime(TinyGPSTime &t, int zone)
{	
	if (!t.isValid())
	{
		lcd.print(F("******** "));
	}
	else
	{
		char sz[32];
		sprintf(sz, "%02d:%02d:%02d ", t.hour()+zone, t.minute(), t.second());  
		lcd.print(sz);
	}
	smartDelay(0);
}
static void printInt(unsigned long val, bool valid, int len)
{
	char sz[32] = "*****************";
	if (valid) sprintf(sz, "%ld", val);
	sz[len] = 0;
	for (int i=strlen(sz); i<len; ++i)
	sz[i] = ' ';
	if (len > 0)
	sz[len-1] = ' ';
	Serial.print(sz);
	smartDelay(0);
}

float calculateVolts()
{
	int val = analogRead(batteryPin);
	float volts = val*(5/1023.0)*2.96; // calculate the ratio for 0 to 5 V
	return volts;
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
