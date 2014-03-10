#include <iostream>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <signal.h>
#include "rover.h"

using namespace std;

int reset_b=0;

int main() {

	wiringPiSetup();

	int inpA, inpB;
	int arduino, xbee;
	wheel w1(25, 0, 90);
	

// **************************************************************************

	start:

		cout<<"Start"<<endl;
		arduino=serialOpen("/dev/ttyUSB0",9600);
		xbee=serialOpen("/dev/ttyAMA0", 9600);


// **************************************************************************

	loop:

		cout<<"Loop"<<endl;
		inpA=(int)(serialGetchar(xbee));
		inpB=(int)(serialGetchar(xbee));
		cout<<inpA<<"   "<<inpB<<endl;
		if(inpA>0&&inpA<=25) {
			w1.direction=1;
			w1.rpmBase=(int)(255-255*inpA/25);
			if(w1.rpmBase>155) w1.rpmBase=255;
		}
		else if(inpA>25&&inpA<=50) {
			w1.direction=0;
			w1.rpmBase=(int)(255-255*(inpA-25)/25);
			if(w1.rpmBase>155) w1.rpmBase=255;
		}
		else if(inpA>50&&inpA<=75) {
			w1.angleLevel=(int)(4-(inpA-51)/6);
		}
		else if(inpA>75&&inpA<=100) {
			w1.angleLevel=(int)(5+(inpA-76)/6);
		}

		if(inpB>0&&inpB<=25) {
			w1.direction=1;
			w1.rpmBase=(int)(255-255*inpB/25);
			if(w1.rpmBase>155) w1.rpmBase=255;
		}
		else if(inpB>25&&inpB<=50) {
			w1.direction=0;
			w1.rpmBase=(int)(255-255*(inpB-25)/25);
			if(w1.rpmBase>155) w1.rpmBase=255;
		}
		else if(inpB>50&&inpB<=75) {
			w1.angleLevel=(int)(4-(inpB-51)/6);
		}
		else if(inpB>75&&inpB<=100) {
			w1.angleLevel=(int)(5+(inpB-76)/6);
		}

		w1.rpmCalc();

		serialPutchar(arduino, 0);
		serialPutchar(arduino, (char)w1.rpm_output);
		serialPutchar(arduino, (char)w1.direction);
		serialPutchar(arduino, (char)w1.angleLevel);

		serialFlush(xbee);

		delay(50);


// **************************************************************************

	if(!digitalRead(reset_b)) goto reset;
	else goto loop;


// **************************************************************************

	reset:

		cout<<"Reset"<<endl;

		w1.direction=(int)1;
		w1.rpm_output=(int)255;
		w1.angleLevel=(int)5;

		serialPutchar(arduino, 0);
		serialPutchar(arduino, (char)w1.rpm_output);
		serialPutchar(arduino, (char)w1.direction);
		serialPutchar(arduino, (char)w1.angleLevel);

		serialClose(xbee);
		serialClose(arduino);

		while(!digitalRead(reset_b));
		goto start;


// **************************************************************************

	return 0;

}
