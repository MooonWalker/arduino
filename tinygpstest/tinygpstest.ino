#include "SoftwareSerial.h"
#include "TinyGPS++.h"
#include "LiquidCrystal.h"

#define BUFFERLENGTH 200

TinyGPSPlus gps;
SoftwareSerial ss(6, 7);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int ledpin=13;
unsigned long last = 0UL; 
const int batteryPin = 0; 
const float referenceVolts = 5;
const float resistorFactor = 1023.0 / (10/(10 + 10)); 

void setup()
{

  Serial.begin(28800);
  ss.begin(38400);
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.clear();
  lcd.print("GPS app starting...");
  delay(300);
  pinMode(ledpin, OUTPUT);
  digitalWrite(ledpin,LOW);
  last=millis();
  lcd.clear(); //törlés

  cli();
	//set timer1 interrupt at 1Hz
	TCCR1A = 0;// set entire TCCR1A register to 0
	TCCR1B = 0;// same for TCCR1B
	TCNT1  = 0;//initialize counter value to 0
	// set compare match register for 1hz increments
	OCR1A = 31249;// = ((16*10^6) / (2*1024)) - 1 (must be <65536)
	// turn on CTC mode
	TCCR1B |= (1 << WGM12);
	// Set CS10 and CS12 bits for 1024 prescaler
	TCCR1B |= (1 << CS12) | (1 << CS10);
	// enable timer compare interrupt
	TIMSK1 |= (1 << OCIE1A);
  sei();
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

ISR(TIMER1_COMPA_vect)
{  //change the 0 to 1 for timer1 and 2 for timer2
	//interrupt commands here
}

void loop()
{

	
		int val = analogRead(batteryPin);
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

		float volts = val*(5/1023.0)*2.96; // calculate the ratio for 0 to 5 V
		Serial.print("Voltage: ");
		Serial.println(volts);

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
			lcd.print("V");
		}
		smartDelay(1000);
		
		if (gps.location.isValid())
		{
			digitalWrite(ledpin,HIGH);
		}
		else
		{
			digitalWrite(ledpin,LOW);
		}
	
}
