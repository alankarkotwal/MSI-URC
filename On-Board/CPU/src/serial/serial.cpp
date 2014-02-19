/*********************************************************************************************\

Serial API for driving a generic serial device on a UNIX machine. Function definitions.
Megha Gupta, Alankar Kotwal

Log: Refer to the header.

\*********************************************************************************************/


#include"serial.h"


// TODO: Need to look up built-in serial timeouts. This implementation wastes CPU resources!


void serial_device::open_port(char* port) {

	fd=open(port, O_RDWR | O_NONBLOCK);
	port_name=port;
	
	if(fd==-1) {

		printf("serial_device::open_port: Unable to open serial device %s.", port);
		return -1;

	}
	
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

void serial_device::write_byte(char data_byte) {

	write(STDOUT_FILENO, &data_byte, 1);

}


char serial_device::read_byte() {
	
	timeout_read.start();
	timeout_read.set_timer(SERIAL_TIMEOUT);
	char data;
	while(read(fd, &data, 1)<0) {
	
		if(timeout_read.check_timer()) {
		
			return NULL;
	
	}

	return data;

}


bool serial_device::read_bytes(char* buf, int num) {
	
	for(int i=0;i<num;i++) {
	
		buf[i]=read_byte();
		if(buf[i]==NULL) {
		
			return -1;
		
		}
	
	}
	
	return 1;
	
}


void serial_device::write_bytes(char* buf) {
	
	for(int i=0;i<strlen(buf);i++) {
	
		write_byte(buf[i]);
	
	}
	
}


void serial_device::close_port() {
	
	close(fd);
	tcsetattr(STDOUT_FILENO, TCSANOW, &old_stdio);

}

// The end.
