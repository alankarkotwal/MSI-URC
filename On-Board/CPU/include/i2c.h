/*********************************************************************************************\

I2C Code Header for driving an I2C IMU device on a UNIX machine.
Megha Gupta, Alankar Kotwal

Log follows:

1) 26 Feb 2014: Alankar created this file with the class and functions. Compiled as a library.

\*********************************************************************************************/

#ifndef _I2C_H
#define _I2C_H

#include <unistd.h>
#include <math.h>
#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <cstring>
#include <string>
#include <time.h>
#include "L3G.h"
#include "LSM303.h"
#include "i2c-dev.h"
#include "timing.h"

using namespace std;

class i2c_device {

	timer i2c_timer;

	int i2c_file;
	float startInt;
	float rate_gyr_x, rate_gyr_y, rate_gyr_z;
	float intgX, intgY, intgZ;
	float angleX, angleY, angleZ;
	float accel_x, accel_y, accel_z;
	float Ax, Ay, Az;
	float veloX, veloY, veloZ;

	int sensor_sign[4];
	float gyr_raw;
	float G_GAIN
	float G_offset[4];

  public:

  	void enable();
  	void readBlock(uint8_t command, uint8_t size, uint8_t *data);
  	void selectDevice(int i2c_file, int addr);
  	void readACC(int *a);
  	void readMAG(int *m);
  	void readGYR(int *g);
  	void writeAccReg(uint8_t reg, uint8_t value);
  	void writeMagReg(uint8_t reg, uint8_t value);
  	void writeGyrReg(uint8_t reg, uint8_t value);
  	void gyr_angles();
  	void gyr_rates();
  	void accel_calc();
  	void velo_calc();
  	float Dt();

};

#endif
