
float dx = 0 ;
float x1 = 16.0 ;
float k = 3 ;
float V = 0;

void setup()
{
Serial.begin(9600);
}
void loop()
{
float x = analogRead(A2);

  
  x = (x - 360)/16 ;
  dx = x1 - x ;
  
  Serial.println(dx);
  V = k * dx ;
  
  if (V <0)
  {
    V= -V;
  V = map(V ,0,(17.2 - x1)*k , 0 , 255);
  analogWrite(9 , V);
  analogWrite(10, 0);
Serial.println(V);
  }
  else if (V > 0)
  {
  V = map(V , 0 ,x1*k , 0 , 255);
  analogWrite(10 , V);
  analogWrite(9,0);
  Serial.println(V);
  }
  
}
