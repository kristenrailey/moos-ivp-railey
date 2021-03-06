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
  void setSurfaceSoundSpeed(double c_0) {m_c_0=c_0;};
  void setTimeInterval(double time_int){m_time_int=time_int;};
  void setWaterDepth(double water_depth){m_water_depth=water_depth;};
  void setSoundSpeedGrad(double gradient){m_gradient= gradient;};
  //Calculation functions for Acoustic Path
  double calcProj_r(double x_rec,double y_rec, double x_src, double y_src);
  double calcProj_theta(double x_rec,double y_rec, double x_src, double y_src);

  double calcC(double z);
  double calcMidpt(double r_1,double r_2);
  double calcPerpSlope(double r_1,double z_1, double r_2, double z_2);
  double calcPerpIntercept(double slope, double midpt_r,double midpt_z);
  double calcCircCenter_r(double z_center, double int_slope, double int_b);

  double calcCircCenter_z();
  double calcRBisect(double r_1,double z_1,double r_center, double z_center);
  double calcThetaSrc(double R,double z_src);
  bool checkValidR(double R);
  double calcValidR(double R_current);
  double calcNewCircCenter_r(double z_rec, double r_rec,double R_new,double circ_z_center);
  double calcPosOnCirc_r(double circ_z_center,double circ_r_center, double z_src, double R);
  double convertRad2Degrees(double angle);

  //Calculation functions for Transmission Loss
  double calcArcLength(double R, double theta_src, double theta_rec);
  double calcThetaRec(double z_src, double z_rec, double theta_src);
  double calc_r(double theta_src,double s, double R);
  double calcJ(double s, double theta_src, double d_theta, double R);
  double calcPfromArcLength(double z_src, double z_rec, double J, double theta_src);
  double calcTransmissionLoss(double theta_src, double z_src, double z_rec, double R, double d_theta);



  protected:
  //Inputs
    double m_c_0, m_water_depth;
    double m_time_int, m_gradient;
    
 
  


};
