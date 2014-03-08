#include <iostream>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <signal.h>
#include "rover.h"

using namespace std;

int permission;

int main() {

	int inp;
	int temp;

	//char Direction=(char)0;
	//char DCSpeed=(char)0;
	//char AngleLevel=(char)5;
	wheel w1(25, 0, 90);

	int arduino=serialOpen("/dev/ttyUSB0",9600);
	int xbee=serialOpen("/dev/ttyAMA0", 9600);

	wiringPiSetup();

	while(1) {

		inp=(int)(serialGetchar(xbee));
		if(inp>0&&inp<=25) {
			w1.direction=1;
			w1.rpmBase=(int)(255-255*inp/25);
			if(w1.rpmBase>155) w1.rpmBase=255;
		}
		else if(inp>25&&inp<=50) {
			w1.direction=0;
			w1.rpmBase=(int)(255-255*(inp-25)/25);
			if(w1.rpmBase>155) w1.rpmBase=255;
		}
		else if(inp>50&&inp<=75) {
			w1.angleLevel=(int)(4-(inp-51)/6);
		}
		else if(inp>75&&inp<=100) {
			w1.angleLevel=(int)(5+(inp-76)/6);
		}

		cout<<w1.rpmBase<<"   "<<w1.angleLevel<<"   "<<w1.direction<<endl;
		w1.rpmCalc();

		serialPutchar(arduino, 0);
		serialPutchar(arduino, (char)w1.rpmOutput);
		serialPutchar(arduino, (char)w1.direction);
		serialPutchar(arduino, (char)w1.angleLevel);

		serialFlush(xbee);

		delay(50);

	}

	return 0;

}
