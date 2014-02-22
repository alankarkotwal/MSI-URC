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
#include"../timing/timing.h"

#define SERIAL_TIMEOUT F_CPU/1000

struct serial_device {

	int fd;
	char port_name[20];
	int baud_rate;
	struct termios tio;
	struct termios stdio;
	struct termios old_stdio;
	
	struct timer timeout_read;
	struct timer timeout_write;
		
	void open_port(char* port_name);
	void configure_port(int baud_rate);
	void write_byte(char data);
	char read_byte();
	void write_bytes(char*);
	bool read_bytes(char* buf, int num);
	void flush_port();
	void close_port();

};

serial_device arduino_mega, gps, xbee;

#endif
