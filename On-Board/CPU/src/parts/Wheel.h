/* @TODO: Put in here a brief description for the class and its functions. Documentation time. */

#ifndef _WHEEL_H
#define _WHEEL_H

#define WHEEL_RADIUS_FRONT 7
#define WHEEL_RADIUS_MID 7
#define WHEEL_RADIUS_BACK 7

#define UNDEF_WHEEL_TYPE 0
#define LEFT_FRONT 1
#define LEFT_MID 2
#define LEFT_BACK 3
#define RIGHT_FRONT 4
#define RIGHT_MIN 5
#define RIGHT_BACK 6

struct Wheel
{
	int Radius;
	int Speed;
	int Type;
	int Current;
	int Angle;
	int Torque;
	int EncoderDistance;

	Wheel();
	Wheel(int WheelType);
};

#endif
