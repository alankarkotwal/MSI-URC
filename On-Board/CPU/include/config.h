// Config for RPi
#define ROVER_NAME "The Final Rover"

// Rover running modes
#define DEBUG
//#define DEBUG_CUSTOM
#define STEER_INIT_VALUES_CODED
#define STEERING_SCALING_FACTOR 0.75

// Settings
#define ARDUINO_MAIN_PORT "/dev/ttyAMA0"
#define ARDUINO_MAIN_BAUD 9600

#define ARDUINO_STEER_PORT "/dev/ttyACM0"
#define ARDUINO_STEER_BAUD 9600

//#define ARDUINO_ARM_PORT "/dev/ttyACM1"
//#define ARDUINO_ARM_BAUD 9600

#define XBEE_PORT "/dev/ttyUSB0"
#define XBEE_BAUD 19200

#define GPS_LEN 10
#define GPS_IDENTIFIER_INT 71

//#define ARM_XY
#define BASE_PWM 170
#define DIGGER_PWM 125
