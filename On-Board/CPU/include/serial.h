/*********************************************************************************************\

Serial API Header for driving a generic serial device on a UNIX machine.
Megha Gupta, Alankar Kotwal

Log follows:

1) 15 Feb 2014: Alankar created this file with the struct and functions.
2) 16 Feb 2014: Alankar incorporated error checking. Compiled as a library.
3) 28 Feb 2014: Megha incorporated asynchronous mode.
4) 1 Mar 2014: Final working library complete.

\*********************************************************************************************/


#ifndef _SERIAL_H
#define _SERIAL_H

#include <wiringPi.h>
#include <wiringSerial.h>

class serial_device {
	int fd;

	public:

	serial_device();
	int open_port(const char* device, unsigned int baud_rate);
	int read_bytes(char* data, int num);
	int write_bytes(char* buf, int size);
	int available();
	void flush();
	int close_port();
};

#endif
