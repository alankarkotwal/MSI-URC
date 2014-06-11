
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include"serial.h"

#include<stdint.h>
#include<math.h>

/********constants********/



class FINAL_ARM
{
    public:
        serial_device arm_arduino;
    //arm parameters
const float d = 285.0, b = 105.0, a1=350.0, a2=495.0, b1=225.0, c1=560.0, b2=150.0, c2=325.0, e=120.0, A= 0.3178237, B = 0.202269, C = -0.087266;  //dimensions of arm
const float l1max = 150.0, l2max = 150.0;                              //maximum length that actuators can extract
const float PI = 3.141592654;

//coordinates
const float xmin = 400.0, xmax = 800.0, ymin = -200.0, ymax = 600.0;        //x and y coordinates of the corners of the square
const float ds = 10.0;                  //steps in which x and y coordinates change

/******variables*******/

//geometrical
float x,y,z,r ;
//coordinates of tip of arm(x,y) ; extracted length(l1,l2) ; required change in length
float l1, l2;
float theta1, theta2, theta3, t1, t2, t3;   //angles at the shouler and elbow joints
float angle;

//pot calibration
float pot1_min, pot1_max, pot2_min, pot2_max, scale_factor1, scale_factor2 ;  //minimum and maximum output of pot of actuators
float pot1_tol, pot2_tol;  //tolerence level of potentiometer readings
float val;
float tol_min_1 , tol_min_2 ;

//arduino communication
const char  pot = 'P';
const uint8_t act1 = 0, act2 = 1, in = 0, out = 1;
const uint8_t left = 0, right = 1, up = 0, down = 1;

FINAL_ARM()
{
    pot1_min=510.0;
    pot1_max=656.0;
    pot2_min=0.0;
    pot2_max=146.0;
    scale_factor1=0.1019/(pot1_max-pot1_min);
    scale_factor2=146.0/105.0;  //minimum and maximum output of pot of actuators
    pot1_tol=2.0;
    pot2_tol=2.0;  //tolerence level of potentiometer readings
    tol_min_1 = 1.0;
    tol_min_2 = 1.0;
}

void writeAct(uint8_t act, uint8_t direction, uint8_t pwm);

int readPot(uint8_t act);

float theta1Current();

float l1Current();
 
float l2Current();


//function to get x,y coordinates of tip of arm from l1, l2
void solveXY() ; //calculation part working properly

//function to get l1,l2 from x,y coordinates of tip of arm using inverse kinematics
void solveL1L2();    //working properly


void setArm();

void setArm2();

void setArm3();

void base(char dir, int pwm);

void digger(char dir, int pwm);
};



