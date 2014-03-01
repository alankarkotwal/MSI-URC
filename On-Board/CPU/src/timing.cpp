/*********************************************************************************************\

Timing functionality on a UNIX machine. Function definitions.
Megha Gupta, Alankar Kotwal

Log: Refer to the header.

\*********************************************************************************************/


#include "timing.h"


timeout::timeout(){

}


void timeout::init_timer() {

    gettimeofday(&start_time, NULL);
    gettimeofday(&prev_time, NULL);

}


unsigned long int timeout::time_ms() {

    struct timeval current_time;

    int sec,usec;

    gettimeofday(&current_time, NULL);
    sec=current_time.tv_sec-prev_time.tv_sec;
    usec=current_time.tv_usec-prev_time.tv_usec;
    if (usec<0) {
        usec=1000000-prev_time.tv_usec+current_time.tv_usec;
        sec--;
    }

    prev_time=current_time;
    return sec*1000+usec/1000;
}


// The end.
