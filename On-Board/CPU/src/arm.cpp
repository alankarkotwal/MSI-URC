/*********************************************************************************************\

Robotic Arm Code, MSI-URC 2014
Rachana Agrawal

Log: Refer to header.

\*********************************************************************************************/

#include<iostream>
#include"arm.h"
#include<math.h>



//send command to arduino to write to actuators
void ARM::writeAct(uint8_t act, uint8_t direction, uint8_t pwm)
{
	uint8_t send = 0;
	send = 8 + act*pow(2,2) + direction*pow(2,1) ;
	serial.WriteByte('T');  //header 
  serial.WriteByte(send);
	serial.WriteByte('S');
	serial.WriteByte(pwm);

}

//send command to arduino to read actuator feedback
int ARM::readPot(uint8_t act)
{
  byte value;
	int pot_value = 0;
	uint8_t send = 0;
	send =  act*pow(2,2);
	serial.WriteByte('T');
	serial.WriteByte(send);
	label:
		if(serial.IsOpen() && serial.BytesInComm()>0 )
	    {
		byte header;
		serial.ReadByte(header);
		if(header == pot)
        {
            while((serial.IsOpen() && serial.BytesInComm()>0) == 0);
       		if(serial.IsOpen() && serial.BytesInComm()>0)
            {
                serial.ReadByte(value);
                pot_value = value;
                while((serial.IsOpen() && serial.BytesInComm()>0) == 0);
                if(serial.IsOpen() && serial.BytesInComm()>0)
                    serial.ReadByte(value);
                pot_value = pot_value + value*256;
            }
            else goto label;
        }
        else goto label;
		}
		else goto label;

	return pot_value;
}

//autocalibration of actuators
void ARM::calibrate()
{
	//retract fully
	writeAct(act1, in, 255);
	writeAct(act2, in, 255);

	t = clock();
	while( ((float)(clock() - t)/CLOCKS_PER_SEC) < 40); //buffer time of 40s is provided to ensure actuator is at extreme position
	writeAct(act1, in, 0);
	writeAct(act2, in, 0);
    
	for(int i=0 ; i<10 ; i++)
		 {
  			read1 = read1 + readPot(act1);
			  read2 = read2 + readPot(act2);
 		 }
 	read1_min_1 = read1/10;
 	read2_min_1 = read2/10;

 	//extract fully
	writeAct(act1, out, 255);
	writeAct(act2, out, 255);
	t = clock();
	while( ((float)(clock() - t)/CLOCKS_PER_SEC) < 40);
	writeAct(act1, out, 0);
	writeAct(act2, out, 0);

	read1 = 0;
	read2 = 0;
    for(int i=0 ; i<10 ; i++)
 		{
  			read1 = read1 + readPot(act1);
  			read2 = read2 + readPot(act2);
 		}
 	read1_max_1 = read1/10;
 	read2_max_1 = read2/10;

	t = clock();
	while( ((float)(clock() - t)/CLOCKS_PER_SEC) < 5);

    read1 = 0;
    read2 = 0;
 	for(int i=0 ; i<10 ; i++)
 	{
  		read1 = read1 + readPot(act1);
  		read2 = read2 + readPot(act2);
 	}
 	read1_max_2 = read1/10;
 	read2_max_2 = read2/10;

//retract fully
	writeAct(act1, in, 255);
	writeAct(act2, in, 255);
	t = clock();
	while( ((float)(clock() - t)/CLOCKS_PER_SEC) < 40);
	writeAct(act1, in, 0);
	writeAct(act2, in, 0);

  read1 = 0;
  read2 = 0;
  for(int i=0 ; i<10 ; i++)
 {
  read1 = read1 + readPot(act1);
  read2 = read2 + readPot(act2);
 }
 read1_min_2 = read1/10;
 read2_min_2 = read2/10;

//set parameters of arm feedback
 scale_factor1_1 = (read1_max_1 - read1_min_1)/150 ;
 scale_factor1_2 = (read1_max_2 - read1_min_2)/150 ;
 scale_factor1 = (scale_factor1_1 + scale_factor1_2)/2 ;
 pot1_max = (read1_max_1 + read1_max_2)/2;
 pot1_min = (read1_min_1 + read1_min_2)/2;

 pot1_tol = (abs(pot1_min - read1_min_1) + abs(scale_factor1 - scale_factor1_1)*150)/scale_factor1 + tol_min_1;

 scale_factor2_1 = (read2_max_1 - read2_min_1)/150 ;
 scale_factor2_2 = (read2_max_2 - read2_min_2)/150 ;
 scale_factor2 = (scale_factor2_1 + scale_factor2_2)/2 ;
 pot2_max = (read2_max_1 + read2_max_2)/2;
 pot2_min = (read2_min_1 + read2_min_2)/2;

 pot2_tol = (abs(pot2_min - read2_min_1) + abs(scale_factor2 - scale_factor2_1)*150)/scale_factor2 + tol_min_2;
 
}

//current actuator length 
float ARM::l1Current()
{
	float pot = 0;
  	for(int i =0; i<10; i++)
  	{
    	pot = pot + readPot(act1);
  	}

 	float val = ( pot/10 - pot1_min)/scale_factor1;
 	if ( val>150) val = 150;
 	if ( val<0) val =0;

 	return val;
}


float ARM::l2Current()
{
	float pot = 0;
  	for(int i =0; i<10; i++)
  	{
    	pot = pot + readPot(act2);
  	}

 	float val = ( pot/10 - pot2_min)/scale_factor2;
 	if ( val>150) val = 150;
 	if ( val<0) val =0;

 	return val;
}


//function to get x,y coordinates of tip of arm from l1, l2
void ARM::solveXY()  //calculation part working properly
{
  l1 = l1Current();
  l2 = l2Current();
  theta1 = acos((b*b + (a1-c1)*(a1-c1) - (d+l1)*(d+l1))/(2*b*(a1-c1)));
  theta2 = acos((b2*b2 + (a1-b1)*(a1-b1) - (l2+d)*(l2+d) - 50*50)/(2*b2*(a1-b1)));

  x = a2*cos(theta2-theta1) - a1*cos(theta1);

  y = a1*sin(theta1) + a2*sin(theta2 - theta1);

}

//function to get l1,l2 from x,y coordinates of tip of arm using inverse kinematics
void ARM::solveL1L2()    //working properly
{
  r = sqrt(x*x+y*y);
  l1 = sqrt(b*b + (a1-c1)*(a1-c1) + 2*b*(a1-c1)*cos(acos((a1*a1+r*r-a2*a2)/(2*r*a1))+atan(y/x))) - d;
  l2 = sqrt(b2*b2 + (a1-b1)*(a1-b1) - 2*b2*(a1-b1)*((a2*a2+a1*a1-r*r)/(2*a1*a2)) - 50*50) - d;
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

}

void ARM::setArm()
{

	int flag1 = 0;
	int flag2 = 0;
	while(flag1 == 0 || flag2 == 0)
   {
       if(flag1 == 0)
     {
         if(abs(l1-l1Current())<15)
         {
            if(l1>l1Current())
            writeAct(act1, out, 255*0.3); //reduce the pwm when actuator is within a 15mm of set value
            else
            writeAct(act1, in, 255*0.3);
         }
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
         if(abs(l2-l2Current())<15)
         {
            if(l2>l2Current())
            writeAct(act2, out, 255*0.3);
            else
            writeAct(act2, in, 255*0.3);
         }
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
   writeAct(act1, in, 0);
   writeAct(act2, in, 0);
   solveXY();

}
