#include <Servo.h>
#include <math.h>
#define PI 3.14159
//PWM pins
int frontLeftSpeed = 2; 
int frontRightSpeed = 7;
int midLeftSpeed = 3;
int midRightSpeed = 6;
int rearLeftSpeed = 4;
int rearRightSpeed = 5;
//int yaw = 12;
//int pitch = 13;

//INA1 pins
int frontLeft1 = 23;
int frontRight2 = 32;
int midLeft2 = 25;
int midRight2 = 30;
int rearLeft2 = 26;
int rearRight2 = 28;

//INA2 pins - for more info, refer to the documentation of motor drivers.
int frontLeft2 = 22;
int frontRight1 = 33;
int midLeft1 = 24;
int midRight1 = 31;
int rearLeft1 = 27;
int rearRight1 = 29;

Servo servoFL, servoFR, servoBL, servoBR;
int thetaFL, thetaFR, thetaBL, thetaBR;
float L1 = 26, L2 = 30, W = 47,R;
//Servo servoYaw; 
//Servo servoPitch;


void initialise()
{
  
  servoFL.attach(8);
  servoFR.attach(11);
  servoBL.attach(9);
  servoBR.attach(10);
  //servoYaw.attach(yaw);
  //servoPitch.attach(pitch);
  //servoYaw.write(90);
  //servoPitch.write(90);
  /*servoFL.write(90);
  servoBL.write(100);
  servoBR.write(115);
  servoFR.write(98);*/


  servoFL.write(86);
  servoBL.write(87); //clockwise
  servoBR.write(90);
  servoFR.write(90);

  pinMode(frontLeftSpeed, OUTPUT);
  pinMode(frontRightSpeed, OUTPUT);
  pinMode(midLeftSpeed, OUTPUT);
  pinMode(midRightSpeed, OUTPUT);
  pinMode(rearLeftSpeed, OUTPUT);
  pinMode(rearRightSpeed, OUTPUT);

  pinMode(frontLeft1, OUTPUT);
  pinMode(frontRight1, OUTPUT);
  pinMode(midLeft1, OUTPUT);
  pinMode(midRight1, OUTPUT);
  pinMode(rearLeft1, OUTPUT);
  pinMode(rearRight1, OUTPUT);

  pinMode(frontLeft2, OUTPUT);
  pinMode(frontRight2, OUTPUT);
  pinMode(midLeft2, OUTPUT);
  pinMode(midRight2, OUTPUT);
  pinMode(rearLeft2, OUTPUT);
  pinMode(rearRight2, OUTPUT);

  digitalWrite(frontLeft1, LOW);
  digitalWrite(frontRight1, LOW);
  digitalWrite(midLeft1, LOW);
  digitalWrite(midRight1, LOW);
  digitalWrite(rearLeft1, LOW);
  digitalWrite(rearRight1, LOW);

  digitalWrite(frontLeft2, LOW);
  digitalWrite(frontRight2, LOW);
  digitalWrite(midLeft2, LOW);
  digitalWrite(midRight2, LOW);
  digitalWrite(rearLeft2, LOW);
  digitalWrite(rearRight2, LOW);

  analogWrite(frontLeftSpeed, 255);
  analogWrite(frontRightSpeed, 255);
  analogWrite(midLeftSpeed, 255);
  analogWrite(midRightSpeed, 255);
  analogWrite(rearLeftSpeed, 255);
  analogWrite(rearRightSpeed, 255);
  
}

void ackerman(uint8_t angle)
{
  angle = (angle/10)*10;
  Serial.println(angle);
  if(angle <= 90)
  {
    thetaFL = angle; //note: the servos used move in anticlockwise direction with increasing angle
    R = (L1/tan(90 - thetaFL)) + (W/2) ;

    thetaFR = 90 - (180 * atan(L1/(R + (W/2))) / PI); 
    thetaBL = 90 + (180 * atan(L2/(R - (W/2))) / PI); 
    thetaBR = 90 + (180 * atan(L2/(R + (W/2))) / PI);
    servoFL.write((int)thetaFL);
    servoBL.write((int)thetaBL);
    servoBR.write((int)thetaBR);
    servoFR.write((int)thetaFR);
//    delay(2);

  }
  else
  {
    thetaFR = angle ;
    R = (L1/tan(thetaFR - 90)) + (W/2) ;

    thetaFL = 90 + (180 * atan(L1/(R + (W/2))) / PI); 
    thetaBL = 90 - (180 * atan(L2/(R - (W/2))) / PI); 
    thetaBR = 90 - (180 * atan(L2/(R + (W/2))) / PI); 
    servoFL.write((int)thetaFL);
    servoBL.write((int)thetaBL);
    servoBR.write((int)thetaBR);
    servoFR.write((int)thetaFR);
  } 
}

