#include <iostream>
#include "serial.h"
#include "config.h"
#include "decoding.h"
#include "encoding.h"

using namespace std;

serial_device xbee;

int num;
int currentActionID;

int STOP=0;
int RESET=0;

Decoding decoding;
Encoding encoding;

void initialize();
void loop();
void reset();
void stop();

void sendTelemetryData();
void readFromSensor();

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
	wiringPiSetup();
	xbee.open_port("/dev/ttyUSB0", 19200);
}

void loop() {

	num=xbee.available();
	if(num) {

		for(int i=0;i<num;i++) {
			decoding.putNewData(xbee.read());
		}
	}

	decoding.parseIt();

	int actionIDSize = decoding.getactionIDlistSize();
	for(int i=0; i<actionIDSize; i++){
		currentActionID=decoding.getCurrentActionID();

		if(currentActionID==ID_ROVER_DRIVE) {
			cout<<"Throttle level "<<(int)decoding.ROVER_THROTTLE<<"\t";
			cout<<"Turn level "<<(int)decoding.ROVER_TURN<<endl;
			decoding.actionDone();
		}
		else if(currentActionID==ID_STOPALL) {
			cout<<"Stop all"<<endl;
			decoding.actionDone();
		}
		else if(currentActionID==ID_ROBOTIC_ARM) {
			cout<<"Robotic Arm X "<<(int)decoding.ROBOTIC_ARM_X<<endl;
			cout<<"Robotic Arm Y "<<(int)decoding.ROBOTIC_ARM_Y<<endl;
			cout<<"Robotic Arm D "<<(int)decoding.ROBOTIC_ARM_D<<endl;
			decoding.actionDone();
		}
		else if(currentActionID==ID_CAMERAS) {
			cout<<"Camera Pitch "<<(int)decoding.CAMERAS_MAINCAMERA_PITCH<<endl;
			cout<<"Camera Yaw "<<(int)decoding.CAMERAS_MAINCAMERA_YAW<<endl;
			decoding.actionDone();
		}
		else if(currentActionID==ID_BIO) {
			cout<<"Bio"<<endl;
			decoding.actionDone();
		}


		else{// DEFAULT
			decoding.actionDone();// no match for ID simply pop it out
		}
	}//for ends


	//Encoding
	readFromSensor();
	sendTelemetryData();
}
// lat = 97.233256
// lon = 21.345657
// Region = 1
// IMU roll = 35 deg.
// IMU pitch = 78 deg.
// IMU heading = 300 deg from north

void readFromSensor(){
// read and update values
	if(millis()<1000) encoding.GPS_LAT_BD=0;
	else if(millis()<2000) encoding.GPS_LAT_BD=10;
	else if(millis()<3000) encoding.GPS_LAT_BD=20;
	else if(millis()<4000) encoding.GPS_LAT_BD=30;
	else if(millis()<5000) encoding.GPS_LAT_BD=40;
	else encoding.GPS_LAT_BD=50;
	encoding.GPS_LAT_AD_1=76;
	encoding.GPS_LAT_AD_2=54;
	encoding.GPS_LAT_AD_3=32;

	encoding.GPS_LON_BD=12;
	encoding.GPS_LON_AD_1=34;
	encoding.GPS_LON_AD_2=56;
	encoding.GPS_LON_AD_3=78;

	encoding.GPS_REGION=1;

	encoding.IMU_ROLL=60;
	encoding.IMU_PITCH=60;
	encoding.IMU_HEADING_BH=120;// this is actual heading / 2
}

void sendTelemetryData(){

	unsigned char GPSdatas[E_SIZE_GPS];
	encoding.encode(E_ID_GPS, GPSdatas);
	xbee.write_byte((char)DELIMETER);
	xbee.write_bytes(GPSdatas, E_SIZE_GPS);

	unsigned char IMUdatas[E_SIZE_IMU];
	encoding.encode(E_ID_IMU, IMUdatas);
	xbee.write_byte((char)DELIMETER);
	xbee.write_bytes(IMUdatas,E_SIZE_IMU);
}

void reset() {
	xbee.close_port();
}

void stop() {
}

