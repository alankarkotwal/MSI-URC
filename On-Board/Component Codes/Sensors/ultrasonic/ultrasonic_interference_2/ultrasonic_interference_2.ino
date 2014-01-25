//For running two ultrasonic sensors using interrupts

const int pingPin = 4;
const int echoPin = 2; const int echo2Pin = 21;
const int pingPin2 =5;
const int echoPin2= 3; const int echo2Pin2= 20; 
long timer1, timer2, inches1, inches2;

void int11(){timer1 = micros();//Serial.println("sensor 1 rising");
}
void int12()
{
  timer1 = micros() - timer1;
  //Serial.print("timer1 ");
  //Serial.println(timer1);
//  Serial.println("sensor 1 falling");
} 
void int21()
{timer2 = micros(); //Serial.println("sensor 2 rising");
}
void int22()
{
  timer2 = micros() - timer2;
  Serial.println("sensor 2 falling");
}

void setup() {
  Serial.begin(9600);
  pinMode(pingPin, OUTPUT);
  pinMode(pingPin2, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(echoPin2, INPUT);
  
  attachInterrupt(0, int11, RISING);
  attachInterrupt(2, int12, FALLING);
  attachInterrupt(1, int21, RISING);
  attachInterrupt(3, int22, FALLING);

}

void loop()
{
  
  digitalWrite(pingPin, LOW); //digitalWrite(pingPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH); //digitalWrite(pingPin2, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);// digitalWrite(pingPin2, LOW);
  digitalWrite(pingPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin2, HIGH); 
  delayMicroseconds(5);
  digitalWrite(pingPin2, LOW); 
 
  
    // duration = pulseIn(echoPin, HIGH);

  
  if(timer1 > 50000){ inches1 = microsecondsToInches(50000);}
  else{inches1 = microsecondsToInches(timer1);}
  
  if(timer2 > 50000){ inches2 = microsecondsToInches(50000);}
  else{inches2 = microsecondsToInches(timer2);}
   Serial.print("sensor1 = ");
   Serial.println(inches1);
   Serial.print("sensor2 = ");
   Serial.println(inches2);
   
   delay(500);
}

long microsecondsToInches(long microseconds)
{
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds)
{
  return microseconds / 29 / 2;
}
