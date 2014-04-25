#include <Wire.h>
#include <cLCD.h>
#include <OneWire.h>
#include <Rtc_Pcf8563.h>
#include <SD.h>

#define eport B0100000	// address = 000;  7 bits!

#define iocon 0x0A
#define iodira 0x00
#define gpioa 0x12
#define gppua 0x0C	//pullups
#define gpintena 0x04
#define defvala 0x06
#define intcona 0x08
#define intfa 0x0E
#define intcapa 0x10
#define ipola 0x02

#define ERROR_LED 4
#define ERROR_RTC_GET 2 // Unable to get RTC time and date 


//init the real time clock
Rtc_Pcf8563 rtc;
OneWire  ds(3);  // DS1820 on pin 3 Measures temperatures from –55°C to +125°C in
                 //0.5°C increments.
cLCD lcd(B0100000); //banked operation, BANK 1, GPIOB all outputs

// Global variables

volatile boolean menuflag = false;
volatile boolean keypressed = false;


//const double period = 0x36EE80;   //kesleltetes 1 ora
const double period = 0x6DDD00;   //kesleltetes 2 ora
boolean first;
byte I2c_lcd_b;
byte result;
byte addr[2][8];
int hour;

String dataString = "";
String sWhole = "";
String sFract = "";
String sDate = "";
String sTime ="";

const int chipSelect = 8;
unsigned long oldmillis, currmillis, timer1, interv;
int millisoverflow = 0;
float minT, maxT, decT;
int lightval;
int Temperature = 0;
int SignBit, Tc_100, Whole, Fract;
float tfract;

void setup()
{
  minT=100;
  maxT=0;
  currmillis=millis();
  oldmillis=currmillis;
  timer1=currmillis;
  interv=2500;
  
  first = true;
  String message="";
  pinMode(ERROR_LED, OUTPUT); // Set error LED 
  
  Serial.begin(9600);
  Wire.begin();
  ds.search(addr[0]);
  ds.search(addr[1]);
//  expanderWrite(0x0B, B10000000); //IOCONZ, BANK1  
                                  // valid command only if BANK = 0, which is true upon reset
                                  // set BANK = 1 if it had been 0 (nothing happens if BANK = 1 already)
  expanderWrite(0x06, 0xff); //GPPUA                               
  expanderWrite(0x01, 0xff); //IPOLA                               
  expanderWrite(0x02, 0xff); //GPINTENA
  keypressed=false;
  sei(); //enable interrupts globally
  attachInterrupt(0, keypress, CHANGE);
  delay(100);
  //expanderRead(INTCAPA); //INTCAPA   
  
  
  
  lcd.begin(16, 2);
  //lcd.backlight();
  lcd.setCursor(0,0);
 
  pinMode(8, OUTPUT);
  
  if (!SD.begin(8)) 
  {
//  Serial.println("Card failed, or not present");
    lcd.print("Card failed!");
    // don't do anything more:
    return;
  }
    
 
  lcd.print("Card initialized.");
  
  File logFile = SD.open("errorlog.txt", FILE_WRITE);
  if (logFile) 
  { 
    message = String(rtc.formatDate(RTCC_DATE_ASIA)) + " " + String(rtc.formatTime());
    logFile.println("Program started: " + message);
    logFile.close();
//  Serial.println("Program started: "  + message);

  }  
  // if the file isn't open, pop up an error:
  else 
  {
//    Serial.println("error opening errorlog.txt");
  } 
 
}


