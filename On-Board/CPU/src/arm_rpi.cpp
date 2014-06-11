#include<iostream>
#include"arm_rpi.h"
#include<math.h>

#define PRINTARM 1

using namespace std;

float sign(float value)
{
    if(value>0) return value;
    else return value*(-1);
}

void FINAL_ARM::writeAct(uint8_t act, uint8_t direction, uint8_t pwm)
{
	uint8_t send = 0;
	char temp;
	send = 8 + act*pow(2,2) + direction*pow(2,1) ;
	temp = 'A';
	arm_arduino.write_byte('A');
    arm_arduino.write_byte((char)send);
	arm_arduino.write_byte('S');
	arm_arduino.write_byte((char)pwm);
	//cout<<"\n written..";
}

int FINAL_ARM::readPot(uint8_t act)
{
    uint8_t value;
	int pot_value = 0;
	uint8_t send = 0;
	send =  act*pow(2,2);
	arm_arduino.write_byte('A');
	arm_arduino.write_byte((char)send);
	label:
		if(arm_arduino.available() > 0)
	    {
		uint8_t header;
		header = arm_arduino.read();
		if(header == pot)
        {
            while(arm_arduino.available() == 0);
       		if(arm_arduino.available()>0)
            {
                value = arm_arduino.read();
                pot_value = value;
                while(arm_arduino.available() == 0);
                if(arm_arduino.available()>0)
                    value = arm_arduino.read();
                pot_value = pot_value + value*256;
            }
            else goto label;
        }
        else goto label;
		}
		else goto label;
		//cout<<"\n read...";
	return pot_value;
}

float FINAL_ARM::l1Current()
{return 0;
}

float FINAL_ARM:: theta1Current()
{
	float pot = 0;
  	for(int i =0; i<10; i++)
  	{
    	pot = pot + readPot(act1);
  	}

 	float val = 1.1198 + ( pot/10.0 - pot1_min)*scale_factor1;
 	
 	return val;
}


float FINAL_ARM::l2Current()	
{
 
	float pot = 0;
  	for(int i =0; i<10; i++)
  	{
    	pot = pot + readPot(act2);
  	}

 	float val = ( pot/10.0 - pot2_min)/scale_factor2;
 	
 	return val;
}


//function to get x,y coordinates of tip of arm from l1, l2
void FINAL_ARM::solveXY()  //calculation part working properly
{
  float theta1_now = theta1Current();
  float l2_now = l2Current();
  #if PRINTARM
  std::cout<<"theta1    "<<theta1_now<<"    l2  "<<l2_now;
  #endif
  
  theta2 = PI -acos((b2*b2 + c2*c2 - (l2_now+d)*(l2_now+d))/(2*b2*c2)) + C;
  

  r = sqrt(a2*a2 + c1*c1 - 2*c1*a2*cos(theta2 - A));

  x = cos(theta1_now - B - acos((c1*c1 + r*r - a2*a2)/(2*c1*r)))*r + e;

  y = sin(theta1_now - B - acos((c1*c1 + r*r - a2*a2)/(2*c1*r)))*r;

#if PRINTARM
  std::cout<<"x y current   "<<x<<" "<<y<<"\n";
  #endif
}

//function to get l1,l2 from x,y coordinates of tip of arm using inverse kinematics
void FINAL_ARM::solveL1L2()    //working properly
{
   
  r = sqrt((x-e)*(x-e) + y*y);
  theta1 = B + acos((c1*c1 + r*r - (a2)*(a2))/(2*c1*r)) + asin(y/r);
  
  theta2 = A + acos(((a2)*(a2) + c1*c1 -r*r)/(2*c1*(a2)));
  l2 = sqrt(b2*b2 + c2*c2 + 2*c2*b2*cos(theta2 - C)) - d;
  #if PRINTARM
  cout<<theta1<<"\t"<<l2<<endl;
  #endif
  //check if required length exceeds max length of actuators
  if(theta1<0)
  {
    theta1=0.006;
  }
  else if(theta1>1.22)
  {
    theta1=1.22;
  }
  if(l2<5)
 {
   l2=5;
 }
  else if(l2>140)
  {
    l2=140;
  }
  #if PRINTARM
  std::cout<<"to set "<<theta1<<"   "<<l2<<"\n";
  #endif

}


