/*********************************************************************************************\

Timing functionality on a UNIX machine. Function definitions.
Megha Gupta, Alankar Kotwal

Log: Refer to the header.

\*********************************************************************************************/


#include"timing.h"


void timer::start() {

	old_time=clock();

}


clock_t timer::get_time() {

	present_time=clock();
	return present_time;

}


void timer::set_timer(clock_t timeout) {

	old_time=clock();
	countdown_time=timeout;
	enable=1;

}


bool timer::check_timer() {

	if(enable) {
	
		clock_t diff;
		present_time=clock();
		diff=present_time-old_time;
	
		if(diff>=countdown_time) {

			return true;

		}

		else {

			return false;

		}
		
	}
	
	else {
	
		return false;
	
	}

}


void timer::disable_timer() {

	enable=false;

}

// The end.
