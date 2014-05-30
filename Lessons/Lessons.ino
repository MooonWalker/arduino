
	


	
void setup()
{
	pinMode(5,OUTPUT); //ENA
	pinMode(9,OUTPUT);
	pinMode(10,OUTPUT);
	digitalWrite(5,HIGH);	
	digitalWrite(10,HIGH);	
	digitalWrite(9,HIGH);
}


void loop()
{
	//
	digitalWrite(5,HIGH);
	digitalWrite(9,HIGH);
	digitalWrite(10,LOW);
	//
	
	
	
}


