/*********************************************************************************************\

Serial API for driving a generic serial device on a UNIX machine. Function definitions.
Megha Gupta, Alankar Kotwal

Log: Refer to the header.

\*********************************************************************************************/


#include "serial.h"


serial_device::serial_device() {

}


serial_device::~serial_device() {

    close_port();

}


char serial_device::open_port(const char *device,const unsigned int baud_rate) {
 
    struct termios options;

    fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1) {
        return -2;
    }

    fcntl(fd, F_SETFL, FNDELAY);

    tcgetattr(fd, &options);
    bzero(&options, sizeof(options));

    cfsetispeed(&options, baud_rate);
    cfsetospeed(&options, baud_rate);

    options.c_cflag |= ( CLOCAL | CREAD |  CS8);
    options.c_iflag |= ( IGNPAR | IGNBRK );
    options.c_cc[VTIME]=0;
    options.c_cc[VMIN]=0;
    tcsetattr(fd, TCSANOW, &options);
    return (1);

}


void serial_device::close_port() {

    close (fd);

}


char serial_device::write_byte(char data) {

    if (write(fd,&data,1)!=1) {
        return -1;
    }

    return 1;

}


char serial_device::write_bytes(const void *data, const unsigned int number_bytes) {

    if (write (fd,data,number_bytes)!=(ssize_t)number_bytes) {
        return -1;
    }

    return 1;

}


char serial_device::read_byte(char *data, unsigned int timeout_ms) {

    timeout timer;
    timer.init_timer();

    while (timer.time_ms()<timeout_ms || timeout_ms==0) {
        switch (read(fd,data,1)) {
            case 1  : return 1;
            case -1 : return -2;
        }
    }

    return 0;
}


int serial_device::read_bytes (void *data,unsigned int max_bytes,unsigned int timeout_ms) {

    timeout timer;
    timer.init_timer();

    unsigned int number_read_bytes=0;
    while (timer.time_ms()<timeout_ms || timeout_ms==0){

        unsigned char* current_pointer=(unsigned char*)data+number_read_bytes;
        int temp=read(fd,(void*)current_pointer,max_bytes-number_read_bytes);

        if (temp==-1) {
            return -2;
        }

        if (temp>0) {
            number_read_bytes+=temp;
            if (number_read_bytes>=max_bytes) {
                return 1;
            }
        }
    }

    return 0;

}


void serial_device::flush_receiver() {

    tcflush(fd,TCIFLUSH);

}


int serial_device::available(){

    int number_bytes=0;
    ioctl(fd, FIONREAD, &number_bytes);
    return number_bytes;

}