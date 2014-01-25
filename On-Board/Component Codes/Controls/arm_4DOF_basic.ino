/*arm has 4 DOF, this is an open loop system
spherical coordinate system is used, origin at the shoulder joint
inputs are from keyboard: 4 and 6 for rotation about vertical axis(phi)
8 and 2 for theta
q and z for r variation
*/ 
#include<Servo.h>
#include<math.h> 

Servo base, shoulder, elbow, wrist;

//constants
const int max[4], min[4]; //max and minimum angle each servo can move
const int l1, l2, l3; //arm lengths, l1 for upper_arm, l2 for lower_arm, l3 for hand

//global variables
int s1, s2, s3, s4;  //servo angle s1-base, s2-shoulder, s3-elbow, s4-wrist
float r, theta, phi;
char command;
double cos_s3;

void initialize()
{
  r = l1-(l2+l3) > 0? l1-(l2+l3) : 0;
  theta = max[1];
  phi = 90;
  s4 = 90;
  //have to set values of constants also
}


void getRTP()
{
  command = Serial.read();
  switch(command)
  {
    case '8': theta += 1; break;
    
    case '2': theta -+ 1; break;
    
    case '4': phi += 1; break;
    
    case '6': phi -= 1; break;
    
    case 'q': r += 1; break;
    
    case 'z': r -+ 1; break;
  }
  
  void solveRTP()
  {
    s1 = phi;
    s2 = theta;
    cos_s3 = (l1*l1 + (l2+l3)*(l2+l3) - r*r)/(2*l1*(l2+l3);
    s3 = acos(cos_s3);
  }
  
  void setServo()
  {
    wrist.write(s4);
    elbow.write(s3);
    shoulder.write(s2);
    base.write(s1);
  }
  
  void setup()
 {
  Serial.begin(9600);
  base.attach();
  shoulder.attach();
  elbow.attach();
  wrist.attach();
  solveRTP();
  setServo();
 }
  

void loop()
{
  getRTP();
  solveRTP();
  setServo();
}
  


