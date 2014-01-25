
void get_accAngle()
{
  RaccX = (accelX*v_ref/1024 - v_z_g)/acc_sens;
  RaccY = (accelY*v_ref/1024 - v_z_g)/acc_sens;
  RaccZ = (accelZ*v_ref/1024 - v_z_g)/acc_sens;
  
  R = sqrt(RaccX*RaccX+RaccY*RaccY+RaccZ*RaccZ);
  
  axR = acos(RaccX/R);
  ayR = acos(RaccY/R);
  azR = acos(RaccZ/R);
}

void get_gyroAngle()
{
  gyroRate_X = (gyroX*v_ref/1024 - v_z_g)/gyro_sens;  
  gyroRate_Y = (gyroY*v_ref/1024 - v_z_g)/gyro_sens;
}
