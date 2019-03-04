//File: AcousticPath.h

#include <iostream>
#include <math.h>
#include <cmath>
#include <list>

//#define PI 3.14159265

class AcousticPath
{

 public:
  AcousticPath(){};
  ~AcousticPath() {};


  //Set variables
  void setSurfaceSoundSpeed(float c_0) {m_c_0=c_0;};
  void setTimeInterval(float time_int){m_time_int=time_int;};
  void setWaterDepth(float water_depth){m_water_depth=water_depth;};
  void setSoundSpeedGrad(float gradient){m_gradient= gradient;};
  //Calculation functions
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

  protected:
  //Inputs
    float m_c_0, m_water_depth;
    float m_time_int, m_gradient;
 
  


};