void right(int DCSpeed)
{
  servoFL.write(90 + 45);
  servoBL.write(100 + 45);
  servoBR.write(115 - 45);
  servoFR.write(98 + 45);
  DCSpeed = 255 -DCSpeed;
  digitalWrite(frontLeft1, HIGH);
  digitalWrite(frontRight1, HIGH);
  digitalWrite(midLeft1, HIGH);
  digitalWrite(midRight1, HIGH);
  digitalWrite(rearLeft1, HIGH);
  digitalWrite(rearRight1, HIGH);

  digitalWrite(frontLeft2, LOW);
  digitalWrite(frontRight2, LOW);
  digitalWrite(midLeft2, LOW);
  digitalWrite(midRight2, LOW);
  digitalWrite(rearLeft2, LOW);
  digitalWrite(rearRight2, LOW);

  analogWrite(frontLeftSpeed, DCSpeed);
  analogWrite(frontRightSpeed, DCSpeed);
  analogWrite(midLeftSpeed, DCSpeed);
  analogWrite(midRightSpeed, DCSpeed);
  analogWrite(rearLeftSpeed, DCSpeed);
  analogWrite(rearRightSpeed, DCSpeed);
}

void left(int DCSpeed)
{
  servoFL.write(90 - 45);
  servoBL.write(100 - 45);
  servoBR.write(115 + 45);
  servoFR.write(98 - 45);
  DCSpeed = 255 -DCSpeed;
  digitalWrite(frontLeft1, HIGH);
  digitalWrite(frontRight1, HIGH);
  digitalWrite(midLeft1, HIGH);
  digitalWrite(midRight1, HIGH);
  digitalWrite(rearLeft1, HIGH);
  digitalWrite(rearRight1, HIGH);

  digitalWrite(frontLeft2, LOW);
  digitalWrite(frontRight2, LOW);
  digitalWrite(midLeft2, LOW);
  digitalWrite(midRight2, LOW);
  digitalWrite(rearLeft2, LOW);
  digitalWrite(rearRight2, LOW);

  analogWrite(frontLeftSpeed, DCSpeed);
  analogWrite(frontRightSpeed, DCSpeed);
  analogWrite(midLeftSpeed, DCSpeed);
  analogWrite(midRightSpeed, DCSpeed);
  analogWrite(rearLeftSpeed, DCSpeed);
  analogWrite(rearRightSpeed, DCSpeed);
}
void forward (int DCSpeed)
{
 // Serial.println("forward");
  servoFL.write(86);
  servoBL.write(87); //clockwise
  servoBR.write(90);
  servoFR.write(90);
  DCSpeed = 255 -DCSpeed;
  digitalWrite(frontLeft1, HIGH);
  digitalWrite(frontRight1, HIGH);
  digitalWrite(midLeft1, HIGH);
  digitalWrite(midRight1, HIGH);
  digitalWrite(rearLeft1, HIGH);
  digitalWrite(rearRight1, HIGH);

  digitalWrite(frontLeft2, LOW);
  digitalWrite(frontRight2, LOW);
  digitalWrite(midLeft2, LOW);
  digitalWrite(midRight2, LOW);
  digitalWrite(rearLeft2, LOW);
  digitalWrite(rearRight2, LOW);

  analogWrite(frontLeftSpeed, DCSpeed);
  analogWrite(frontRightSpeed, DCSpeed);
  analogWrite(midLeftSpeed, DCSpeed);
  analogWrite(midRightSpeed, DCSpeed);
  analogWrite(rearLeftSpeed, DCSpeed);
  analogWrite(rearRightSpeed, DCSpeed);
  
}

void backward(int DCSpeed)
{
  servoFL.write(104);
  servoBL.write(110); //clockwise
  servoBR.write(76);
  servoFR.write(76);
  
//  Serial.println("backward");
  DCSpeed = 255 -DCSpeed;
  digitalWrite(frontLeft1, LOW);
  digitalWrite(frontRight1, LOW);
  digitalWrite(midLeft1, LOW);
  digitalWrite(midRight1, LOW);
  digitalWrite(rearLeft1, LOW);
  digitalWrite(rearRight1, LOW);

  digitalWrite(frontLeft2, HIGH);
  digitalWrite(frontRight2, HIGH);
  digitalWrite(midLeft2, HIGH);
  digitalWrite(midRight2, HIGH);
  digitalWrite(rearLeft2, HIGH);
  digitalWrite(rearRight2, HIGH);

  analogWrite(frontLeftSpeed, DCSpeed);
  analogWrite(frontRightSpeed, DCSpeed);
  analogWrite(midLeftSpeed, DCSpeed);
  analogWrite(midRightSpeed, DCSpeed);
  analogWrite(rearLeftSpeed, DCSpeed);
  analogWrite(rearRightSpeed, DCSpeed);
  
}

void stopit()
{
//  Serial.println("stopit");
  digitalWrite(frontLeft1, HIGH);
  digitalWrite(frontRight1, HIGH);
  digitalWrite(midLeft1, HIGH);
  digitalWrite(midRight1, HIGH);
  digitalWrite(rearLeft1, HIGH);
  digitalWrite(rearRight1, HIGH);

  digitalWrite(frontLeft2, HIGH);
  digitalWrite(frontRight2, HIGH);
  digitalWrite(midLeft2, HIGH);
  digitalWrite(midRight2, HIGH);
  digitalWrite(rearLeft2, HIGH);
  digitalWrite(rearRight2, HIGH);

  analogWrite(frontLeftSpeed, 255);
  analogWrite(frontRightSpeed, 255);
  analogWrite(midLeftSpeed, 255);
  analogWrite(midRightSpeed, 255);
  analogWrite(rearLeftSpeed, 255);
  analogWrite(rearRightSpeed, 255); 
  
}

