#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "serial.h"
#include "config.h"
#include "arm_rpi.h"

int RESET=0;
int STOP=0;

int FL_angles[]={};
int FR_angles[]={};
int RL_angles[]={};
int RR_angles[]={};

float speed_ratios[]={};

int default_steer[4]; //FL, RL, RR, FR
int set_angle[4];
int set_pwm[4];
int present_state[4];

int speed_level;
int angle_level;

using namespace std;

serial_device arduino, xbee;

void initialize();
void loop();
void reset();
void stop();

int main() {
	initialize();
	while(!STOP) {
		while(!RESET) {
			loop();
		}
		reset();
		initialize();
	}
	stop();
	return 0;
}

void initialize() {
	arduino.open_port(ARDUINO_PORT, ARDUINO_BAUD);
	xbee.open_port(XBEE_PORT, XBEE_BAUD);
	while(arduino.available<4);
//	for(int i=0;i<4;i++) {
//		default_steer[i]=(int)arduino.read();
//	}
}

void loop() {
	// Get and decode xbee data


}

void reset() {
	// Set everything to default values, SEND TO ARDUINO!
	arduino.close_port();
	xbee.close_port();
}

void stop() {
	system("sudo shutdown now");
}
