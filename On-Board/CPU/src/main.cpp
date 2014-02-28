#include<iostream>
#include"serial.h"
#include<unistd.h>

using namespace std;

serial_device arduino(0);
serial_device xbee(1);
serial_device gps(2);


void signal_handler_IO_arduino(int status) {
	
	arduino.wait_flag = 0;
	cout<<"b"<<endl;

}

void signal_handler_IO_xbee(int status) {
	
	xbee.wait_flag = 0;

}


void signal_handler_IO_gps(int status) {
	
	gps.wait_flag = 0;

}

int main() {

	arduino.open_port("/dev/ttyUSB1");
	arduino.configure_port(B9600);

	char container[50];

	while(1) {

		cout<<"a"<<endl;
		//arduino.read_bytes(container, 50);
		//cout<<container<<endl;
		arduino.write_bytes("aloo",4);
		usleep(1000000);
	}

	arduino.close_port();
}
