#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

SoftwareSerial ss(6, 7);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int ledpin =13;
char commandoGPR[7] = "$GPRMC"; 
char commandoGGA[7] = "$GPGGA";
char line[320]="";
boolean newline=false;
int counter =0;
int idxcntr =0;
int chkGPRMC=0, chkGPGGA=0;
String sentenceRMC="", sentenceGGA="";
String utcRMC="";
String status="";
int stringIndices[50];
String lat="", lon="";

void setup()
{
  Serial.begin(19200);
  ss.begin(38400);
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.clear();
  lcd.print("GPS app starting...");
  delay(200);
  pinMode(ledpin, OUTPUT);
  digitalWrite(ledpin,LOW);
  clearLine();
  
  lcd.clear();
}

void clearLine()
{
	for (int i=0;i<320;i++)
	{       // Initialize a buffer for received data
		line[i]=' ';
	}

	for (int i=0;i<50;i++)
	{
		stringIndices[i]=0;
	}
	
}


void loop()
{
  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())
    {
      char c = ss.read();
       //Serial.write(c); // uncomment this line if you want to see the GPS data flowing
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

	   if(c==13)  // END of line
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
				
			if (chkGPRMC==6)
			{
				//digitalWrite(ledpin,HIGH);
				for (int i=7;i<counter;i++)
				{  
					sentenceRMC += line[i];

					if (line[i]==',')
					{
						stringIndices[idxcntr]=i-7;
						idxcntr++;
					}
				}								
				utcRMC=sentenceRMC.substring(0,stringIndices[0]-3);
				status=sentenceRMC.substring(stringIndices[0]+1,stringIndices[1]);
				lat=sentenceRMC.substring(stringIndices[1]+1,stringIndices[2]);
				lon=sentenceRMC.substring(stringIndices[3]+1,stringIndices[4]);

				Serial.print("RMC: ");
				Serial.println(sentenceRMC);
				//lcd.setCursor(0,0);
				//lcd.print(utcRMC);					
				lcd.setCursor(0,0);
				lcd.print(lat);
				lcd.setCursor(0,1);
				lcd.print(lon);

				Serial.print("lat: ");
				Serial.println(lat);
				Serial.print("lon: ");
				Serial.println(lon);
			}

			if (chkGPGGA==6)
			{
				Serial.println("GGA");
				//for (int i=7;i<counter;i++)
				//{
					//sentenceGGA += line[i];
				//
					//if (line[i]==',')
					//{
						//stringIndices[idxcntr]=i-7;
						//idxcntr++;
					//}
				//}
				
			}
							
	   }	  
    }
  }

  //delay(500);
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
}