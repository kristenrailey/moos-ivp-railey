//File: AcousticPath.h

#include <iostream>
#include <math.h>
#include <cmath>
#include <list>


class AcousticPath
{

 public:
  AcousticPath(){};
  ~AcousticPath() {};


  //Set functions for initial params
  void setSurfaceSoundSpeed(float c_0) {m_c_0=c_0;};
  void setTimeInterval(float time_int){m_time_int=time_int;};
  void setWaterDepth(float water_depth){m_water_depth=water_depth;};
  void setSoundSpeedGrad(float gradient){m_gradient= gradient;};
  //Calculation functions for Acoustic Path
  float calcProj_r(float x_rec,float y_rec, float x_src, float y_src);
  float calcC(float z);
  float calcMidpt(float r_1,float r_2);
  float calcPerpSlope(float r_1,float z_1, float r_2, float z_2);
  float calcPerpIntercept(float slope, float midpt_r,float midpt_z);
  float calcCircCenter_r(float z_center, float int_slope, float int_b);

  float calcCircCenter_z();
  float calcRBisect(float r_1,float z_1,float r_center, float z_center);
  float calcThetaSrc(float R,float z_src);
  bool checkValidR(float R);
  float calcValidR(float R_current);
  float calcNewCircCenter_r(float z_rec, float r_rec,float R_new,float circ_z_center);
  float calcPosOnCirc_r(float circ_z_center,float circ_r_center, float z_src, float R);
  float convertRad2Degrees(float angle);

  //Calculation functions for Transmission Loss
  float calcArcLength(float R, float theta_src, float theta_rec);
  float calcThetaRec(float z_src, float z_rec, float theta_src);
  float calc_r(float theta_src,float s, float R);
  float calcJ(float s, float theta_src, float d_theta, float R);
  float calcPfromArcLength(float z_src, float z_rec, float J, float theta_src);
  float calcTransmissionLoss(float theta_src, float z_src, float z_rec, float R, float d_theta);



  protected:
  //Inputs
    float m_c_0, m_water_depth;
    float m_time_int, m_gradient;
    
 
  


};
