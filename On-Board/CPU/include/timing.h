/*********************************************************************************************\

Timing functionality on a UNIX machine.
Megha Gupta, Alankar Kotwal

Log follows:

1) 17 Feb 2014: Alankar created this file.

\*********************************************************************************************/

#ifndef _TIMING_H
#define _TIMING_H

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

class timeout {

    public:
        timeout();
        void init_timer();
        unsigned long int time_ms();

    private:    
        struct timeval start_time;
        struct timeval prev_time;

};

#endif