void loop()
{
  if(keypressed)
  {
    handleKeypress();       
  }
  
  hour=rtc.getHour();  
  if(hour > 6 && hour < 23)
  {
    lcd.backlight();
  }
  else
  {
     lcd.noBacklight();
  }
//  lightval= analogRead(1);
//  lcd.noBacklight();
//  if (lightval >= 20)
//  {
//    lcd.backlight();
//  }
  
    Temperature = getTemperature(); //external sensor
//    Temperature = getTemperature2(); //internal sensor
  formatTemp(Temperature, 0); //External sensor
//    formatTemp(Temperature, 1); //Internal sensor
 
   sTime=rtc.formatTime(RTCC_TIME_HM);   //read formatted time from i2C RTC
   sDate=rtc.formatDate(RTCC_DATE_ASIA); //read formatted date from 12C RTC
    
   minT = min(minT, decT);
   maxT = max(maxT, decT);           
  
if(first || millis()-timer1 >=interv)
  {
    if(interv==700UL)
    {
      lcd.clear();
      lcd.print("Temp.: ");
//    Serial.print(millis()-timer2);
//    Serial.println();
      if (SignBit) // If its negative
      {  
  //     Serial.print("-");
         lcd.print("-");
      }      
  //  Serial.println();
  //  Serial.print("Rawtemp: ");
  //  Serial.print(Temperature);
  //  Serial.println();
  //
  //  Serial.print(Whole);  
      lcd.print(Whole);    
    //Serial.print(".");
      lcd.print(".");
    
      if (Fract < 10)
      {
      // Serial.print("0");
       lcd.print("0");
      }
    //Serial.print(Fract);
      lcd.print(Fract);    
    //Serial.print("Celsius");
      lcd.print(char(178));
      lcd.print("C");   
   // Serial.print("\n");   
  //  Serial.print(sTime);
  //  Serial.print(" ");
  //  Serial.print(sDate);
  //  
  //  Serial.println();
    
      lcd.setCursor(0,1);
      lcd.print("Time : ");
      lcd.print(sTime);
  
        
      interv=1600UL;        

      timer1=millis();    
    }  
    else if(interv==2500UL)
    {
        writeMaxMin(); 
        interv=700UL;
        timer1=millis();
    }
    
    if(interv==1600UL) interv=2500UL;
    
  }
      
  if(keypressed)
  {
    handleKeypress();       
  }
  
  
// writeWrites(); //Display logged data counter

 
//----------------------------------------Preparing the data for logging  
if (first || currmillis >= (oldmillis + period) || (currmillis+period)<= (oldmillis/2))
{ 
   first = false;
   millisoverflow=millisoverflow+1;
   dataString += String(sWhole + "." + sFract + ";" + sDate + ";" + sTime + ";");
     
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  delay(50);
  
  if (dataFile) 
  {
    dataFile.println(dataString);
    dataFile.close();
    
   //  Serial.println(dataString);
  }  
  // if the file isn't open, pop up an error:
  else 
  {
  //Serial.println("error opening datalog.txt");
  } 

  oldmillis = currmillis;
 } // endif
  currmillis=millis();
} // END LOOP
//**************************************************************************************

// Convert normal decimal numbers to binary coded decimal
byte DecToBcd(byte val)
{
  return (val / 10 * 16 + val % 10); 
}

// Convert binary coded decimal to normal decimal numbers
byte BcdToDec(byte val)
{
  return ((val / 16) * 10 + val % 16); 
}
//****************************************************************************

//**************************************************************************
int getTemperature() //gets the actual Temperature from 1-wire interface
{
  byte i;
  byte present = 0;
  byte data[12];
 

  int HighByte, LowByte, TReading;
  digitalWrite(13, LOW); 
  
 
//  Serial.print(addr[0][0], HEX);
//  Serial.println();
//  Serial.print(addr[1][0], HEX);
//  Serial.println();
  
  ds.reset();
  ds.select(addr[0]);
  ds.write(0x44,1);         // start conversion, with parasite power on at the end
//  
  delay(750);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  
  present = ds.reset();
  ds.select(addr[0]);    
  ds.write(0xBE);         // Read Scratchpad
  
  for ( i = 0; i < 9; i++) 
   {           // we need 9 bytes
    data[i] = ds.read();
//    Serial.print("homero: ");
//    
//    Serial.print(data[i], HEX);
//    Serial.print(" ");
   }
   
  LowByte = data[0];
  HighByte = data[1];
  TReading = (HighByte << 8) + LowByte;
  
  return TReading;
  
}

