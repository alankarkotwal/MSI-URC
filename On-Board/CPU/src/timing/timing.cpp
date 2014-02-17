/*********************************************************************************************\

Timing functionality on a UNIX machine. Function definitions.
Megha Gupta, Alankar Kotwal

Log: Refer to the header.

\*********************************************************************************************/


#include"timing.h"


void timing::start() {

	old_time=clock();

}


clock_t timing::get_time() {

	present_time=clock();
	return present_time;

}


void timing::set_timer(clock_t timeout) {

	old_time=clock();
	countdown_time=timeout;
	enable=1;

}


bool timing::check_timer() {

	if(enable) {
	
		clock_t diff;
		present_time=clock();
		diff=present_time-old_time;
	
		if(diff>=countdown_time) {

			return TRUE;

		}

		else {

			return FALSE;

		}
		
	}
	
	else {
	
		return FALSE;
	
	}

}


void timing::disable_timer() {

	enable=FALSE;

}

// The end.
