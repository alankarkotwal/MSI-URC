#include <iostream>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <signal.h>

using namespace std;

int permission;

int main() {

	int inp;
	int temp;

	char Direction=(char)0;
	char DCSpeed=(char)0;
	char AngleLevel=(char)5;

	int arduino=serialOpen("/dev/ttyUSB0",9600);
	int xbee=serialOpen("/dev/ttyAMA0", 9600);

	wiringPiSetup();

	while(1) {

		inp=(int)(serialGetchar(xbee));
		if(inp>0&&inp<=25) {
			Direction=(char)1;
			DCSpeed=(char)(255-255*inp/25);
			if(DCSpeed>155) DCSpeed=255;
		}
		else if(inp>25&&inp<=50) {
			Direction=(char)0;
			DCSpeed=(char)(255-255*(inp-25)/25);
			if(DCSpeed>155) DCSpeed=255;
		}
		else if(inp>50&&inp<=75) {
			AngleLevel=(char)(4-(inp-51)/6);
		}
		else if(inp>75&&inp<=100) {
			AngleLevel=(char)(5+(inp-76)/6);
		}

		cout<<(int)DCSpeed<<"   "<<(int)AngleLevel<<"   "<<(int)Direction<<endl;

		serialPutchar(arduino, 0);
		serialPutchar(arduino, DCSpeed);
		serialPutchar(arduino, Direction);
		serialPutchar(arduino, AngleLevel);

		serialFlush(xbee);

		delay(50);

	}

	return 0;

}
