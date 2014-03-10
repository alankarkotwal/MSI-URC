/*********************************************************************************************\

Robotic Arm Code, MSI-URC 2014
Rachana Agrawal

1) 7 March 2014 : code for 2D arm movement 

\*********************************************************************************************/

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringSerial.h>


class ARM
{
    public:
        int serial;
        
        /********constants********/
        
        //arm parameters
        float d = 285, b = 110, a1=435, a2=430, b1=70, c1=90, b2=85;  //dimensions of arm
        int l1max = 150, l2max = 150;                              //maximum length that actuators can extract

        //coordinates
        float xmin = 200, xmax = 400, ymin = 0, ymax = 600;        //x and y coordinates of the corners of the square
        float ds = 20;                  //steps in which x and y coordinates change

        /******variables*******/

        //geometrical
        float x,y,r ;
        float delL1, delL2, d1, d2;  //coordinates of tip of arm(x,y) ; extracted length(l1,l2) ; required change in length
        unsigned int l1, l2;
        float theta1, theta2;   //angles at the shouler and elbow joints

        //pot calibration
        float read1=0, read2=0, read1_min_1=0, read1_max_1=0, read1_min_2=0, read1_max_2=0, read2_min_1=0, read2_max_1=0, read2_min_2=0, read2_max_2=0 ;  //read1_min_2 - 2nd set of reading for pot 1 min postion
        float scale_factor1_1=0, scale_factor1_2=0, scale_factor2_1=0, scale_factor2_2=0;
        float pot1_min=0, pot1_max=0, pot2_min=0, pot2_max=0, scale_factor1=0, scale_factor2=0 ;  //minimum and maximum output of pot of actuators
        float pot1_tol=1, pot2_tol=1;  //tolerence level of potentiometer readings
        float val;
        float tol_min_1 = 2, tol_min_2 = 2;

        clock_t t;

        //serial communication
        const char  pot = 'P';
        
        uint8_t act1 = 0, act2 = 1, in = 0, out = 1;

        void writeAct(uint8_t act, uint8_t direction, uint8_t pwm);

        int readPot(uint8_t act);

        void calibrate();

        float l1Current();

        float l2Current();

        //function to get x,y coordinates of tip of arm from l1, l2
        void solveXY() ; //calculation part working properly

        //function to get l1,l2 from x,y coordinates of tip of arm using inverse kinematics
        void solveL1L2();    //working properly

        void setArm();

};
