#include <Wire.h>
#define MAGNETIC_DECLINATION -6.0    // not used now -> magnetic bearing
int CompassAddress = 0x1E;


void Compass_Init()
{
  Wire.beginTransmission(CompassAddress);
  Wire.write(0x02); 
  Wire.write(0x00);   // Set continouos mode (default to 10Hz)
  Wire.endTransmission(); //end transmission
}


void Read_Compass()
{
  int i = 0;
  byte buff[6];

Wire.beginTransmission(CompassAddress); 
  Wire.write(0x03);        //sends address to read from
  Wire.endTransmission(); //end transmission

    //Wire.beginTransmission(CompassAddress); 
  Wire.requestFrom(CompassAddress, 6);    // request 6 bytes from device
  while(Wire.available())   // ((Wire.available())&&(i<6))
  { 
    buff[i] = Wire.read();  // receive one byte
    i++;
  }
  Wire.endTransmission(); //end transmission

    if (i==6)  // All bytes received?
  {
    // MSB byte first, then LSB, X,Y,Z
    magnetom_x = ((((int)buff[4]) << 8) | buff[5]);    // X axis (internal y axis)
    magnetom_y = ((((int)buff[0]) << 8) | buff[1]);    // Y axis (internal x axis)
    magnetom_z = ((((int)buff[2]) << 8) | buff[3]);    // Z axis
    
  }
  else
    Serial.println("!ERR: Mag data");
}

void Compass_Heading()
{
  float MAG_X;
  float MAG_Y;
  float cos_roll;
  float sin_roll;
  float cos_pitch;
  float sin_pitch;
  
  //cos_roll = cos(axR);
  //sin_roll = sin(axR);
  //cos_pitch = cos(ayR);
  //sin_pitch = sin(ayR);
  // Tilt compensated Magnetic filed X:
  MAG_X = magnetom_x*cos_pitch+magnetom_y*sin_roll*sin_pitch+magnetom_z*cos_roll*sin_pitch;
  // Tilt compensated Magnetic filed Y:
  MAG_Y = magnetom_y*cos_roll-magnetom_z*sin_roll;
  // Magnetic Heading
  MAG_Heading = atan2(-MAG_Y,MAG_X);
    Serial.print("MAG_Heading: ");
  Serial.print(MAG_Heading);
}

