#include <iostream>
#include <wiringPi.h>
#include "serial.h"
#include "rover.h"

using namespace std;

int reset_b=0;

int main() {

	wiringPiSetup();

	serial_device xbee, arduino;

	int inpA, inpB;
	char tempA, tempB;
	char temp;

	wheel w1(25, 0, 90);


// **************************************************************************

	start:

//		cout<<"Start"<<endl;
		digitalWrite(1, LOW);
		digitalWrite(2, LOW);
		arduino.open_port("/dev/ttyUSB0",9600);
		xbee.open_port("/dev/ttyAMA0", 9600);
		digitalWrite(1, HIGH);
		digitalWrite(2, HIGH);


// **************************************************************************

	loop:

//		cout<<"Loop"<<endl;
		digitalWrite(1, HIGH);
		digitalWrite(2, HIGH);

		if(xbee.available()>1) {

			xbee.read_bytes(&tempA, 1);
			xbee.read_bytes(&tempB, 1);
			inpA=(int)tempA;
			inpB=(int)tempB;
			cout<<inpA<<"   "<<inpB<<endl;
			if(inpA>=0&&inpA<=25) {
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
		}

		else {
			w1.rpmBase=255;
			w1.angleLevel=5;
			w1.direction=1;
		}

		w1.rpmCalc();

		temp=(char)0;
		arduino.write_bytes(&temp, 1);
		temp=(char)w1.rpm_output;
		arduino.write_bytes(&temp, 1);
		temp=(char)w1.direction;
		arduino.write_bytes(&temp, 1);
		temp=(char)w1.angleLevel;
		arduino.write_bytes(&temp, 1);

		xbee.flush();

		delay(50);


// **************************************************************************

	if(!digitalRead(reset_b)) goto reset;
	else goto loop;


// **************************************************************************

	reset:

//		cout<<"Reset"<<endl;
		digitalWrite(1, LOW);
		digitalWrite(2, LOW);

		w1.direction=(int)1;
		w1.rpm_output=(int)255;
		w1.angleLevel=(int)5;

		temp=(char)0;
		arduino.write_bytes(&temp, 1);
		temp=(char)w1.rpm_output;
		arduino.write_bytes(&temp, 1);
		temp=(char)w1.direction;
		arduino.write_bytes(&temp, 1);
		temp=(char)w1.angleLevel;
		arduino.write_bytes(&temp, 1);

		xbee.flush();
		arduino.flush();

		xbee.close_port();
		arduino.close_port();

		while(!digitalRead(reset_b));
		goto start;


// **************************************************************************

	return 0;

}
