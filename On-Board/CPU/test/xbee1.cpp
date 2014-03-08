#include <iostream>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <signal.h>

using namespace std;

int reset_b=0;

int main() {

	wiringPiSetup();

	int xbee=serialOpen("/dev/ttyAMA0", 9600);
	goto reset;

	loop:
		serialPutchar(xbee,'a');
		// Loop code

	if(!digitalRead(reset_b)) goto reset;
	else goto loop;

	reset:
		// Reset routine
		while(!digitalRead(reset_b)) serialPutchar(xbee,'b');
		goto loop;

	return 0;

}
