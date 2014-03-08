#include<iostream>
#include<stdio.h>
#include<string>
#include<math.h>

class wheel{
private:

public:
	int radius; //in cm
	int torque;
//	string type;	//conatant for a wheel
	int DCmotor_current; //via arduino (current sensors)
	int rpmBase; //rpm received via xbee from the base station, all rpms are in range (0-255) to avoid scaling everytime
	int rpmEnc; //rpm from corresponding motor encoder, calculation done on arduino
	int rpm_output;
	int rhinoCurrent; //via arduino(current sensors)
	int angleFeedback;//angle which the servo shows
	int angleBase;//angle determined from ackerman calculations
	int angleOutput;
	int angleLevel;
	int rpm_max;//maximum rpm of the motor
	int voltage_max;
	int torqueCalc(); //calcuate torque fron the above current
	int rpmImu();//calc rpm from accelerometer and gyroscope values (velocity and stuff)
	int rpmOutputCalc(int base, int enc, int imu);//final rpm to be given to the wheels
	int angleDifferential();//to be given the the corresponding rhino
	int angleAckerman(int angle);
	int direction;
	wheel(int rad, int speed, int angle);
	int rpmCalc()
	{
		//use rpmBase, rpmImu, rpmEnc, rpmMax to determine rpmOutput
		rpm_output = rpmBase;
		return rpm_output;
	}
}; 

wheel::wheel(int rad, int speed, int angleL)
{
	radius = rad;
	rpm_output = speed;
	angleLevel = angleL;
	}
int wheel::torqueCalc()
{
	torque = DCmotor_current*rpm_output*voltage_max/rpm_max;
	return torque;
	}
/*	struct serial_device:
	parameters:
		baudrate //baudrate for the port
		name //"ttyACM" or equivalent
		number //ttyACM0 
		fileDescripter
	functions:
		read()
		write()
		open()
		configure()
		close()

class gyroscope{ //not yet complete
	parameters:
		type
		other library parameters	
	functions: 				
		init() //initialise the gyroscope
		velocity()
		orientation()
};
class accelerometer{//not yet complete
	parameters:
		from the library

	functions:
		acc_x()
		acc_y()			
		acc_z()
		velocity_x()
		velocity_y()			
		velocity_z()
		distance_x()
		distance_y()			
		distance_z()		
};		
class spectrometer{
	//no clue about what is going on here
};
6) struct battery:
	Parameters:
		state_of_charge
		current
	functions:
		safety_check() //ckeck if everything is working alright, return an alarm and shuts down everything in case of trouble
		
7) struct linearActuator:
	parameters:
		length_feedback //feedback from the linear actuators, via arduino
	functions:
		length() //stretch to this particular length
8) struct servo:
	similar to servo in arduino, for servos in the end effector						
			
9) struct camera: //camera mount, will use parameters from the servo struct
	parameters:
		tilt_base //values from the base
		pan_base
		tilt_feedback //feedback values from the corresponding servos
		pan_feedback
	functions:
		tilt() //tilt to be given to the camera mount, 
		pan()
10) struct sharp: //for the sharp sensors
	parameters:
		read //value from the sensor via arduino 
	functions:
		distance //convert the value above into distance, need to decide whether this has to be done on the arduino or pi
						
for encoders on joints, only values have to be read and no dedicated struct 
is required*/
