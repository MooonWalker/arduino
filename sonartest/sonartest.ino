#include <SharpIR.h>

#define ir A0
#define model 1080

boolean done = false;
const int TRIG_PIN = 50; //mega
const int ECHO_PIN = 40; //mega

SharpIR sharp(ir, 25, 93, model);

// ir: the pin where your sensor is attached
// 25: the number of readings the library will make before calculating a mean distance
// 93: the difference between two consecutive measurements to be taken as valid
// model: an int that determines your sensor:  1080 for GP2Y0A21Y
//                                            20150 for GP2Y0A02Y
//                                            (working distance range according to the datasheets) 
 
void setup() 
{
  // initialize serial communication:
  Serial.begin(9600);
 
  pinMode(TRIG_PIN,OUTPUT);
  pinMode(ECHO_PIN,INPUT);
  pinMode(ir, INPUT);
}

void loop()
{
  long duration, distanceCm;
  unsigned long pepe1=millis();  // takes the time before the loop on the library begins 
  int dis=sharp.distance();  // this returns the distance to the object you're measuring 
  Serial.print("Mean distance: ");  // returns it to the serial monitor
  Serial.println(dis);
  
  unsigned long pepe2=millis()-pepe1;  // the following gives you the time taken to get the measurement
  Serial.print("Time taken (ms): ");
  Serial.println(pepe2);  
   
  digitalWrite(TRIG_PIN, 0);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN,HIGH);
  distanceCm = duration / 29.1 / 2 ;
   
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
  delay(500);  
}
