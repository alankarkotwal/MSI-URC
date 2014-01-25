
void getomega(){
 //omegaX= gyroX*18/3.14;
 //omegaY= gyroY*18/3.14;
 //omegaZ= gyroZ*180/3.14;

if(gyroX>-0.04 && gyroX<-0.01){

omegaX=0.0;
}
else if(gyroX<-0.04){
omegaX=gyroX*24.5;//25.16

}
else{
omegaX=gyroX*42;
}
if(gyroY> -0.2 && gyroY<0.03){
omegaY=0.0;
 
}
else{

omegaY=gyroY*29.14;
}
if(gyroZ<0.24 && gyroZ>0.21){
omegaZ=0.0;
}
else if(gyroZ<0.21){
omegaZ=(gyroZ-0.22)*31.33;
}
else{
omegaZ=(gyroZ-0.23)*28;//27.98
}
//  theta_ctrl[i]=(omegaZ-a1*theta_ctrl[i-1]-a2*theta_ctrl[i-2]-a3*theta_ctrl[i-3]-a4*theta_ctrl[i-4]-a5*theta_ctrl[i-5])/a0;
//  omegarealZ=theta_ctrl[i];
//  if (i<11){
//  i++;
//  }
//  else{
//  theta_ctrl[i-5]=theta_ctrl[i-4];
//  theta_ctrl[i-4]=theta_ctrl[i-3];
//  theta_ctrl[i-3]=theta_ctrl[i-2];
//  theta_ctrl[i-2]=theta_ctrl[i-1];
//  theta_ctrl[i-1]=theta_ctrl[i];
//  }


}
