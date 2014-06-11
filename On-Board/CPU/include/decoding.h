#ifndef _DECODING_H
#define _DECODING_H

#include<vector>

#define MAXQUEUESIZE 5    // max data length necessary for the longest packet sent or received on ROVER
#define DELIMETER 251       // delemeter for xbee data

#define ID_ROVER_DRIVE	 	(unsigned char)181
#define ID_STOPALL	 	(unsigned char)250
#define ID_ROBOTIC_ARM		(unsigned char)191
#define ID_CAMERAS		(unsigned char)192
#define ID_BIO			(unsigned char)200

#define SIZE_ROVER_DRIVE	4
#define SIZE_STOPALL		2
#define SIZE_ROBOTIC_ARM 	5
#define SIZE_CAMERAS		4
#define SIZE_BIO		4

class Decoding
{
public:
	Decoding();
	~Decoding();

	void putNewDatas(unsigned char* data);
	void putNewData(unsigned char byte);

	void parseIt();
	unsigned char getCurrentActionID();
	int getdataqueueSize(){return dataqueue.size();}
	int getactionIDlistSize(){return actionIDlist.size();}
	void actionDone();

	void eraseAllData();
	void eraseAllAction();

	/* data */
    unsigned char   ROVER_THROTTLE,
                    ROVER_TURN,

                    ROBOTIC_ARM_X,
                    ROBOTIC_ARM_Y,
                    ROBOTIC_ARM_D,
                    ROBOTIC_ARM_B,

                    CAMERAS_MAINCAMERA_PITCH,
                    CAMERAS_MAINCAMERA_YAW;

    std::vector<unsigned char> actionIDlist;
    std::vector<unsigned char> dataqueue;

};

#endif
