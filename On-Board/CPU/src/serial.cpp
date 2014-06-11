/*********************************************************************************************\

Serial API for driving a generic serial device on a UNIX machine. Function definitions.
Megha Gupta, Alankar Kotwal

Log: Refer to the header.

\*********************************************************************************************/

#include<iostream>
#include<cstdlib>
#include "serial.h"


serial_device::serial_device() {
	fd = -1;
}

int serial_device::open_port(const char* device, unsigned int baud_rate) {
	fd=serialOpen(device, baud_rate);
	if(fd==-1) {
		std::cout<<"Failed to open serial port "<<device<<std::endl;
		std::cout<<"Did you connect "<<name<<"?"<<std::endl<<std::endl;
//		exit(0);
	}
	serialFlush(fd);
}


int serial_device::read_bytes(char* data, int num) {

	for(int i=0;i<num;i++) {
		data[i]=serialGetchar(fd);
		if((int)data[i]==-1) {
			return 0;
		}
	}
	return 1;
}

char serial_device::read() {
	char temp;
	read_bytes(&temp, 1);
	return temp;
}

int serial_device::write_bytes(char* buf, int size) {

	for(int i=0;i<size;i++) {
		serialPutchar(fd, buf[i]);
	}
}

int serial_device::write_byte(char data)	{
	serialPutchar(fd, data);

}

int serial_device::available() {
	return serialDataAvail(fd);
}

void serial_device::flush() {
	serialFlush(fd);
}

int serial_device::close_port() {
	serialClose(fd);
}

// The end.
