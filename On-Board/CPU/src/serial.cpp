/*********************************************************************************************\

Serial API for driving a generic serial device on a UNIX machine. Function definitions.
Megha Gupta, Alankar Kotwal

Log: Refer to the header.

\*********************************************************************************************/


#include "serial.h"
#define _POSIX_SOURCE 1 

serial_device::serial_device(int type) {

	device_type=type;
}


void signal_handler_IO_arduino(int);
void signal_handler_IO_xbee(int);
void signal_handler_IO_gps(int);


void (*function_pointer[SERIAL_NO])(int)={signal_handler_IO_arduino, signal_handler_IO_xbee, signal_handler_IO_gps};


bool serial_device::open_port(string port) {

	fd=open(port.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);

	if(fd<0) {

		perror(port.c_str());
		cout<< "serial_device::open_port: Unable to open serial device " << port << endl;
		return false;
	}
	wait_flag = 1;
	saio.sa_handler = function_pointer[device_type];
	sigemptyset(&saio.sa_mask);
	saio.sa_flags = 0;
	saio.sa_restorer = NULL;
	sigaction(SIGIO, &saio, NULL);
	fcntl(fd, F_SETOWN, getpid());
	fcntl(fd, F_SETFL, FASYNC);
	return true;
}


void serial_device::configure_port(int baud_rate) { //the baudrate should be given as B9600

	tcgetattr(fd, &oldtio);
	
	newtio.c_cflag = baud_rate | CRTSCTS | CS8 | CLOCAL | CREAD;
	newtio.c_iflag = IGNPAR | ICRNL;
	newtio.c_oflag = 0;
	newtio.c_lflag = ICANON;
	newtio.c_cc[VMIN]=1;
	newtio.c_cc[VTIME]=0;
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd,TCSANOW,&newtio);


/*
	memset(&stdio, 0, sizeof(stdio));
	stdio.c_iflag=0;
	stdio.c_oflag=0;
	stdio.c_cflag=0;
	stdio.c_lflag=0;
	stdio.c_cc[VMIN]=1;
	stdio.c_cc[VTIME]=0;
	tcsetattr(fd, TCSANOW, &stdio);
	tcsetattr(fd, TCSAFLUSH, &stdio);
	fcntl(fd, F_SETFL, O_NONBLOCK);
 
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

	sleep(2);*/ 										// Required to make flush work
//	tcflush(fd, TCIOFLUSH);

}


void serial_device::flush_port() {

	sleep(2); 										// Required to make flush work
	tcflush(fd, TCIOFLUSH);

}

void serial_device::write_bytes(char* data_byte, int length=1) {

	char c[] = {'\r'};
	write(fd, &data_byte, length);
	write(fd, &c, 1);

}


//char serial_device::read_byte() {

	
	/*timeout_read.start();
	timeout_read.set_timer(SERIAL_TIMEOUT);
	char data;
	while(read(fd, &data, 1)<0) {

		if(timeout_read.check_timer())
			return '\0';

	}

	return data;*/

//}


void serial_device::read_bytes(char* buffer, int length) {

 res = read(fd, buffer, length);
 buffer[res]=0;

/*	for(int i=0;i<num;i++) {

		int bytes;
		bytes=read(fd, buf, num);
		if(bytes!=num) {

			return false;

		}

	}

	return true;
	*/

}

void serial_device::close_port() {

	close(fd);

}


// The end.