void FINAL_ARM::setArm()
{
    #if PRINTARM
    cout<<"\n current length l1 "<<l1Current()<<"   l2  "<<l2Current()<<"\n";
    #endif

	int flag1 = 0;
	int flag2 = 0;
	while(flag1 == 0 || flag2 == 0)
   {
       if(flag1 == 0)
     {
         /*if(abs(l1-l1Current())<40)
         {
            if(l1>l1Current())
            writeAct(act1, out, 255*0.5);
            else
            writeAct(act1, in, 255*0.5);
         }*/
         if(abs(l1-l1Current())<pot1_tol)
     {
         writeAct(act1, in, 0);
         flag1 = 1;
     }
     else
     {
        if(l1>l1Current())
        writeAct(act1, out, 255);
        else
        writeAct(act1, in, 255);
     }
     }

        if(flag2 == 0)
     {
         /*if(abs(l2-l2Current())<40)
         {
            if(l2>l2Current())
            writeAct(act2, out, 255*0.5);
            else
            writeAct(act2, in, 255*0.5);
         }*/
         if(abs(l2-l2Current())<pot2_tol)
     {
         writeAct(act2, in, 0);
         flag2 = 1;
     }
     else
     {
        if(l2>l2Current())
        writeAct(act2, out, 255);
        else
        writeAct(act2, in, 255);
     }
     }

   }
  // writeAct(act1, in, 0);
  // writeAct(act2, in, 0);
   solveXY();
   #if PRINTARM
    cout<<"\nl1current "<<l1Current()<<"    l2Current "<<l2Current();
    #endif
}


void FINAL_ARM::setArm2()
{
     #if PRINTARM
    cout<<"\n current length l1 "<<l1Current()<<"   l2  "<<l2Current()<<"\n";
    #endif

	int flag = 0;  //flags to check the status of each actuator... whether reached the set position

    //if either one of the actuators not reached set position run the loop
    writeAct(act1, in, 200);
    writeAct(act2, in, 200);
   while(flag == 0)
    {
       uint8_t direction1, direction2;

       float l1_old = l1Current();
       float l2_old = l2Current();

       float change1=0, change2=0;

       float pwm1=1, pwm2=1;

       if(l1<l1Current()) direction1 = in;
       else direction1 = out;

       if(l2<l2Current()) direction2 = in;
       else direction2 = out;

            change1 = abs(l1_old - l1Current())/l1;
            change2 = abs(l2_old - l2Current())/l2;

            if(abs(change1 - change2) > 0.2)
                {
            if(change1 > change2)
            {
                pwm1 = 1;
                pwm2 = abs(pwm1 - 0.1);
                writeAct(act1, direction1, 255*pwm1);
                writeAct(act2, direction2, 255*pwm2);
            }

            if(change1 < change2)
            {
                pwm2 = 1;
                pwm1 = abs(pwm1 - 0.1);
                writeAct(act1, direction1, 255*pwm1);
                writeAct(act2, direction2, 255*pwm2);
            }
                }

            if(abs(l1-l1Current())<pot1_tol && abs(l2 - l2Current())<pot2_tol)   //if actuator length within tolerance limit then motor off
         {
             writeAct(act1, in, 0);
             writeAct(act2, in, 0);
             flag = 1;
         }
       }

   solveXY();   //update the current x and y position based on current actuator lengths
   #if PRINTARM
    cout<<"\nl1current "<<l1Current()<<"    l2Current "<<l2Current();
    #endif
}

void FINAL_ARM::setArm3()
{
	uint8_t direction1, direction2;
	float del1, del2;
	if(theta1>theta1Current()) direction1 = in;
	else direction1 = out;

	if(l2>l2Current()) direction2 = out;
	else direction2 = in;
	
	writeAct(act1, direction1, 255);
	writeAct(act2, direction2, 255);
	
	int flag1 =0; int flag2=0;
	
	while(flag1 == 0 || flag2 == 0)
	{
		del1 = theta1-theta1Current();
		if(sign(del1) < pot1_tol)
		{
			writeAct(act1, direction1, 0);
			flag1 = 1;
		}
		
		del2 = l2-l2Current();
		if(sign(del2) < pot2_tol)
		{
			writeAct(act2, direction2, 0);
			flag2 = 1;
		}	
	
	}

	
}


void FINAL_ARM::base(char dir, int pwm)
{
	uint8_t send = 0;
	if(dir == left) send = 32;
	else if(dir == right) send = 48;
	arm_arduino.write_byte('A');
        arm_arduino.write_byte((char)send);
	arm_arduino.write_byte('S');
	arm_arduino.write_byte((char)pwm);
	
}


void FINAL_ARM::digger(char dir, int pwm)
{
	uint8_t send = 0;
	if(dir == up) send = 128;
	else if(dir == down) send = 192;
	arm_arduino.write_byte('A');
        arm_arduino.write_byte((char)send);
	arm_arduino.write_byte('S');
	arm_arduino.write_byte((char)pwm);
	
}
