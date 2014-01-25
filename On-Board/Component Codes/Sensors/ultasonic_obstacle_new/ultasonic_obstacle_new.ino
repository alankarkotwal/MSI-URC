//obstacle avoidance code for a bot with 4 ultrasonic sensors

#include<math.h>

#define NUM 4 //no of sensors
//connect the echo pins on pins 0-(n-1) and trig pins on pins n-(2n-1)
//for 4 sensors: front_left_rear_right
int left_pos = 10, left_neg = 9, right_pos = 6, right_neg = 11; //inputs to motor
float inches[NUM], discrete[NUM];//to store values from the sensors
float ideal[NUM], error[NUM],output[NUM];
//int primary, secondary;
float kp, ki, kd;
float front, right, angle, angle2;
unsigned long time, time_past = 0;
float error_past[NUM] = {0,0,0,0}; float error_int[NUM] = {0,0,0,0};
int fast = 60;
void initialize()
{
  for(int i = 0; i<NUM; i++)
  {
    pinMode(i, INPUT);
    pinMode(i+4, OUTPUT);
  }
  pinMode(left_pos, OUTPUT); digitalWrite(left_pos, LOW);
  pinMode(left_neg, OUTPUT); digitalWrite(right_pos, LOW);
  pinMode(right_pos, OUTPUT); digitalWrite(left_neg, LOW);
  pinMode(right_neg, OUTPUT); digitalWrite(right_neg, LOW);
  pinMode(13, OUTPUT);//note that this isthe trig pin of back sensor(ie 3rd)
}

void trigger()
{ 
  for(int i = 0; i<NUM; i++)
  { if(i == 2){}
    else
    {
      digitalWrite(i+4, LOW);
      delayMicroseconds(2);
      digitalWrite(i+4, HIGH);
      delayMicroseconds(5);
      digitalWrite(i+4, LOW);
      inches[i] = (pulseIn(i, HIGH))/74/2;
    }
  }
    digitalWrite(13, LOW);
    delayMicroseconds(2);
    digitalWrite(13, HIGH);
    delayMicroseconds(5);
    digitalWrite(13, LOW);
    inches[2] = (pulseIn(2, HIGH))/74/2;
//    Serial.print("Back "); Serial.println(inches[2]);
   
}

void idealize(int inch)
{
  for(int i = 0; i<NUM; i++){ideal[i] = inch;}
 
}

void pid()
{ 
  time = millis() - time_past;
  for(int i = 0; i<NUM; i++)
  {
    error[i] = ideal[i] - inches[i];
    if(error[i]<=0){error[i] = 0;}
    error_int[i] += error[i]*time; 
    output[i] = kp*error[i] + ki*error_int[i] + kp*(error[i] - error_past[i])/time;
    error_past[i] = error[i];
  }
  time_past = time;  
  Serial.print("Back "); Serial.println(inches[2]);
  Serial.print("Left "); Serial.println(inches[3]);

}
//output[i]: move away these much distances from the current position
void get_direction() //this function will be different for different sensors, here 4
{
  front = output[2] - output[0]; //the bot should move these many inches in the front to be safe
  right = output[3] - output[1]; //these many inches to the right
//  Serial.print("Front "); Serial.println(front);
 // Serial.print("Right "); Serial.println(right);
  
  if(front>= 0){
  float angle = atan(right/front);
  angle = map(angle, -PI/2, PI/2, -fast, fast);
}
  else{angle = atan(right/front) + PI;
  angle2 = map(angle, PI/2, 3*PI/2, -fast, fast);
  }
  
}
void motors()
{
  if(output[0] == 0){//if there's no obstacle in the front, move front irrespective of other obstacles
    analogWrite(left_pos, fast);
    analogWrite(right_pos, fast);
    analogWrite(left_neg, 0);
    analogWrite(right_neg, 0);
  }
  if(front>=0)
  {
   analogWrite(left_pos, fast + angle);
   analogWrite(right_pos, fast - angle);
   analogWrite(left_neg, 0);
   analogWrite(right_neg, 0);
  }
  else if(front<0)
  {
   analogWrite(left_neg, fast + angle2);
   analogWrite(right_neg, fast - angle2);
   analogWrite(left_pos, 0);
   analogWrite(right_pos, 0);
  }
} 

void setup()
{
  initialize();
  idealize(10);
  kp = 1; ki = 0; kd = 0;
}

void loop()
{ 
 
  trigger();
  pid();
  get_direction();
  motors();
  delay(20);
 /* analogWrite(left_pos, 120);
  analogWrite(right_pos, 120);
  analogWrite(left_neg, 0);
  analogWrite(right_neg, 0);
  delay(500);*/
}
