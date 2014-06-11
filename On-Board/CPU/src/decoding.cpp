#include "decoding.h"

using namespace std;

Decoding::Decoding() {
    ROVER_THROTTLE =0;
    ROVER_TURN =0;

    ROBOTIC_ARM_X = 0;
    ROBOTIC_ARM_Y = 0;
    ROBOTIC_ARM_D = 0;
    ROBOTIC_ARM_B = 0;

    CAMERAS_MAINCAMERA_PITCH = 0;
    CAMERAS_MAINCAMERA_YAW = 0;
}

Decoding::~Decoding() {
}

void Decoding::putNewDatas(unsigned char data[]) // Does not work as of now.
{
    int len = sizeof(data)/sizeof(unsigned char);
    for(int i =0; i < len ; i++){
        dataqueue.push_back(data[i]);
    }
}

void Decoding::putNewData(unsigned char byte)
{
	dataqueue.push_back(byte);
	if(dataqueue.size() > MAXQUEUESIZE*3){
	       dataqueue.erase(dataqueue.begin(),dataqueue.end() - MAXQUEUESIZE*3);
	 }
	            
}

void Decoding::parseIt()
{
    for(int i=0; i < dataqueue.size(); i++){
        if(dataqueue.at(i) == DELIMETER && dataqueue.size() > i+1 ){   // one packet found


                if( ID_ROVER_DRIVE == dataqueue.at(i+1) ){
//                cout<<"Throttle ID Found"<<endl;
                    if(dataqueue.size() > i + SIZE_ROVER_DRIVE){ // enough data exist to parse
                        ROVER_THROTTLE = dataqueue.at(i+2);
                        ROVER_TURN = dataqueue.at(i+3);
                        if(actionIDlist.size()){
                        	if( actionIDlist.at(actionIDlist.size()-1) != ID_ROVER_DRIVE){
                                	actionIDlist.push_back(ID_ROVER_DRIVE);
                                }
                        }
                        else{
                        	actionIDlist.push_back(ID_ROVER_DRIVE);
                        }
                    }
                    i=i+ SIZE_ROVER_DRIVE;

                }
                else if( ID_ROBOTIC_ARM==dataqueue.at(i+1)){
//                cout<<"robotic Arm ID found"<<endl;
                    if(dataqueue.size() > i + SIZE_ROBOTIC_ARM){ // enough data exist to parse
                        ROBOTIC_ARM_X = dataqueue.at(i+2);
                        ROBOTIC_ARM_Y = dataqueue.at(i+3);
                        ROBOTIC_ARM_D = dataqueue.at(i+4);
                        ROBOTIC_ARM_B = dataqueue.at(i+5);
                        if(actionIDlist.size()){
                        	if( actionIDlist.at(actionIDlist.size()-1) != ID_ROBOTIC_ARM){
                                	actionIDlist.push_back(ID_ROBOTIC_ARM);
                                }
                        }
                        else{
                        	actionIDlist.push_back(ID_ROBOTIC_ARM);
                        }

                    }
                    i=i+ SIZE_ROBOTIC_ARM;

            	}
            	else if( ID_CAMERAS == dataqueue.at(i+1) ){
//            	cout<<"Cameras ID Found"<<endl;
            		if(dataqueue.size() > i+SIZE_CAMERAS){// enough data exist to parse
            			CAMERAS_MAINCAMERA_PITCH=dataqueue.at(i+2);
            			CAMERAS_MAINCAMERA_YAW = dataqueue.at(i+3);
            			if(actionIDlist.size()){
            				if(actionIDlist.at(actionIDlist.size()-1) != ID_CAMERAS){
            					actionIDlist.push_back(ID_CAMERAS);
            				}
            			}
            			else{
            				actionIDlist.push_back(ID_CAMERAS);
            			}
            		}
            		i=i+SIZE_CAMERAS;
            	}


        }
    }

    if(dataqueue.size() > MAXQUEUESIZE){
        dataqueue.erase(dataqueue.begin(),dataqueue.end() - MAXQUEUESIZE);
    }
}

unsigned char Decoding::getCurrentActionID()
{
    if(actionIDlist.size()){
        return actionIDlist.at(0);
    }
    return (unsigned char)0;   //  no new action to be done just keep doing what ever you were doing
}
void Decoding::actionDone()
{
    if(actionIDlist.size()){
        actionIDlist.erase(actionIDlist.begin(),actionIDlist.begin()+1);    // remove first value
    }
}

void Decoding::eraseAllData()
{
    dataqueue.clear();
}
void Decoding::eraseAllAction()
{
    actionIDlist.clear();
}
