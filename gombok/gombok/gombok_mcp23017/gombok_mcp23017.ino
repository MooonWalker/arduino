#include <Wire.h>

#define eport 0x20  // MCP23017 is on I2C port 0x20 B0100000 7 bit 0100A1A2A0
#define INTCAPA 0x08
#define INTCAPB 0x18

byte inputs=0;             // Variable to store the Byte in
volatile boolean keypressed;
unsigned long time = 0;

const int buttonPin = 2;  // the number of the pushbutton pin

void setup()
{
  Serial.begin(9600); 
  Wire.begin();                // wake up I2C bus
  pinMode(buttonPin,    INPUT_PULLUP);
  expanderWrite(0x0B, B10000000); //IOCON, BANK1  
                                  // valid command only if BANK = 0, which is true upon reset
                                  // set BANK = 1 if it had been 0 (nothing happens if BANK = 1 already)
  //expanderWrite(0x06, 0xff); //GPPUA                               
  expanderWrite(0x16, 0x7f); //GPPUB
  //expanderWrite(0x01, 0xff); //IPOLA                               
  expanderWrite(0x11, 0xff); //IPOLB
  //expanderWrite(0x02, 0xff); //GPINTENA
  expanderWrite(0x12, 0xff); //GPINTENB
  
  keypressed=false;
  
  // read from interrupt capture ports to clear them
  expanderRead(INTCAPB); //INTCAPA   
    
  //attachInterrupt(0, keypress, FALLING);
  attachInterrupt(digitalPinToInterrupt(buttonPin), keypress, FALLING);
    Serial.println("Configured");
}

void loop()
{
//  Wire.beginTransmission(B0100000); // Connect to chip
//  Wire.write(0x09);             // GPIOA
//  Wire.endTransmission();       // Close connection
//  Wire.requestFrom(B0100000, 1);    // Request one Byte
//  inputs=Wire.read();  
  
   
    if(keypressed)
    {
        handleKeypress();    
        
    }
  
  // turn LED off after 500 ms 
 if (millis() > (time + 500) && time != 0)
   {
     Serial.print("Time is up");
     Serial.println();
      time = 0;
   }  // end if time up
   
}   //-----------------------Endloop

void keypress()
{
    keypressed = true;
}

// set register "reg" on expander to "data"
// for example, IO direction
void expanderWrite(const byte reg, const byte data ) 
{
  Wire.beginTransmission(eport);
  Wire.write (reg);
  Wire.write (data);  // port A
  Wire.endTransmission ();
} // end of expanderWrite

// read a byte from the expander
unsigned int expanderRead(const byte reg) 
{
  Wire.beginTransmission (eport);
  Wire.write (reg);
  Wire.endTransmission ();
  Wire.requestFrom (eport, 1);
  return Wire.read();
} // end of expanderRead

void handleKeypress()
{
  unsigned int keyValue = 0;
  delay(100); // de-bounce before we re-enable interrupts
  keypressed = false;
  
  keyValue= expanderRead(INTCAPB);
  if(keyValue==192) keyValue= 5;
  if(keyValue==8) keyValue= 4;
  if(keyValue==32) keyValue= 3;
  Serial.print("Key value: ");
  Serial.print(keyValue);
  Serial.println();
  time = millis();  // remember when
  
}
