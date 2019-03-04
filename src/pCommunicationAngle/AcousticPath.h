//File: AcousticPath.h

#include <iostream>
#include <math.h>
#include <cmath>
#include <list>

#define G 9.8

class AcousticPath
{

 public:
  AcousticPath(){};
  ~AcousticPath() {};


  //Set variables
  void setSurfaceSoundSpeed(float c_0) {m_c_0=c_0;};
  void setRPosRec(float r_rec){m_r_rec=r_rec;};
  void setZPosRec(float z_rec){m_z_rec=z_rec;};
  void setRPosSrc(float r_src){m_r_src=r_src;};
  void setZPosSrc(float z_src){m_z_src=z_src;};
  void setWaterDepth(float water_depth){m_water_depth=water_depth;};
   
  //Calculation functions
  float calcC(float z);
  float calcMidpt(float r_1,float r_2);
  float calcPerpSlope(float r_1,float z_1, float r_2, float z_2);

  float calcPerpIntercept(float slope, float midpt_r,float midpt_z);
  float calcCircCenter_r(float z_center, float int_slope, float int_b);

  float calcCircCenter_z();
  float calcRBisect(float r_1,float z_1,float r_center, float z_center);
  float calcThetaSrc(float R);
  bool checkValidR(float R);
  float calcValidR(float R_current);


  float calcNewCircCenter_r(float R_new,float circ_z_center);
  float calcPosOnCirc_r(float circ_z_center,float circ_r_center, float R);

  protected:
  //Inputs
    float m_c_0, m_r_rec, m_z_rec, m_r_src, m_z_src, m_water_depth;
 
  


};
