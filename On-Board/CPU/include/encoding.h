#ifndef _ENCODING_H
#define _ENCODING_H

#define E_SIZE_GPS 10
#define E_SIZE_IMU 4
#define E_SIZE_BIO 2

#define E_ID_GPS 'G'
#define E_ID_IMU 'I'
#define E_ID_BIO 'B'

class Encoding
{
    public:
        Encoding();
        ~Encoding();

        void encode(unsigned char ID, unsigned char* arr);

        unsigned char   GPS_LAT_BD,
                        GPS_LAT_AD_1,
                        GPS_LAT_AD_2,
                        GPS_LAT_AD_3,

                        GPS_LON_BD,
                        GPS_LON_AD_1,
                        GPS_LON_AD_2,
                        GPS_LON_AD_3,

                        GPS_REGION,

                        IMU_ROLL,
                        IMU_PITCH,
                        IMU_HEADING_BH,

                     /*   MAG_X,
                        MAG_Y,
                        MAG_Z,*/
                        
                        BIO
                        ;
//*******************************************************
		//const unsigned char ID_GPS = 'G';
		//const unsigned char ID_IMU = 'I';
		//const unsigned char ID_BIO = 'B';

		//const int SIZE_GPS = 10;    // lat(B.BBB), lon(B.BBB) , region(B)
		//const int SIZE_IMU = 4;     //roll(B), pitch(B), heading(B)
		//const int SIZE_BIO = 2;     // not decided

};

#endif
