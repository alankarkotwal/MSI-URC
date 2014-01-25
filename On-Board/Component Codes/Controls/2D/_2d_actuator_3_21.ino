//2D control with position feedback of actuator 
//3-01-2014
//working properly 07/01/2014
//Input is given from serial monitor as 1(move in +ve x direction), 2(-ve x), 3(+ve y) and 4(-ve y)
//arm moves in steps of 10mm until receives command for stopping( same input as above)
//actuators move with speeds proportional to the length they have to move to reach target from current position
//pot feedback is monitored and actuator stops on reaching within +-5 mm from target position
//arm tip movement is limited within a square ( taking base as origin) 

//constants
float d = 285, b = 110, a1=435, a2=430, b1=70, c1=90, b2=85;  //dimensions of arm
float pot1_min = 388, pot1_max = 633, pot2_min = 386, pot2_max = 631;  //minimum and maximum output of pot of actuators
int l1max = 150, l2max = 150;                              //maximum length that actuators can extract
float xmin = 200, xmax = 400, ymin = 0, ymax = 600;        //x and y coordinates of the corners of the square
float ds = 10;                  //steps in which x and y coordinates change

//peripherals
int pot1 = 0, pot2 = 2;  //pot analog input pins
int out3 = 2, out4 = 4, out1 = 7, out2 = 8; //motor output digital pins 
int motor2 = 3, motor1 = 9; //pwm pins for output to enable pins of motor driver

//variables
float x,y, l1, l2,r, delL1, delL2, d1, d2;  //coordinates of tip of arm(x,y) ; extracted length(l1,l2) ; required change in length
float theta1, theta2;   //angles at the shouler and elbow joints
boolean check = false;  
char val;

//function intialize : to fully retract both the actuators
void initialize()   
{
  //set directions
  digitalWrite(out1, LOW);
  digitalWrite(out2, HIGH);
  digitalWrite(out3, LOW);
  digitalWrite(out4, HIGH);
  //start the motors
  analogWrite(motor1, 255);
  analogWrite(motor2, 255);
  //feedback loop
  while(1)
  {
   if( analogRead(pot1)<390 && analogRead(pot2)<390 )
  break; 
  }
  //stop motors
  analogWrite(motor1, 0);
  analogWrite(motor2, 0);
  
 Serial.println("intialized...");
 l1=0;
 l2=0;
}

//function to get x,y coordinates of tip of arm from l1, l2
void solveXY()
{
  l1 = l1Current();
  l2 = l2Current();
  theta1 = acos((b*b + (a1-c1)*(a1-c1) - (d+l1)*(d+l1))/(2*b*(a1-c1)));
  theta2 = acos((b2*b2 + (a1-b1)*(a1-b1) - (l2+d)*(l2+d) - 50*50)/(2*b2*(a1-b1)));
  
  x = a2*cos(theta2-theta1) - a1*cos(theta1);
  
  y = a1*sin(theta1) + a2*sin(theta2 - theta1);
}

//function to get l1,l2 from x,y coordinates of tip of arm using inverse kinematics
void solveL1L2()
{
  Serial.print(x);Serial.print(' ');
  Serial.println(y);
  r = sqrt(x*x+y*y);
  l1 = sqrt(b*b + (a1-c1)*(a1-c1) + 2*b*(a1-c1)*cos(acos((a1*a1+r*r-a2*a2)/(2*r*a1))+atan(y/x))) - d;
  l2 = sqrt(b2*b2 + (a1-b1)*(a1-b1) - 2*b2*(a1-b1)*((a2*a2+a1*a1-r*r)/(2*a1*a2)) - 50*50) - d;
  Serial.println(l1);
  Serial.println(l2);
  //check if required length exceeds max length of actuators
  if(l1<0) 
  { 
    l1=0;
    solveXY();
  }
  else if(l1>150) 
  {
    l1=150;
    solveXY();
  }
  if(l2<0)
 {
   l2=0;
   solveXY();
 }
  else if(l2>150) 
  {
    l2=150;
    solveXY();
  }
  Serial.print(l1);
  Serial.print(' ');
  Serial.println(analogRead(pot1));
  Serial.print(l2);
 Serial.print(' ');
 Serial.println(analogRead(pot2));
   
}

