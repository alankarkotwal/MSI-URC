#include"serial.h"
#include<iostream>

using namespace std;

serial_device arduino;

int main() {

	arduino.open_port("/dev/ttyUSB0", 9600);
	while(1) {
		if(arduino.available()) {
			cout<<(int)arduino.read()<<endl;
		}
	}
}
