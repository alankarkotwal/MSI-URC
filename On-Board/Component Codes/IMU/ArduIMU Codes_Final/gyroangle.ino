void getangle2()
{
 long t=0;
 float intgX= 0;
 float intgY= 0;
 float intgZ= 0;
 
 omegaX= gyroX*18/3.14;
 omegaY= gyroY*18/3.14;
 omegaZ= gyroZ*180/3.14;

 t = millis();
 if(omegaX>-0.181648 && omegaX<-0.322432)
 {  
 while (millis()<=1000+t)
 {
 intgX= intgX + (omegaX *Dt);
 }
 sumX = sumX +intgX;
 }
 else{
 sumX=0.0;
 }
 if(omegaY>0.091345 && omegaY<0.08708)
 {
 while (millis()<=1000+t)
   {
     intgY= intgY + (omegaY *Dt);
   }
   sumY = sumY +intgY;
 }
 else{
   sumY=0.0;
 }
 if(omegaZ>0.047048 && omegaZ<0.04059)
 {
 while (millis()<=1000+t)
 {
 intgZ= intgZ + (omegaZ *Dt);
}
 sumZ = sumZ +intgZ;
 }
 else{
 sumZ=0.0;
 }
}
