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

#include "timing.h"
#include <sys/time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <termios.h>
#include <string.h>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>


class serial_device {

    public:
        serial_device();
        ~serial_device();

        char open_port(const char *device, const unsigned int baud_rate=B9600);
        void close_port();
        char write_byte(char);
        char read_byte(char *data, const unsigned int timeout_ms=NULL);
        int available();
        char write_bytes (const void *buffer, const unsigned int number_bytes);
        int read_bytes(void *buffer, unsigned int max_bytes, const unsigned int timeout_ms=NULL);
        void flush_receiver();

    private:
        int fd;

};


#endif