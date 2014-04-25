const int TRIG_PIN = 50;
const int ECHO_PIN = 40;
 
void setup() 
{
  // initialize serial communication:
  Serial.begin(9600);
 
  pinMode(TRIG_PIN,OUTPUT);
  pinMode(ECHO_PIN,INPUT);
}

void loop()
{
  long duration, distanceCm;
   
  digitalWrite(TRIG_PIN, 0);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN,HIGH);
  distanceCm = duration / 29.1 / 2 ;
   
  if (distanceCm <= 0){
    Serial.println("Out of range");
  }
  else {
  
    Serial.print(distanceCm);
    Serial.print("cm");
    Serial.println();
  }
  delay(500);  
}
