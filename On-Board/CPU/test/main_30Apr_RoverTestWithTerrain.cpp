#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <signal.h>
#include "serial.h"
#include "config.h"

#define START_BIT_INT 255

#define STEERING_MOTORS int i=0;i<4;i++
#define NUMBER_STEERING_MOTORS 4
#define DRIVE_MOTORS int i=0;i<6;i++
#define NUMBER_DRIVE_MOTORS 6

#define STEERING_SCALING_FACTOR 0.75

using namespace std;

int RESET=0;
int STOP=0;

int inp=0;


/*float ackermann_offsets[4][11]={	{-47.8307770926, -38.4723077304, -29.07194451, -19.5625591992, -9.8861198517, 0, 7.4913601749, 12.0592815028, 15.2602076661, 17.7442090079, 19.834070938},
					{50, 40, 30, 20, 10, 0, -7.4269870581, -11.8947951963, -15, -17.3961595234, -19.4035045029},
					{19.4035045029, 17.3961595234, 15, 11.8947951963, 7.4269870581, 0, -10, -20, -30, -40, -50},
					{-19.834070938, -17.7442090079, -15.2602076661, -12.0592815028, -7.4913601749, 0, 47.8307770926, 38.4723077304, 29.07194451, 19.5625591992, 9.8861198517}	};*/

float ackermann_offsets[4][11]={	{-19.834070938, -17.7442090079, -15.2602076661, -12.0592815028, -7.4913601749, 0, 47.8307770926, 38.4723077304, 29.07194451, 19.5625591992, 9.8861198517},
					{19.4035045029, 17.3961595234, 15, 11.8947951963, 7.4269870581, 0, -10, -20, -30, -40, -50},
					{50, 40, 30, 20, 10, 0, -7.4269870581, -11.8947951963, -15, -17.3961595234, -19.4035045029},
					{-47.8307770926, -38.4723077304, -29.07194451, -19.5625591992, -9.8861198517, 0, 7.4913601749, 12.0592815028, 15.2602076661, 17.7442090079, 19.834070938}	};


