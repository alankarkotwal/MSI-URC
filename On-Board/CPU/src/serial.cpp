/*********************************************************************************************\

Serial API for driving a generic serial device on a UNIX machine. Function definitions.
Megha Gupta, Alankar Kotwal

Log: Refer to the header.

\*********************************************************************************************/


#include "serial.h"


// TODO: Need to look up built-in serial timeouts. This implementation wastes CPU resources!
// Found this: http://stackoverflow.com/questions/10522277/how-can-i-implement-timeout-for-read-when-reading-from-a-serial-port-c-c
// @Aman: Exactly what we're looking for, right?


bool serial_device::open_port(string port) {

	fd=open(port.c_str(), O_RDWR | O_NONBLOCK);
	port_name = port;
	
	if(fd==-1) {

		cout<< "serial_device::open_port: Unable to open serial device " << port << endl;
		return false;
	}
	return true;
}


void serial_device::configure_port(int baud_rate) {

	tcgetattr(STDOUT_FILENO, &old_stdio);

	memset(&stdio, 0, sizeof(stdio));
	stdio.c_iflag=0;
	stdio.c_oflag=0;
	stdio.c_cflag=0;
	stdio.c_lflag=0;
	stdio.c_cc[VMIN]=1;
	stdio.c_cc[VTIME]=0;
	tcsetattr(STDOUT_FILENO, TCSANOW, &stdio);
	tcsetattr(STDOUT_FILENO, TCSAFLUSH, &stdio);
	fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
 
	memset(&tio, 0, sizeof(tio));
	tio.c_iflag=0;
	tio.c_oflag=0;
	tio.c_cflag=CS8|CREAD|CLOCAL;
	tio.c_lflag=0;
	tio.c_cc[VMIN]=1;
	tio.c_cc[VTIME]=5;
	
	cfsetospeed(&tio, baud_rate);
	cfsetispeed(&tio, baud_rate);
	
	tcsetattr(fd, TCSANOW, &tio);
	
	sleep(2); 										// Required to make flush work
  tcflush(fd, TCIOFLUSH);

}


void serial_device::flush_port() {

	sleep(2); 										// Required to make flush work
  tcflush(fd, TCIOFLUSH);

}

void serial_device::write_bytes(char* data_byte, int length=1) {

	write(STDOUT_FILENO, &data_byte, length);

}


char serial_device::read_byte() {
	
	timeout_read.start();
	timeout_read.set_timer(SERIAL_TIMEOUT);
	char data;
	while(read(fd, &data, 1)<0) {
	
		if(timeout_read.check_timer())
			return '\0';
	
	}

	return data;

}


bool serial_device::read_bytes(char* buf, int num) {
	
	for(int i=0;i<num;i++) {
	
		int bytes;
		bytes=read(fd, buf, num);
		if(bytes!=num) {
		
			return false;
		
		}
	
	}
	
	return true;
	
}

void serial_device::close_port() {
	
	close(fd);
	tcsetattr(STDOUT_FILENO, TCSANOW, &old_stdio);

}

// The end.
