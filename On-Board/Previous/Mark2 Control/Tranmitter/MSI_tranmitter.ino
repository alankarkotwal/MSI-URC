#define LEFT_VER 0
#define LEFT_HOR 1
#define RIGHT_VER 4
#define RIGHT_HOR 5

#define LEFT_BUTTON 4
#define RIGHT_BUTTON 5
//#define DEBUG

void setup()
{
  Serial.begin(9600);
  
  pinMode(LEFT_BUTTON, INPUT);
  pinMode(RIGHT_BUTTON, INPUT);
}

void loop()
{
  #ifndef DEBUG
    /*int leftVer = 0, rightHor = 0;
    for (int i = 0; i < 10; i++)
    {
      leftVer += AnalogRead(LEFT_VER);
      rightHor += AnalogRead(RIGHT_HOR);
    }
    leftVer = leftVer/10;
    rightHor = rightHor/10;*/
    Serial.write(',');
    //Serial.write(leftVer);
    Serial.write(AnalogRead(LEFT_HOR));
    Serial.write(AnalogRead(RIGHT_VER));
    //Serial.write(rightHor);
    //Serial.write(digitalRead(LEFT_BUTTON));
    //Serial.write(digitalRead(RIGHT_BUTTON));
    delay(50);
  #endif
  #ifdef DEBUG
    Serial.println(',');
    Serial.println(AnalogRead(LEFT_VER));
    //Serial.println(AnalogRead(LEFT_HOR));
    //Serial.println(AnalogRead(RIGHT_VER));
    Serial.println(AnalogRead(RIGHT_HOR));
    //Serial.println(digitalRead(LEFT_BUTTON));
    //Serial.println(digitalRead(RIGHT_BUTTON));
    delay(1000);
  #endif
}

int AnalogRead(int pin)
{
  if(pin==LEFT_VER) return analogRead(A0)/4;
  else if(pin==LEFT_HOR) return analogRead(A1)/4;
  else if(pin==RIGHT_VER) return analogRead(A4)/4;
  else if(pin==RIGHT_HOR) return analogRead(A5)/4;
  else return 0;
}
