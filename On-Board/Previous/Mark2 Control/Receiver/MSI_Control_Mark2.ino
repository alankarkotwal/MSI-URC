#include <Servo.h>
#include <math.h>
int sAngle = 90;

char DC;
char ser;
uint8_t DCSpeed;
uint8_t SerAngle;
uint8_t SerAngle_prev = 128;

void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);
  initialise();
  pinMode(37, OUTPUT);
  pinMode(36, OUTPUT);  
}

void loop()
{
  //forward(128);
  // digitalWrite(37, HIGH);
  // digitalWrite(36, LOW);
  // delay(200);
  if (Serial1.available())
  {
    if (Serial1.read() == ',')
    {
      DC = Serial1.read();
      ser = Serial1.read();
      DCSpeed = DC;
      SerAngle = ser;
//      Serial.print(SerAngle);
//      Serial.print(" ");
//      Serial.println(DCSpeed);
//      if ((SerAngle >= 3 && SerAngle <=50) || (SerAngle >= 170 && SerAngle <=240))
//      {Serial.println(SerAngle);}
//      else {Serial.println(" ");}

      //map (SerAngle, 0, 255, 0, 179);
     /* if (SerAngle >= 200) {
        if (SerAngle_prev >= 20 && SerAngle_prev <= 200)
        {
          Serial.println("right");
          // right(128);
        }
      }
      else if (SerAngle < 20){
        if (SerAngle_prev >= 20 && SerAngle_prev <= 200)
        {
          Serial.println("left");
          //left(128);
        }
      }
      //ackerman(sAngle); //turn the required angle

      SerAngle_prev = SerAngle;*/

      delay(10);
      if (DCSpeed >= 140) //move forward
      {
        map (DCSpeed, 140, 255, 0, 255);
        forward(DCSpeed);
        // Serial.println("forward");
      }
      else if (DCSpeed < 115)
      {
        DCSpeed =  255 - DCSpeed;
        map (DCSpeed, 140, 255, 0, 255);
        backward(DCSpeed);
        // Serial.println("backward");
      }

      else
      {
        stopit();
        // Serial.println("stopit");
      }

      delay(40);
    }
  }
}



