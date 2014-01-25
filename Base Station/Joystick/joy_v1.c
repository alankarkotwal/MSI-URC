#include <stdlib.h>
//#include <GL/glew.h>7
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif
#include <cstdio>
#include <iostream>

#include "Gamepad.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "SFile.h"
#include <math.h>

static bool verbose = true;
#define POLL_ITERATION_INTERVAL 30

SFile serialPort;
BYTE b;
uint8_t send=0;
uint8_t state[6];
//serialPort = new SFile();

void onButtonDown(struct Gamepad_device * device, unsigned int buttonID, double timestamp, void * context) {
	if (verbose) {
		printf("Button %u down on device %u \n", buttonID, device->deviceID );
	}
	serialPort.WriteByte((BYTE)buttonID);
    state[buttonID]=1;
}


void onButtonUp(struct Gamepad_device * device, unsigned int buttonID, double timestamp, void * context) {
	if (verbose) {
		printf("Button %u up on device %u \n", buttonID, device->deviceID);
	}
	serialPort.WriteByte((BYTE)buttonID);
    state[buttonID]=0;
}

void onAxisMoved(struct Gamepad_device * device, unsigned int axisID, float value, float lastValue, double timestamp, void * context) {
	if (verbose) {
		printf("Axis %u moved from %f to %f on device %u\n", axisID, lastValue, value, device->deviceID);
	}

}

void onDeviceAttached(struct Gamepad_device * device, void * context) {
	if (verbose) {
		printf("Device ID %u attached (vendor = 0x%X; product = 0x%X) \n", device->deviceID, device->vendorID, device->productID);
	}
}

void onDeviceRemoved(struct Gamepad_device * device, void * context) {
	if (verbose) {
		printf("Device ID %u removed \n", device->deviceID);
	}
}

static void initGamepad() {
	Gamepad_deviceAttachFunc(onDeviceAttached, (void *) 0x1);
	Gamepad_deviceRemoveFunc(onDeviceRemoved, (void *) 0x2);
	Gamepad_buttonDownFunc(onButtonDown, (void *) 0x3);
	Gamepad_buttonUpFunc(onButtonUp, (void *) 0x4);
	Gamepad_axisMoveFunc(onAxisMoved, (void *) 0x5);
	Gamepad_init();
}




#define POLL_ITERATION_INTERVAL 30

bool joy_print() {
	static unsigned int iterationsToNextPoll = POLL_ITERATION_INTERVAL;

	iterationsToNextPoll--;
	if (iterationsToNextPoll == 0) {
		Gamepad_detectDevices();
		iterationsToNextPoll = POLL_ITERATION_INTERVAL;
	}
	Gamepad_processEvents();


	if (Gamepad_numDevices() < 1) {

		printf("No devices found; plug in a USB gamepad and it will be detected automatically\n");
	}
	send = 0;
	for(int i=0; i<6; i++)
    {
        send = send + state[i]*pow(2,i);
    }
    serialPort.WriteByte((BYTE)send);
    std::cout<<(int)send<<std::endl;

	return true;
}











static int make_resources(void)
{
    return 1;
}
static void update_fade_factor(void)
{
    joy_print();


}
static void render(void)
{

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    joy_print();
    glutSwapBuffers();
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(400, 300);
    glutCreateWindow("Hello World");

    initGamepad();

    serialPort.Open(1,9600);

    if(serialPort.IsOpen())
    {
        std::cout<<"serial port open";
    }
    else
    {
        std::cout<<"serial port not open";
        std::cout<<serialPort.CommNumber();
    }


    glutDisplayFunc(&render);
    glutIdleFunc(&update_fade_factor);
/*    glewInit();
    if (!GLEW_VERSION_2_0) {
        fprintf(stderr, "OpenGL 2.0 not available\n");
        return 1;
    }
    else std::cout<<"available";
     if (!make_resources()) {
        fprintf(stderr, "Failed to load resources\n");
        return 1;
    }*/

    glutMainLoop();
    return 0;
}


