void setup()
{
	bitSet(DDRB, 7); //Led on PB7/D13
	bitSet(PCICR, PCIE0); //enable pin change interrupt on bank0
	bitSet(PCMSK0, PCINT6 ); //enable PB6 pin change interrupt
	//TCCR1A = 0;		//detach PWM from Timer1
	//TCCR1B = 1<<WGM13 | 1<<WGM12 | 1<<CS12 | 1<<CS10; //
	//ICR1 = 15625;
	//
	//bitSet(TIMSK1, ICIE1); //enable overflow interrupt
//
}

byte pwm=0;
void loop()
{
	
	analogWrite(12, pwm); //ledpin 13 (PB7)
	delay(10);
	pwm++;
  
}

ISR(PCINT0_vect)
{
	if (bitRead(PINB,6))
	{
		bitSet(PORTB, 7); //led on
	}
	else
	{
		bitClear(PORTB, 7);	
	}
	//bitSet(PINB, 7); //toggle led
}