int ackermann_pos[4][11]={	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Left first, then center, then right. Five levels left, one center, five levels right.
				{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // FL, RL, RR, FR
				{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
				{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}		};


float speed_ratios[6][11]={	{0.4439560482, 0.473903761, 0.5233922688, 0.6036485594, 0.7399602722, 1, 0.9744388616, 0.9674502083, 0.9662508489, 0.9674004576, 0.9697830471},
				{0.210292425, 0.2924014276, 0.3896314881, 0.5149043045, 0.695094134, 1, 1, 1, 1, 1, 1},
				{0.4295375104, 0.4586783162, 0.5086404266, 0.5909687205, 0.7316178756, 1, 0.9828367972, 0.9904460541, 0.9861390886, 0.9826178486, 0.9806317656},
				{0.9904460541, 0.9861390886, 0.9826178486, 0.9806317656, 0.9828367972, 1, 0.4295375104, 0.4586783162, 0.5086404266, 0.5909687205, 0.7316178756},
				{1, 1, 1, 1, 1, 1, 0.210292425, 0.2924014276, 0.3896314881, 0.5149043045, 0.695094134},
				{0.9697830471, 0.9674004576, 0.9662508489, 0.9674502083, 0.9744388616, 1, 0.4439560482, 0.473903761, 0.5233922688, 0.6036485594, 0.7399602722},	}; // These come from ackermann calculations

int initial_pos[4]; //FL, RL, RR, FR
int present_pos[4];
int set_pos[4];
float kp[]={4, 4, 4, 4};

string driving_motors[]={"FL", "ML", "RL", "RR", "MR", "FR"};
string steer_motors[]={"FL", "RL", "RR", "FR"};

int speed_level=0;
int angle_level=5;
int steer_dirs[]={3, 3, 3, 3};
int drive_dirs[]={3, 3, 3, 3, 3, 3};
int steer_pwms[]={0, 0, 0, 0};
float drive_pwms[]={0, 0, 0, 0, 0, 0};

int temp_pwm=0;

serial_device arduino_main, arduino_steer, xbee;

void initialize();
void loop();
void reset();
void stop();
void interrupt(int);

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
	cout<<"\nHello, I am "<<ROVER_NAME<<".\n\n----\n\n";
	#ifdef DEBUG
	cout<<"You're in DEBUG mode. If you do not want to see these messages, disable DEBUG in ./include/config.h and recompile.\n\n----\n\n";
	#endif
	wiringPiSetup();
	signal(SIGINT, interrupt);
	signal(SIGTERM, interrupt);
	char temp=(char)255;
	arduino_main.name="Main Arduino";
	arduino_steer.name="Steering Arduino";
	xbee.name="XBee";
	arduino_main.open_port(ARDUINO_MAIN_PORT, ARDUINO_MAIN_BAUD);
	arduino_steer.open_port(ARDUINO_STEER_PORT, ARDUINO_STEER_BAUD);
	xbee.open_port(XBEE_PORT, XBEE_BAUD);
	delay(1000);
	#ifdef DEBUG
	cout<<"\n\n----\n\n";
	cout<<"Serial ports and file descriptors\n\n";
	cout<<"Main arduino: \t"<<ARDUINO_MAIN_PORT<<"\t"<<arduino_main.fd<<endl;
	cout<<"Steer arduino: \t"<<ARDUINO_STEER_PORT<<"\t"<<arduino_steer.fd<<endl;
	cout<<"XBee: \t\t"<<XBEE_PORT<<"\t"<<xbee.fd<<endl;
	cout<<"\n----\n\n";
	#else
	cout<<"You're not in DEBUG mode, set the DEBUG macro in ./include/config.h and recompile to go in DEBUG mode."<<endl;
	#endif
//	arduino_main.write_bytes(&temp, 1); --> Handshake to be written later
//	arduino_steer.write_bytes(&temp, 1);
	while(arduino_steer.available()<NUMBER_STEERING_MOTORS);
	#ifdef DEBUG
	cout<<"Initial analogRead values (divided by 4)"<<endl<<"Depending on the STEERING_MOTORS macro set, this may show less readings."<<endl;
	#endif
	for(STEERING_MOTORS) {
		initial_pos[i]=(int)arduino_steer.read();
		set_pos[i]=initial_pos[i];
		steer_pwms[i]=0;
		#ifdef DEBUG
		cout<<steer_motors[i]<<": "<<initial_pos[i]<<"\t\n\n----\n\n";
		#endif
	}
	#ifdef STEER_INIT_VALUES_CODED
	int temp_pos[]={189, 126, 80, 140};
	for(STEERING_MOTORS) {
		initial_pos[i]=temp_pos[i];
		set_pos[i]=temp_pos[i];
	}
	#ifdef DEBUG
	cout<<"Initial values for steer hard-coded.\n";
	for(STEERING_MOTORS) {
		cout<<steer_motors[i]<<": "<<initial_pos[i]<<"\t";
	}
	#endif
	#endif
	for(DRIVE_MOTORS) {
		drive_pwms[i]=0;
	}
	#ifdef DEBUG
	cout<<"\n\n----\n\n";
	cout<<"Ackermann analogRead values (divided by 4) after correction for initial position:"<<endl;
	#endif
	for(STEERING_MOTORS) {
		cout<<steer_motors[i]<<":\t";
		for(int j=0;j<11;j++) {
			ackermann_pos[i][j]=initial_pos[i]+STEERING_SCALING_FACTOR*ackermann_offsets[i][j];
			#ifdef DEBUG
			cout<<ackermann_pos[i][j]<<"\t";
			if(j==10) {
				cout<<endl;
			}
			#endif
		}
	}
	#ifdef DEBUG
	cout<<"\n\n----\n\nKp values for the steering P-controller, again in order: \n";
	for(STEERING_MOTORS) {
		cout<<steer_motors[i]<<":";
		cout<<kp[i]<<"\t";
	}
	#endif
	#ifdef DEBUG
	cout<<"\n\n----\n\n";
	#endif
}

void loop() {
	#ifdef DEBUG
	xbee.flush();
	while(!xbee.available());
	#endif
	if(arduino_main.available()) {
		cout<<(int)arduino_main.read()<<endl;
	}
	if(xbee.available()) {
		inp=(int)xbee.read();
		#ifdef DEBUG
		cout<<"Input received: "<<inp<<endl;
		cout<<"\n----\n\n";
		#endif

		if(inp>0&&inp<=25) {
			speed_level=(int)(255*inp/25);
			for(DRIVE_MOTORS) {
				drive_dirs[i]=1;
			}
		}
		else if(inp>25&&inp<=50) {
			speed_level=(int)(255*(inp-25)/25);
			for(DRIVE_MOTORS) {
				drive_dirs[i]=2;
			}
		}
		else if(inp>50&&inp<=75) {
			angle_level=(int)(5+(inp-51)/6);
		}
		else if(inp>75&&inp<=100) {
			angle_level=(int)(4-(inp-76)/6);
		}
/*		else if(inp==255) {
			for(DRIVE_MOTORS) {
				drive_dirs[i]=3;
			}
			for(STEERING_MOTORS) {
				steer_dirs[i]=3;
			}
			RESET=1;
			STOP=1;
		}*/

		#ifdef DEBUG
		cout<<"Calculated values for mid-calculation parameters:\n\n";
		cout<<"speed_level: "<<speed_level<<"\n";
		cout<<"angle_level: "<<angle_level<<"\n";
		cout<<"Directions for drive motors: \n";
		for(STEERING_MOTORS) {
			cout<<driving_motors[i]<<": "<<drive_dirs[i]<<"\t";
		}
		cout<<"\n\n----\n\n";
		cout<<"PWM calculated for driving motors:\n";
		#endif

		for(DRIVE_MOTORS) {
			drive_pwms[i]=speed_ratios[i][10-angle_level]*speed_level;
			if(drive_pwms[i]<=50) drive_pwms[i]=0;
			#ifdef DEBUG
			cout<<driving_motors[i]<<": "<<drive_pwms[i]<<"\t";
			#endif
		}
		#ifdef DEBUG
		cout<<"\n\n----\n\n";
		cout<<"Steering information: \n";
		#endif
		for(STEERING_MOTORS) {
			set_pos[i]=ackermann_pos[i][angle_level];
			temp_pwm=(int)(-kp[i]*(present_pos[i]-set_pos[i]));
			if(temp_pwm>=0) {
				steer_dirs[i]=1;
				steer_pwms[i]=(temp_pwm>=255)?254:temp_pwm;
			}
			else {
				steer_dirs[i]=2;
				temp_pwm=-temp_pwm;
				steer_pwms[i]=(temp_pwm>=255)?254:temp_pwm;
			}
			if(steer_pwms[i]<10) steer_pwms[i]=0;
			#ifdef DEBUG
			cout<<steer_motors[i]<<" set analogRead value: "<<set_pos[i]<<"\tset PWM value: "<<steer_pwms[i]<<"\t"<<"set direction: "<<steer_dirs[i]<<"\n";
			#endif
		}

		char temp=(char)START_BIT_INT; // this is 255
		arduino_main.write_bytes(&temp, 1);
		for(DRIVE_MOTORS) {
			temp=(char)drive_pwms[i];
			arduino_main.write_bytes(&temp, 1);
			temp=(char)drive_dirs[i];
			arduino_main.write_bytes(&temp, 1);
		}

		temp=(char)START_BIT_INT;
		arduino_steer.write_bytes(&temp, 1);
		for(STEERING_MOTORS) {
			temp=(char)steer_pwms[i];
			arduino_steer.write_bytes(&temp, 1);
			temp=(char)steer_dirs[i];
			arduino_steer.write_bytes(&temp, 1);
		}
		while(arduino_steer.available()<NUMBER_STEERING_MOTORS+1);
		if(arduino_steer.read()==255) {
			#ifdef DEBUG
			cout<<"Feedback analogRead values: \n";
			#endif
			for(STEERING_MOTORS) {
				present_pos[i]=(int)arduino_steer.read();
				#ifdef DEBUG
				cout<<steer_motors[i]<<": "<<present_pos[i]<<"\t";
				#endif
			}
			#ifdef DEBUG
			cout<<"\n\n----\n\n";
			#endif
		}
		else {
			arduino_steer.flush();
		}
	}
	#ifdef DEBUG
//	delay(1000);
	#endif
}

void reset() {
	for(DRIVE_MOTORS) {
		drive_dirs[i]=3;
	}
	for(STEERING_MOTORS) {
		steer_dirs[i]=3;
	}

	char temp=(char)START_BIT_INT;
	arduino_main.write_bytes(&temp, 1);
	for(DRIVE_MOTORS) {
		temp=(char)drive_pwms[i];
		arduino_main.write_bytes(&temp, 1);
		temp=(char)drive_dirs[i];
		arduino_main.write_bytes(&temp, 1);
	}

	temp=(char)START_BIT_INT;
	arduino_steer.write_bytes(&temp, 1);
	for(STEERING_MOTORS) {
		temp=(char)steer_pwms[i];
		arduino_steer.write_bytes(&temp, 1);
		temp=(char)steer_dirs[i];
		arduino_steer.write_bytes(&temp, 1);
	}


	arduino_main.close_port();
	arduino_steer.close_port();
	xbee.close_port();
}

void stop() {
	system("sudo shutdown now");
}


void interrupt(int arg) { // Function to be executed when a sigint or a sigterm is issued to the process
	cout<<"\n\n----\n\nShutting down";
	for(int i=0;i<5;i++) {
		cout<<".";
	}
	cout<<endl;
	reset();
	delay(1000);
	exit(0);
}
