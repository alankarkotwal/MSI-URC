void getangle()
{
 
 
// omegaX= gyroX*180/3.14;
 //omegaY= gyroY*180/3.14;
 //omegaZ= gyroZ*180/3.14;



 intgX= intgX + (omegaX *Dt);
 intgY= intgY + (omegaY *Dt);
 intgZ= intgZ + (omegaZ *Dt);
// Serial.print(intgX);
// Serial.print ("\t");
// Serial.print(intgY);
// Serial.print ("\t");
// Serial.print(intgZ);
// Serial.print ("\t"); 

}
