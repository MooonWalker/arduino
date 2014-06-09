#include <SPI.h>
#include<avr/wdt.h>

#define SCK_PIN   13
#define MISO_PIN  12
#define MOSI_PIN  11
#define SS_PIN    4
//pin D3 INT1
unsigned long resetTime = 0;
#define TIMEOUTPERIOD 3000             // You can make this time as long as you want,
                                       // it's not limited to 8 seconds like the normal
                                       // watchdog
#define doggieTickle() resetTime = millis();  // This macro will reset the timer
void(* resetFunc) (void) = 0; //declare reset function @ address 0

volatile char command;
int prevState = HIGH;

void watchdogSetup()
{
cli();  // disable all interrupts
wdt_reset(); // reset the WDT timer
MCUSR &= ~(1<<WDRF);  // because the data sheet said to
/*
WDTCSR configuration:
WDIE = 1 :Interrupt Enable
WDE = 1  :Reset Enable - I won't be using this on the 2560
WDP3 = 0 :For 1000ms Time-out
WDP2 = 1 :bit pattern is
WDP1 = 1 :0110  change this for a different
WDP0 = 0 :timeout period.
*/
// Enter Watchdog Configuration mode:
WDTCSR = (1<<WDCE) | (1<<WDE);
// Set Watchdog settings: interrupte enable, 0111 for timer
WDTCSR = (1<<WDIE) | (0<<WDP3) | (1<<WDP2) | (1<<WDP1) | (1<<WDP0);
sei();
Serial.println("finished watchdog setup");  // just here for testing
}

void setup (void)
{
  Serial.begin (9600);
  Serial.println("Waiting...");
  pinMode(6, OUTPUT);
  digitalWrite(6, LOW);
  pinMode(SCK_PIN, INPUT);
  pinMode(MOSI_PIN, INPUT);
  pinMode(MISO_PIN, OUTPUT);
  pinMode(SS_PIN, INPUT);
  pinMode(2, INPUT);
  digitalWrite(2 , LOW);
  // have to send on master in, *slave out*
  //digitalWrite(SS, HIGH);
  SPI.setDataMode(SPI_MODE3);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  

  // turn on SPI in slave mode
  SPCR |= _BV(SPE);

  // now turn on interrupts
  SPI.attachInterrupt();
  //attachInterrupt (1, ss_falling, FALLING);
  watchdogSetup();
}

ISR(WDT_vect) // Watchdog timer interrupt.
{
  if(millis() - resetTime > TIMEOUTPERIOD)
  {
    Serial.println("This is where it would have rebooted");  // just here for testing
   
//  resetFunc();     // This will call location zero and cause a reboot.
  }                                                      // these lines should
    Serial.println("Howdy");                                 // be removed also
}

// SPI interrupt routine
ISR (SPI_STC_vect)
{
  //pinMode(MISO_PIN, OUTPUT);
  command = SPDR;  // grab byte from SPI Data Register
  switch (command)
  {
  // no command? then this is the command
  case 'x':
    SPDR = 0;
    break;
    
  // add to incoming byte, return result
  case 'h':
    SPDR = 0;
      //digitalWrite(6, LOW);  
     int currState = digitalRead(6);
     if(currState != prevState)
       {
           digitalWrite(6, prevState);
       }
     prevState = currState; 
     
     //doggieTickle();
     wdt_reset();
     command=0;  
    break;
  

  } // end of switch
}

void loop (void)
{
 
}
  

