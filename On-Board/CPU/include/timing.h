/*********************************************************************************************\

Timing functionality on a UNIX machine.
Megha Gupta, Alankar Kotwal

Log follows:

1) 17 Feb 2014: Alankar created this file.

\*********************************************************************************************/

#ifndef _TIMING_H
#define _TIMING_H

#include<time.h>

#define F_CPU 700000000

class timer {
	clock_t old_time;
	clock_t present_time;
	clock_t countdown_time;
	bool enable;
	
  public:
	void start();
	clock_t get_time();
	void set_timer(clock_t timeout);
	bool check_timer();
	void disable_timer();
};

#endif