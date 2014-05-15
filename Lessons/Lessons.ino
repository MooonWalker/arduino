#define INTENSITY 30

const byte digitPins[4] = {9, 10, 3, 11};
const int numeral[10] = {
	//ABCDEFG dp
	B11111100, //0
	B01100000, //1
	B11011010, //2
	B11110010, //3
	B01100110, //4
	B10110110, //5
	B00111110, //6
	B11100000, //7
	B11111110, //8
	B11100110, //9	
};
                         // dp, G,  F, E, D, C,  B, A
const int segmentPins[] = { 2 , 13, 1, 4, 8, 12, 5, 6};
const int nbrDigits=4;
	


	
void setup()
{
	//bitSet(TCCR2B, CS21);
	//bitSet(TCCR1B, CS21);
	//bitSet(TIMSK1, TOIE1);
	//bitSet(TIMSK2, TOIE2);
	digitalWrite(2,HIGH);
	
	for (int i=0;i<8;i++)
	{
		pinMode(segmentPins[i], OUTPUT);		
	}
	
	for (int i=0;i<nbrDigits;i++)
	{
		pinMode(digitPins[i],OUTPUT);
	}
	
	
}
void showNumber(int number)
{
	if (number==0)
	{
		showDigit(0, nbrDigits-1);
	} 
	else
	{
		for (int digit=nbrDigits-1;digit>=0;digit--)
		{
			if (number>0)
			{
				showDigit(number % 10, digit);
				number=number/10;
			}			
		}		
	}	
}

void showDigit( int number, int digit )
{
	digitalWrite(digitPins[digit],HIGH);
	for (int segment =1;segment<8;segment++)
	{
		boolean isBitSet = bitRead(numeral[number],segment);
		digitalWrite(segmentPins[segment],isBitSet);
	}
	delay(4);
	digitalWrite(digitPins[digit],0);
	
}

void loop()
{
	
	int value= analogRead(0);
	showNumber(value);
	
	//	analogWrite(digitPins[3], INTENSITY);
		//delay(150);
	
	
}


ISR(TIMER2_OVF_vect)
{
	//digitalWrite(segmentPins[1], HIGH);
	
}

ISR(TIMER1_OVF_vect)
{
	
}
