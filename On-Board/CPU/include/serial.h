/*********************************************************************************************\

Serial API Header for driving a generic serial device on a UNIX machine.
Megha Gupta, Alankar Kotwal

Log follows:

1) 15 Feb 2014: Alankar created this file with the struct and functions.
2) 16 Feb 2014: Alankar incorporated error checking. Compiled as a library.

\*********************************************************************************************/

#ifndef _SERIAL_H
#define _SERIAL_H

#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<termios.h>
#include <iostream>
#include <cstring>
#include <string>
#include "timing.h"
using namespace std;

#define SERIAL_TIMEOUT F_CPU/1000

class serial_device {

	int fd;
	string port_name;
	int baud_rate;
	struct termios tio;
	struct termios stdio;
	struct termios old_stdio;
	
	timer timeout_read;
	timer timeout_write;

  public:
	bool open_port(string port_name);
	void configure_port(int baud_rate);
	char read_byte();
	void write_bytes(char* buf, int length);
	bool read_bytes(char* buf, int num);
	void flush_port();
	void close_port();

};

serial_device arduino_mega, gps, xbee;

#endif
