/*********************************************************************************************\

I2C Code for driving an I2C IMU device on a UNIX machine. Function definitions.
Megha Gupta, Alankar Kotwal

Log: Refer to the header.

\*********************************************************************************************/


#include "i2c.h"
#include "i2c-dev.h"
#include "L3G.h"
#include "LSM303.h"


void i2c_device::readBlock(uint8_t command, uint8_t size, uint8_t *data) {

    int result = i2c_smbus_read_i2c_block_data(i2c_file, command, size, data);
    if (result != size) {

        printf("Failed to read block from I2C.");
        exit(1);
    }
}


void i2c_device::selectDevice(int i2c_file, int addr) {
    
    char device[3];
    if (addr == 1) {
        device[1] = 'L';
        device[2] = '3';
        device[3] = 'G';
    }

    else {
        device[1] = 'L';
        device[2] = 'S';
        device[3] = 'M';
    }


    if (ioctl(i2c_file, I2C_SLAVE, addr) < 0) {
/*      fprintf(stderr,
        "Error: Could not select device  0x%02x: %s\n",
        device, (int)strerror(errno)); */
        printf("Error: Could not select I2C device.\n");
    }
}


void i2c_device::readAcc(int *a) {
  
  uint8_t block[6];
  selectDevice(i2c_file,ACC_ADDRESS);
  readBlock(0x80 | LSM303_OUT_X_L_A, sizeof(block), block);

  *a = (int16_t)(block[0] | block[1] << 8) >> 4;
  *(a+1) = (int16_t)(block[2] | block[3] << 8) >> 4;
  *(a+2) = (int16_t)(block[4] | block[5] << 8) >> 4;
}


void i2c_device::readMAG(int  *m) {

  uint8_t block[6];
  selectDevice(i2c_file,MAG_ADDRESS);
  // DLHC: register address order is X,Z,Y with high bytes first
  readBlock(0x80 | LSM303_OUT_X_H_M, sizeof(block), block);

  *m = (int16_t)(block[1] | block[0] << 8);
  *(m+1) = (int16_t)(block[5] | block[4] << 8) ;
  *(m+2) = (int16_t)(block[3] | block[2] << 8) ;
}


void i2c_device::readGYR(int *g) {

  uint8_t block[6];

  selectDevice(i2c_file,GYR_ADDRESS);

  readBlock(0x80 | L3G_OUT_X_L, sizeof(block), block);

    *g = (int16_t)(block[1] << 8 | block[0]);
    *(g+1) = (int16_t)(block[3] << 8 | block[2]);
    *(g+2) = (int16_t)(block[5] << 8 | block[4]);

}


void i2c_device::writeAccReg(uint8_t reg, uint8_t value)
{
    selectDevice(i2c_file,ACC_ADDRESS);
    int result = i2c_smbus_write_byte_data(i2c_file, reg, value);
    if (result == -1)
    {
        printf ("Failed to write byte to I2C Acc.");
        exit(1);
    }
}


void i2c_device::writeMagReg(uint8_t reg, uint8_t value)
{
    selectDevice(i2c_file,MAG_ADDRESS);
    int result = i2c_smbus_write_byte_data(i2c_file, reg, value);
    if (result == -1)
    {
        printf("Failed to write byte to I2C Mag.");
        exit(1);
    }
}


void i2c_device::writeGyrReg(uint8_t reg, uint8_t value)
{
    selectDevice(i2c_file,GYR_ADDRESS);
  int result = i2c_smbus_write_byte_data(i2c_file, reg, value);
    if (result == -1)
    {
        printf("Failed to write byte to I2C Gyr.");
        exit(1);
    }
}


void i2c_device::enable()
{

  __u16 block[I2C_SMBUS_BLOCK_MAX];

  int res, bus,  size;

  char i2c_filename[20];
  sprintf(i2c_filename, "/dev/i2c-%d", 1);
  i2c_file = open(i2c_filename, O_RDWR);
  if (i2c_file<0) {
    printf("Unable to open I2C bus!");
    exit(1);
  }

 // Enable accelerometer.
  writeAccReg(LSM303_CTRL_REG1_A, 0b01010111); //  z,y,x axis enabled , 100Hz data rate
  writeAccReg(LSM303_CTRL_REG4_A, 0b00101000); // +/- 8G full scale: FS = 10 on DLHC, high resolution output mode

 // Enable magnetometer
    writeMagReg(LSM303_MR_REG_M, 0x00);  // enable magnometer

 // Enable Gyro
    writeGyrReg(L3G_CTRL_REG1, 0b00001111); // Normal power mode, all axes enabled
    writeGyrReg(L3G_CTRL_REG4, 0b00110000); // Continuos update, 2000 dps full scale

    i2c_timer.start();
    timeold=i2c_timer.get_time();
}


 // Calculation of Dt
float i2c_device::Dt() {
  int timeNow=i2c_timer.get_time();
  float deltaT=(timeNow-timeold)/1000;
  timeold=timeNow;
  return deltaT;
}     
 //Convert Gyro raw to degrees per second


 void i2c_device::gyr_rates() {     
  rate_gyr_x =( (sensor_sign[1]) *gyr_raw * G_GAIN)-G_offset[1];
  rate_gyr_y = ((sensor_sign[2]) *(gyr_raw+1) * G_GAIN)-G_offset[2];
  rate_gyr_z = ((sensor_sign[3]) *(gyr_raw+2) * G_GAIN)-G_offset[3];
}


void i2c_device::read_accel() {
  readAcc();
  AN[3] = compass.a.x;
  AN[4] = compass.a.y;
  AN[5] = compass.a.z;
  accel_x = (sensor_sign[3] * AN[3])/25;// - AN_OFFSET[3]);
  accel_y = (sensor_sign[4] * AN[4])/25;// - AN_OFFSET[4]);
  accel_z = (sensor_sign[5] * AN[5])/26;// - AN_OFFSET[5]);
}


// gyroangles calculation
void i2c_device::gyr_angles() {
  int startInt = mymillis();
    
  //Each loop should be at least 250ms.

  while(mymillis() - startInt < 250) {
    intgX=intgX+(rate_gyr_x*Dt());
    intgY=intgY+(rate_gyr_y*Dt());
    intgZ=intgZ+(rate_gyr_z*Dt());
  }

  angleX=angleX+intgX;
  angleY=angleY+intgY;
  angleZ=angleZ+intgZ;

}


//accceleration
void i2c_device::accel_calc() {
  if (accel_x<1 && accel_x>-1) {
    Ax=0;
  }
  else {
    Ax=(accel_x-9.8)*0.67;
  }
  if (accel_y<1 && accel_y>-1) {
    Ay=0;
  }
  else {
    Ay=(accel_y-9.8)*0.67;
  }
  if (accel_z<1 && accel_z>-1) {
    Az=0;
  }
  else{
    Az=(accel_z-9.8)*0.67;
  }
}


// velocities
void i2c_device::velo_calc() {
  veloX= veloX+(Ax*Dt());
  veloY= veloY+(Ay*Dt());
  veloZ= veloZ+(Az*Dt());
}


int i2c_device::mymillis() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (tv.tv_sec) * 1000 + (tv.tv_usec)/1000;
}

// The end.