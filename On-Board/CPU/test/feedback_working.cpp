#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string.h>
#include "serial.h"
#include "config.h"

#define START_BIT_INT 255

#define STEERING_MOTORS int i=0;i<4;i++
#define NUMBER_STEERING_MOTORS 4
#define DRIVE_MOTORS int i=0;i<6;i++
#define NUMBER_DRIVE_MOTORS 6

using namespace std;

int RESET=0;
int STOP=0;

int inp=0;

/*int ackermann_offsets[4][11]={	{-175, -140, -105, -70, -35, 0, 27, 45, 58, 69, 78},
					{175, 140, 105, 70, 35, 0, -27, -45, -58, -69, -78},
					{78, 69, 58, 45, 27, 0, -35, -70, -105, -140, -175},
					{-78, -69, -58, -45, -27, 0, 35, 70, 105, 140, 175}	};*/ // Commented until two-byte issue is resolved.

int ackermann_offsets[4][11]={	{-43, -35, -26, -17, -9, 0, 6, 11, 15, 17, 19},
				{43, 35, 26, 17, 9, 0, -6, -11, -15, -17, -19},
				{19, 17, 15, 11, 6, 0, -9, -17, -26, -35, -43},
				{-19, -17, -15, -11, -6, 0, 9, 17, 26, 35, 43}	};


int ackermann_pos[4][11]={	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Left first, then center, then right. Five levels left, one center, five levels right.
				{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // FL, RL, RR, FR
				{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
				{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}		};

float speed_ratios[6][11]={	{0.49, 0.52, 0.57, 0.65, 0.78, 1, 1, 1, 1, 1, 1},
				{0.49, 0.52, 0.57, 0.65, 0.78, 1, 1, 1, 1, 1, 1},
				{0.49, 0.52, 0.57, 0.65, 0.78, 1, 1, 1, 1, 1, 1},
				{1, 1, 1, 1, 1, 1, 0.78, 0.65, 0.57, 0.52, 0.49},
				{1, 1, 1, 1, 1, 1, 0.78, 0.65, 0.57, 0.52, 0.49},
				{1, 1, 1, 1, 1, 1, 0.78, 0.65, 0.57, 0.52, 0.49},	}; // These come from ackermann calculations

int initial_pos[4]; //FL, RL, RR, FR
int present_pos[4];
int set_pos[4];
float kp[]={6, 6, 6, 6};

string driving_motors[]={"FL", "ML", "RL", "RR", "MR", "FR"};
string steer_motors[]={"FL", "RL", "RR", "FR"};

int speed_level=0;
int angle_level=5;
int steer_dirs[]={3, 3, 3, 3};
int drive_dirs[]={3, 3, 3, 3, 3, 3};
int steer_pwms[]={0, 0, 0, 0};
int drive_pwms[]={0, 0, 0, 0, 0, 0};

int temp_pwm=0;

serial_device arduino_main, arduino_steer, xbee;

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
	cout<<"\nHello, I am "<<ROVER_NAME<<".\n\n----\n\n";
	#ifdef DEBUG
	cout<<"You're in DEBUG mode. If you do not want to see these messages, disable DEBUG in ./include/config.h and recompile.\n\n----\n\n";
	#endif
	wiringPiSetup();
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
		#ifdef DEBUG
		cout<<steer_motors[i]<<": "<<initial_pos[i]<<"\t";
		#endif
	}
	#ifdef DEBUG
	cout<<"\n\n----\n\n";
	cout<<"Ackermann analogRead values (divided by 4) after correction for initial position:"<<endl;
	#endif
	for(STEERING_MOTORS) {
		cout<<steer_motors[i]<<":\t";
		for(int j=0;j<11;j++) {
			ackermann_pos[i][j]=initial_pos[i]+ackermann_offsets[i][j];
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
			angle_level=(int)(4-(inp-51)/6);
		}
		else if(inp>75&&inp<=100) {
			angle_level=(int)(5+(inp-76)/6);
		}
		else if(inp==255) {
			for(DRIVE_MOTORS) {
				drive_dirs[i]=3;
			}
			for(STEERING_MOTORS) {
				steer_dirs[i]=3;
			}
			RESET=1;
			STOP=1;
		}

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
			drive_pwms[i]=speed_ratios[i][angle_level]*speed_level;
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
			temp_pwm=-kp[i]*(present_pos[i]-set_pos[i]);
			if(temp_pwm>=0) {
				steer_dirs[i]=1;
				steer_pwms[i]=(temp_pwm>=255)?254:temp_pwm;
			}
			else {
				steer_dirs[i]=2;
				temp_pwm=-temp_pwm;
				steer_pwms[i]=(temp_pwm>=255)?254:temp_pwm;
			}
			#ifdef DEBUG
			cout<<steer_motors[i]<<" set analogRead value: "<<set_pos[i]<<"\tset PWM value: "<<steer_pwms[i]<<"\t"<<"set direction: "<<steer_dirs[i]<<"\n";
			#endif
		}

		char temp=(char)START_BIT_INT; // this is 255
//		arduino_main.write_bytes(&temp, 1);
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

