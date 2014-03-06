#include <iostream>
#include "serial.h"

using namespace std;

int main() {
	serial_device xbee;
	serial_device arduino;
	char a;
	int rpm_left, rpm_right, turn_left, turn_right;
	int MaxDCSpeed, LeftDCSpeed, RightDCSpeed, leftDir, rightDir;
	int RightFactor = 1, LeftFactor = 1;
	int TurnFactor = 1, DeadZone = 0;
	xbee.open_port("/dev/ttyAMA0");
	arduino.open_port("/dev/tty/USB0");
	char feedback[4];
	while(1){

		xbee.read_bytes(&a, 1);
		//cout<<(int)a<<endl;
		int b = (int)a;
		if (b <= 25 && b > 0) MaxDCSpeed = (int)(b*(255-128)/25);
		else if (b > 25 && b <= 50) MaxDCSpeed = (int)((b*(-128)/25) + 256);
		else if (b > 50 && b <= 75) {
			LeftDCSpeed = MaxDCSpeed - (int)TurnFactor*(b-50);
			RightDCSpeed = MaxDCSpeed + (int)TurnFactor*(b-50);
		//	RightDCSpeed = 255 - RightFactor*(RightDCSpeed-128)*(255/127);
		}
		else if (b > 75 && b <=100){
			LeftDCSpeed = MaxDCSpeed + (int)TurnFactor*(b-75);
			RightDCSpeed = MaxDCSpeed - (int)TurnFactor*(b-75);
		//	LeftDCSpeed = 255- LeftFactor*(LeftDCSpeed*(-255/128)+255);
		//	RightDCSpeed = 255 - RightFactor*(RightDCSpeed-128)*(255/127);
		}
		if (LeftDCSpeed < 128 - DeadZone){
			leftDir = 0; //Left wheels backward
			LeftDCSpeed = LeftDCSpeed;
		}
		else if(LeftDCSpeed > 128 + DeadZone){
			leftDir = 1; //Left Wheels forward
			LeftDCSpeed = 255- LeftFactor*((LeftDCSpeed-128)*(255/127));
		}
		else {
			leftDir = 0;
			LeftDCSpeed = 255;
		}
                if (RightDCSpeed < 128 - DeadZone){
                        rightDir = 0; //Left wheels backward
                        RightDCSpeed = RightDCSpeed;
                        RightDCSpeed = 255- RightFactor*(RightDCSpeed*(-255/128)+255);
                }
                else if(RightDCSpeed > 128 + DeadZone){
                        rightDir = 1; //Left Wheels forward
                        RightDCSpeed = 255- RightFactor*((RightDCSpeed-128)*(255/127));
                }
                else {
                       rightDir = 0;
                       RightDCSpeed = 255;
                }
                char ld = (char)leftDir;
                char rd = (char)rightDir;
                char ls = (char)(LeftDCSpeed);
                char rs = (char)(RightDCSpeed);
		//cout<<ld<<endl<<rd<<endl<<ls<<endl<<rs<<endl;
		arduino.write_bytes("s", 1);
                /*arduino.write_bytes(&ld, 1);
                arduino.write_bytes(&rd, 1);
		arduino.write_bytes(&ls, 1);
		arduino.write_bytes(&rs, 1);*/
		arduino.write_bytes("s", 1);
		arduino.write_bytes("a", 1);
                arduino.write_bytes("b", 1);
		arduino.write_bytes("c", 1);
		arduino.write_bytes("d", 1);
		arduino.read_bytes(feedback, 4);
		for(int i=0;i<4;i++) {
			cout<<(int)feedback[i]<<endl;
		}
		cout<<"*****"<<endl;
	}

	return 0;

}
