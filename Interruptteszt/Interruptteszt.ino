void setup()
{
	Serial.begin(9600);
	Serial.println("Start...");
	pinMode(18, INPUT);
	digitalWrite(18, HIGH);
	pinMode(19, INPUT);
	digitalWrite(19, HIGH);
	delay(5);
	attachInterrupt(4, doInterrupt4, CHANGE);
	attachInterrupt(5, doInterrupt5, CHANGE);
  /* add setup code here */

}

void loop()
{

  /* add main program code here */

}

void doInterrupt4()
{
	Serial.println("j");
	delay(2);
}

void doInterrupt5()
{
	Serial.println("b");
	delay(2);
}