//Running two ultrasonic sensors in series, without interrupts

const int pingPin = 4;
const int echoPin = 2;
const int pingPin2 = 5;
const int echoPin2 = 3;
long duration, inches, duration2, inches2;

void setup() {
  Serial.begin(9600);
  pinMode(pingPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(pingPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

}

void loop()
{
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  digitalWrite(pingPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin2, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin2, LOW);

  duration2 = pulseIn(echoPin2, HIGH);

  inches = microsecondsToInches(duration);
  inches2 = microsecondsToInches(duration2);
   
   Serial.print("sensor1 = ");
   Serial.println(inches);
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
