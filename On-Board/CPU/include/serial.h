/*********************************************************************************************\

Serial API Header for driving a generic serial device on a UNIX machine.
Megha Gupta, Alankar Kotwal

Log follows:

1) 15 Feb 2014: Alankar created this file with the struct and functions.
2) 16 Feb 2014: Alankar incorporated error checking. Compiled as a library.
3) 28 Feb 2014: Megha incorporated asynchronous mode.

\*********************************************************************************************/

#ifndef _SERIAL_H
#define _SERIAL_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <iostream>
#include <cstring>
#include <string>
#include "timing.h"
#include <sys/signal.h>
#include <sys/types.h>

#define SERIAL_NO 3

using namespace std;

#define SERIAL_TIMEOUT (F_CPU/1000)

class serial_device {

	int fd, c, res;
//	string port_name;
	int baud_rate;
	char buff[255];
	struct termios newtio;
	struct termios oldtio;
	struct sigaction saio;
//	timer timeout_read;
//	timer timeout_write;

	int device_type;
	

  public:

	bool wait_flag;

	bool open_port(string port_name);
	void configure_port(int baud_rate);
//	char read_byte();
	void write_bytes(char* buf, int length);
	void read_bytes(char* buf, int num);
	void flush_port();
	void close_port();

	serial_device(int type);

};

#endif
