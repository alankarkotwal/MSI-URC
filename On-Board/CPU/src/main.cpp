

#include <iostream>
#include "serial.h"

using namespace std;

int main() {

serial_device arduino;

arduino.open_port("/dev/ttyUSB6");
arduino.configure_port(B9600);

char a;

while(1) {
   arduino.write_bytes("a", 1);
   arduino.read_bytes(&a, 1);
   cout<<a<<endl;
   usleep(1000000);
}

}