//function to calculate extracted length from pot feedback
float l1Current()
{
  float pot = 0;
  for(int i =0; i<10; i++)
  {
    pot = pot + analogRead(pot1);
  }
  
  float val = (( pot/10 - pot1_min)/(pot1_max - pot1_min ))*l1max;
  return val;
}

float l2Current()
{
  float pot = 0;
  for(int i =0; i<10; i++)
  {
    pot = pot + analogRead(pot2);
  }
    
  float val =((pot/10 - pot2_min)/(pot2_max - pot2_min ))*l2max; 
  return val ;
}

//function to set the lengths of actuators 
void setArm()
{
  delL1 = abs(l1-l1Current());
  delL2 = abs(l2-l2Current());
  //set directions of the actuaturs 
    if(l1>l1Current())
   {
   digitalWrite(out1, HIGH); 
   digitalWrite(out2, LOW);  
   }
   else
   {
     digitalWrite(out1, LOW);
     digitalWrite(out2, HIGH);
   }
   if(l2>l2Current())
   {
   digitalWrite(out3, HIGH); 
   digitalWrite(out4, LOW);  
   }
   else
   {
     digitalWrite(out3, LOW);
     digitalWrite(out4, HIGH);
   }
   
   //set speeds of motors according to lengths
   if(delL1 > delL2)
   {
   analogWrite(motor1, 255);
   analogWrite(motor2, (delL2/delL1)*255);
   }
   
   else
   {
   analogWrite(motor2, 255);
   analogWrite(motor1, (delL1/delL2)*255);
   }
   
   Serial.println(l1Current());
   Serial.println(l2Current());
  
   check = false; 
   //feedback loop
   while(!check) 
   {
     for(int i =0; i<10; i++)
     {
       if(abs(l1-l1Current()) < 5 && abs(l2-l2Current())<5)  //if actuator lengths are within 5mm of desired length
       check = true ; 
       else
       check = false;
     }
   }
   
   Serial.println(l1Current());
   Serial.println(l2Current());   
   analogWrite(motor1, 0);
   analogWrite(motor2, 0);
   
   Serial.println("arm set...");
}

//setup function
void setup()
{
  Serial.begin(9600);
  //set pin modes of I/O pins
  pinMode(out1, OUTPUT);
  pinMode(out2, OUTPUT);
  pinMode(out3, OUTPUT);
  pinMode(out4, OUTPUT);
  digitalWrite(out1, LOW);
  digitalWrite(out2, LOW);
  digitalWrite(out3, LOW);
  digitalWrite(out4, LOW);
  analogWrite(motor1, 0);
  analogWrite(motor2, 0);
  
  //initialize();
  delay(1000);
  //go to the centre of square
  x = (xmax+xmin)/2;
  y = (ymax+ymin)/2;
  solveL1L2();
  setArm();
}

//loop
void loop()
{
  if(Serial.available())
  {
    val = Serial.read();  //take input from serial monitor 
    switch(val)
    {
      case '1':
                do
                {x = x+ds;
                if(x>xmax) { x=xmax; break;} //check if going out of prescribed square
                solveL1L2();
                setArm(); }
                while(Serial.available()==0 || (Serial.available()>0&&Serial.read()!='1'));  //keep moving till button released 
                break;
                
                
      case '2': do
                {x = x-ds;
                if(x<xmin) { x=xmin; break;}
                solveL1L2();
                setArm(); }
                while(Serial.available()==0 || (Serial.available()>0&&Serial.read()!='2'));  //keep moving till button released 
                break;
                
     case '3': do
                {y=y+ds;
                if(y>ymax) { y=ymax; break;}
                solveL1L2();
                setArm(); }
                while(Serial.available()==0 || (Serial.available()>0&&Serial.read()!='3'));  //keep moving till button released 
                break;
               
     case '4': do
                {y=y-ds;
                if(y<ymin) { y=ymin; break;}
                solveL1L2();
                setArm(); }
                while(Serial.available()==0 || (Serial.available()>0&&Serial.read()!='4'));  //keep moving till button released 
                break; 
    }
  }
}


//end of program
