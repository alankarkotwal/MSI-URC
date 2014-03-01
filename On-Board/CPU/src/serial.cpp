/*********************************************************************************************\

Serial API for driving a generic serial device on a UNIX machine. Function definitions.
Megha Gupta, Alankar Kotwal

Log: Refer to the header.

\*********************************************************************************************/


#include "serial.h"


serial_device::serial_device() {
	fd = -1;
}

int serial_device::open_port(const char* device) {
	fd = open(device,  O_RDWR | O_NOCTTY | O_NONBLOCK );
}

void serial_device::configure_port(int baud_rate) {

	struct termios options;

    tcgetattr(fd, &options);
    bzero(&options, sizeof(options));

    cfsetispeed(&options, baud_rate);
    cfsetospeed(&options, baud_rate);

    options.c_cflag |= (baud_rate|CLOCAL|CREAD|CS8);
    options.c_iflag |= (IGNPAR|IGNBRK);
    options.c_cc[VTIME]=0;
    options.c_cc[VMIN]=1;
    tcsetattr(fd, TCSANOW, &options);
}

int serial_device::read_bytes(char* data, int num) {
	
	int i = read(fd, data, num);
	return 0;
}

int serial_device::write_bytes(char* buf, ssize_t size) {
  return write(fd, buf, size);
}

int serial_device::close_port() {
	return close(fd);
}

// The end.