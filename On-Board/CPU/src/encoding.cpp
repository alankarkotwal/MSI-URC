#include "encoding.h"


Encoding::Encoding()
{
    //ctor
    GPS_LAT_BD = 0;
    GPS_LAT_AD_1 = 0;
    GPS_LAT_AD_2 = 0;
    GPS_LAT_AD_3 = 0;

    GPS_LON_BD = 0;
    GPS_LON_AD_1 = 0;
    GPS_LON_AD_2 = 0;
    GPS_LON_AD_3 = 0;

    GPS_REGION = 1;   // Region:  ++= 1,   -+ =2,  -- = 3, +- = 4

    IMU_ROLL = 0;
    IMU_PITCH = 0;
    IMU_HEADING_BH = 0; // IMU_HEADING_BH = heading/2
//    MAG_X = 0;
//   MAG_Y = 0;
//    MAG_Z = 0;
}

Encoding::~Encoding()
{
}

void Encoding::encode(unsigned char ID, unsigned char* arr)
{

    switch(ID)
    {
    case E_ID_GPS: {
      	 unsigned char datas1[]={
         E_ID_GPS,
         GPS_LAT_BD,
         GPS_LAT_AD_1,
         GPS_LAT_AD_2,
         GPS_LAT_AD_3,
         GPS_LON_BD,
         GPS_LON_AD_1,
         GPS_LON_AD_2,
         GPS_LON_AD_3,
         GPS_REGION,
//         MAG_X,
//         MAG_Y,
//         MAG_Z
        };
        for(int i=0;i<E_SIZE_GPS;i++) {
        	arr[i]=datas1[i];
        }
        break;
   }

    case E_ID_IMU: {
        unsigned char datas2[E_SIZE_IMU];
        datas2[0] = E_ID_IMU;
        datas2[1] = IMU_ROLL;
        datas2[2] = IMU_PITCH;
        datas2[3] = IMU_HEADING_BH;
        for(int i=0;i<E_SIZE_IMU;i++) {
        	arr[i]=datas2[i];
        }
	break;
   }

   default: arr[0]=0; break; 
    }
}
