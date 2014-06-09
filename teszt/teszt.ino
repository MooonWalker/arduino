
void setup()
{
	bitSet(DDRG, 1);
	digitalWrite(40, LOW);
	TCCR5A=0;
	TCCR5B =1<<WGM53 | 1<<WGM52 | 1<<CS52 |1<<CS50;
	ICR5=7812; // 1Hz
	bitSet(TIMSK5, ICIE5);
}

void loop()
{
}

ISR(TIMER5_CAPT_vect)
{
	bitSet(PING, 1);
}
