// arm integration 12 May 2014


#include <iostream>
#include "serial.h"
#include "encoding.h"
#include "decoding.h"
#include "arm_rpi.h"
#include "config.h"

serial_device xbee;

FINAL_ARM arm;
Encoding encoding;
Decoding decoding;
#define Rachana_Debug 1
#define DEBUG
int num;
int actionIDSize, currentActionID;

using namespace std;

int main() {

	xbee.open_port("/dev/ttyUSB0", 19200);
	arm.arm_arduino.open_port("/dev/ttyACM0", 9600);
	
	cout<<"moving actuator...\n";
	arm.writeAct(arm.act2, arm.in, 255);
	delay(1000);
	arm.writeAct(arm.act2, arm.in, 0);

/*	while(1){
		num = xbee.available();
		if (num) {
			cout<<xbee.read()<<endl;
		}		
	}*/
//	cout<<arm.theta1Current()<<endl;
//	arm.solveXY();

	while(1) {
	//cout<<"always printing"<<endl;

		#ifdef DEBUG
	//	xbee.flush();
	//	while(!xbee.available());
		#endif
//	cout<<xbee.read()<<endl;	
	num=xbee.available();
		if(num) {
			#ifdef DEBUG
//			cout<<"Input received: \t";
			#endif
			for(int i=0;i<num;i++) {
				char tempXbee=xbee.read();
				//cout<<tempXbee<<"  ";
//				cout<<arm.arm_arduino.read()<<endl;
				decoding.putNewData(tempXbee);
//				cout<<(int)tempXbee<<" ";
			}
			cout<<endl;
		}
//		cout<<"dataqueue size : -> " << decoding.getdataqueueSize()<<endl;

		decoding.parseIt();
		actionIDSize = decoding.getactionIDlistSize();

		for(int i=0; i<actionIDSize; i++){
			currentActionID=decoding.getCurrentActionID();
                
		if(currentActionID==ID_ROBOTIC_ARM) {
//				cout<<"current action id is robotic arm"<<endl;
				#if Rachana_Debug
				cout<<"Robotic Arm X -> "<<(int)decoding.ROBOTIC_ARM_X<<"  :  \t";
				cout<<"Robotic Arm Y -> "<<(int)decoding.ROBOTIC_ARM_Y<<"  :  \t";
				cout<<"Robotic Arm D -> "<<(int)decoding.ROBOTIC_ARM_D<<"  :  \t";
				cout<<"Robotic Arm B -> "<<(int)decoding.ROBOTIC_ARM_B<<endl;
				#endif
				switch(decoding.ROBOTIC_ARM_X)
				{
					case 19:
							#ifdef ARM_XY
							arm.x+= arm.ds;
							if(arm.x > arm.xmax)	arm.x = arm.xmax;
							arm.solveL1L2();
							arm.setArm3();
							#else
							arm.writeAct(arm.act1, arm.in, 255);
							cout<<arm.theta1Current()<<"\t"<<arm.readPot(arm.act1)<<endl;
							#endif
						    
						    break;

					case 11: 
							#ifdef ARM_XY
							arm.x-= arm.ds;
							if(arm.x < arm.xmin)	arm.x = arm.xmin;
							arm.solveL1L2();
							arm.setArm3();
							#else
							arm.writeAct(arm.act1, arm.out, 255);
						 	#endif 
						    break;

					case 0:  arm.writeAct(arm.act1, arm.out, 0);
						 arm.writeAct(arm.act2, arm.out, 0);
						 break;
				}

				switch(decoding.ROBOTIC_ARM_Y)
				{
					case 29: 
							#ifdef ARM_XY
							arm.y+= arm.ds;
							if(arm.y > arm.ymax)	arm.y = arm.ymax;
							arm.solveL1L2();
							arm.setArm3();
							#else
							arm.writeAct(arm.act2, arm.in, 255);
							cout<<arm.l2Current()<<endl;
							#endif
							
						    break;
						    
					case 21:
							#ifdef ARM_XY
							arm.y-= arm.ds;
							if(arm.y < arm.ymin)	arm.y = arm.ymin;
							arm.solveL1L2();
							arm.setArm3();
							#else
							arm.writeAct(arm.act2, arm.out, 255);
							cout<<arm.l2Current()<<endl;
							#endif
							
						
						    break;
						    
					case 0:  arm.writeAct(arm.act2, arm.out, 0);
						 arm.writeAct(arm.act1, arm.out, 0);
						 break;
				}
				
				switch(decoding.ROBOTIC_ARM_D)
				{
					case 39: 
							arm.digger(arm.up, 125);
							break;
							
					case 31: 
							arm.digger(arm.down, 125);
							break;
							
					case 0: 
							arm.digger(arm.down, 0);
							break;
						
				}
				
				switch(decoding.ROBOTIC_ARM_B)
				{
					case 49: 
							arm.base(arm.left, 160);
							break;
							
					case 41: 
							arm.base(arm.right, 160);
							break;
							
					case 0: 
							arm.base(arm.right, 0);
							break;
						
				}
				
				
				
				decoding.actionDone();
			}
				else {
//				cout<<"other IDs Found"<<endl;
				decoding.actionDone();
				}

		}
//		delay(50);
	}
}