int getTemperature2() //gets the actual Temperature from 1-wire interface
{
  byte i;
  byte present = 0;
  byte data[13];
 

  int HighByte, LowByte, TReading;
  digitalWrite(13, LOW); 
  
 
//  Serial.print(addr[0][0], HEX);
//  Serial.println();
//  Serial.print(addr[1][0], HEX);
//  Serial.println();
  
  ds.reset();
  ds.select(addr[1]);
  ds.write(0x44,1);         // start conversion, with parasite power on at the end
//  
  delay(750);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  
  present = ds.reset();
  ds.select(addr[1]);    
  ds.write(0xBE);         // Read Scratchpad
  
  for ( i = 0; i < 9; i++) 
   {           // we need 9 bytes
    data[i] = ds.read();
//    Serial.print("homero: ");
//    
//    Serial.print(data[i], HEX);
//    Serial.print(" ");
   }
   
  LowByte = data[0];
  HighByte = data[1];
  TReading = (HighByte << 8) + LowByte;
 

  return TReading;
  
}

void formatTemp(int Temp, boolean Sensor)
{
  if (Sensor==0)
  {
    SignBit = Temp & 0x8000;  // test most sig bit
  
    if (SignBit) // negative
    {
      Temp = (Temp ^ 0xffff) + 1; // 2's comp
    }
  
    Tc_100 = (Temp*100/2);  
    Whole = Tc_100 / 100;  // separate off the whole and fractional portions
    Fract = Tc_100 % 100;
    tfract = (float)(Fract);
    decT = Whole + tfract/100;
      if (SignBit) // If its negative
      {  
         decT = decT * -1;
         sWhole= "-" + String(Whole);    
      }
      else
      {
         sWhole=String(Whole);    
      }

     sFract=String(Fract);
    
  }
  else if(Sensor==1)
  { 
     SignBit = Temp & 0x8000;  // test most sig bit
    if (SignBit) // negative
    {
      Temp = (Temp ^ 0xffff) + 1; // 2's comp
    }
    Tc_100 = (6 * Temp) + Temp / 4;    // multiply by (100 * 0.0625) or 6.25
  
    Whole = Tc_100 / 100;  // separate off the whole and fractional portions
    Fract = Tc_100 % 100;
    tfract = (float)(Fract);
    decT = Whole + tfract/100;
    
    
  
    if (SignBit) // If its negative
    {
        decT = decT * -1;
        sWhole= "-" + String(Whole); 
    }
    else
    {
      sWhole=String(Whole);
    }
    sFract=String(Fract);
   
  }
}
//*************************************************************************
void expanderWrite(const byte reg, const byte data ) 
{
  Wire.beginTransmission(eport);
  Wire.send (reg);
  Wire.send (data);  
  Wire.endTransmission ();
} 

unsigned int expanderRead(const byte reg) 
{
  Wire.beginTransmission (eport);
  Wire.send (reg);
  Wire.endTransmission ();
  Wire.requestFrom (eport, 1);
  return Wire.receive();
} // end of expanderRead

void keypress()
{
    keypressed = true;
   // if (menuflag) return;   
    
}

void handleKeypress()
{
  unsigned int keyValue = 0;
  delay(150); // de-bounce before we re-enable interrupts
  keypressed = false;
  
  keyValue= expanderRead(INTCAPA);
  if(keyValue==192)
   { 
     writeWrites();
   }
   
  if(keyValue==8) keyValue= 4;
  if(keyValue==32) keyValue= 3;
//  Serial.print("Key value: ");
//  Serial.print(keyValue);
//  Serial.println();  
}

void writeMaxMin()
{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Max.: ");
    lcd.print(maxT);
    lcd.print(char(178));
    lcd.print("C");
    lcd.setCursor(0,1);
    lcd.print("Min.: ");
    lcd.print(minT);
    lcd.print(char(178));
    lcd.print("C");
}

void writeWrites()
{
  lcd.clear();
  lcd.print("Writes: ");
  lcd.print(millisoverflow);
  delay(500);
}

//********************************************
