#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include "TinyGPS++.h"

#define BUFFERLENGTH 200

TinyGPSPlus gps;
SoftwareSerial ss(6, 7);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int ledpin =13;
char commandoGPR[7] = "$GPRMC"; 
char commandoGGA[7] = "$GPGGA";
char floatbuf[32]="";
char line[BUFFERLENGTH]="";
boolean newline=false;
int counter =0;
int idxcntr =0;
int chkGPRMC=0, chkGPGGA=0;
String sentenceRMC="", sentenceGGA="";
String utcRMC="";
String status="";
int stringIndices[40];
String lat="", lon="";
float fLat=0, fLon=0;

void setup()
{
  Serial.begin(28800);
  ss.begin(57600);
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.clear();
  lcd.print("GPS app starting...");
  delay(300);
  pinMode(ledpin, OUTPUT);
  digitalWrite(ledpin,LOW);
  clearLine();
  
  lcd.clear();
}

void clearLine()
{
	for (int i=0;i< BUFFERLENGTH ;i++)
	{       // Initialize a buffer for received data
		line[i]=' ';
	}

	for (int i=0;i<40;i++)
	{
		stringIndices[i]=0;
	}
	
}

float conv_coords(float in_coords)
{
	//Initialize the location.
	float f = in_coords;
	// Get the first two digits by turning f into an integer, then doing an integer divide by 100;
	// firsttowdigits should be 77 at this point.
	int firsttwodigits = ((int)f)/100;                               //This assumes that f < 10000.
	float nexttwodigits = f - (float)(firsttwodigits*100);
	float theFinalAnswer = (float)(firsttwodigits + nexttwodigits/60.0);
	return theFinalAnswer;
}

void loop()
{
  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())
    {
      char c = ss.read();
       //Serial.print(c);// uncomment this line if you want to see the GPS data flowing
	   //lcd.write(c);	

	   if(c==36)		// $ ARRIVED
	   {
			newline=true;
			clearLine();	//ERASE line buffer and string indizes
			counter=0;
			idxcntr=0;
			chkGPRMC=0;
			chkGPGGA=0;
			sentenceRMC="";
			sentenceGGA="";
			//digitalWrite(ledpin,LOW);
	   }

	    if(newline)
	    {
		    line[counter]=c;
		    counter++;
	    }

	   if(c=='*')  // END of line
	   {
			newline=false;	

			for (int i=0;i<6;i++)
			{     
				if(line[i]==commandoGPR[i])
				{
					chkGPRMC++;
				}

				if (line[i]==commandoGGA[i])
				{
					chkGPGGA++;
				}
				//lcd.write(line[i]);
			}

			if (chkGPGGA==6)
			{
				Serial.println("GGA");
				for (int j=7;j<counter;j++)
				{
					sentenceGGA += line[j];
					if (line[j]==',')
					{
						stringIndices[idxcntr]=j-7;
						idxcntr++;
					}
				}
				Serial.print("GGA: ");
				Serial.println(sentenceGGA);
			}	
			if (chkGPRMC==6)
			{
				//digitalWrite(ledpin,HIGH);
				for (int z=7;z<counter;z++)
				{  
					sentenceRMC += line[z];
					if (line[z]==',')
					{
						stringIndices[idxcntr]=z-7;
						idxcntr++;
					}
				}								
				utcRMC=sentenceRMC.substring(0,stringIndices[0]-3);
				status=sentenceRMC.substring(stringIndices[0]+1,stringIndices[1]);
				lat=sentenceRMC.substring(stringIndices[1]+1,stringIndices[2]);
				lon=sentenceRMC.substring(stringIndices[3]+1,stringIndices[4]);
				lat.toCharArray(floatbuf, sizeof(floatbuf));
				fLat= atof(floatbuf);
				fLat=conv_coords(fLat);

				for (int k=0;k<sizeof(floatbuf);k++)
				{
					floatbuf[k]=' ';
				}

				lon.toCharArray(floatbuf, sizeof(floatbuf));
				fLon=atof(floatbuf);
				fLon=conv_coords(fLon);
				Serial.print("RMC: ");
				Serial.println(sentenceRMC);
				//lcd.setCursor(0,0);
				//lcd.print(utcRMC);					
				lcd.setCursor(0,0);
				lcd.print(fLat,5);
				lcd.setCursor(0,1);
				lcd.print(fLon,5);

				Serial.print("lat: ");
				Serial.println(lat);
				Serial.print("lon: ");
				Serial.println(lon);

			}							
	   }	  
    }
  }

  if (status=="V")
  {
	  digitalWrite(ledpin,LOW);
  }
  else
  {
	  digitalWrite(ledpin, HIGH);
  }

  Serial.println(status);
  lcd.setCursor(12,1);
  lcd.print(status);
//delay(500);
  
  
  
}