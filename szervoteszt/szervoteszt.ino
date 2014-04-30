#include <io.h>
#include <interrupt.h>
#include <Servo.h> 

Servo balservo, jobbservo;  // create servo object to control a servo 

volatile int buttonstate;
boolean servoattached=false;
unsigned long timer=0, timer1=0;
const int TRIG_PIN = 12;
const int ECHO_PIN = 11;
const int DIST_MED =7;  //yellow led
const int DIST_LOW=8;    //red led

void setup() 
{ 
  cli(); //disable global interrupts
  //  Serial.begin(9600);
  DDRB |=1<<4; //pin12 out
  DDRB |=1<<5; //pin13 out
  DDRD |=1<<7; //pin7 out
  DDRB |=1; //pin8 out

  DDRB |=0<<3; //pin11 input
  DDRD |=0<<2; //pin2 input

  //balservo.attach(10);  // attaches the servo on pin 10 to the servo object 
  //jobbservo.attach(9);
  //balservo.writeMicroseconds(1375);       
  //jobbservo.writeMicroseconds(1146);
  //servoattached=true;

  PORTB |=0<<5;
  PORTD |=1<<2;
  PORTD |=0<<7;
  PORTB |=0;

  EICRA |= ( 1 << ISC00);
  EIMSK |= ( 1 << INT0);

  sei(); //enable global interrupts

  timer=millis();  
  timer1=millis();
} 


void loop() 
{ 
  if(millis()-timer>3000)
  {
    doVoltmeasure();  
    timer=millis();
  }

  if(millis()-timer1>500)
  {
    doSonarping();
    timer1=millis();
  }

  if(buttonstate==HIGH)
  {
    balServo(1);
	jobbServo(1);
  }
  else
  {
    balServo(0);
	jobbServo(0);
  }
  
} 

ISR(INT0_vect)
{
  buttonstate = digitalRead(2);
}

void balServo(signed char velocity)
{
  switch(velocity)
  {
     case 0:
		balservo.detach();
      break;
      
     case 1:
		balservo.attach(10);
		balservo.writeMicroseconds(2000);
      break;
      
     case -1:
		balservo.attach(10);
		balservo.writeMicroseconds(1000);
      break;
  }
}

void jobbServo(signed char velocity)
{
	switch(velocity)
	{
		case 0:
		jobbservo.detach();
		break;
		
		case 1:
		jobbservo.attach(9);
		jobbservo.writeMicroseconds(500);
		break;
		
		case -1:
		jobbservo.attach(9);
		jobbservo.writeMicroseconds(2000);
		break;
	}
}

void doSonarping()
{
  long duration, distanceCm; 
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN,HIGH);

  // convert the time into a distance
  distanceCm = duration / 29.1 / 2 ;

  if(distanceCm <=5)
  {
    digitalWrite(DIST_MED,LOW);
    digitalWrite(DIST_LOW,HIGH);  
  }
  else if(distanceCm<=10 && distanceCm>=5)
  {
    digitalWrite(DIST_MED,HIGH);
    digitalWrite(DIST_LOW,LOW); 
  }
  else
  {
    digitalWrite(DIST_MED,LOW);
    digitalWrite(DIST_LOW,LOW);
  }

    if (distanceCm <= 0)
    {
      Serial.println("Out of range");
    }
    else 
    {
      Serial.print(distanceCm);
      Serial.print("cm");
      Serial.println();
    }

}

void doVoltmeasure()
{
  bitSet(PORTB, 5);    
  delay(60);
  bitClear(PORTB, 5);


  if(readVcc()<4000)
  {
    bitSet(PORTB,5);  
  }  

}

long readVcc() 
{
  long result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = 1126400L / result; // Back-calculate AVcc in mV
  return result;
}


