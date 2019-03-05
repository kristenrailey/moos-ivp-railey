/*****************************************************
 * Acoustic Path.cpp
 * Class for calculating acoustic path between receiver
 * and source, for a linear sound profile. 
 ****************************************************/
#include "AcousticPath.h"
#define PI_KR  3.14159265

//calcProj_r
//Using x,y positions of src and rec, calculate r_src, and remember to set r_rec = 0
float AcousticPath::calcProj_r(float x_rec,float y_rec, float x_src, float y_src){
  float dist_0_rec = sqrt(pow(x_rec,2)+pow(y_rec,2));
  float dist_0_src = sqrt(pow(x_src,2)+pow(y_src,2));
  std::cout<<"Distances calculated:(rec) "<<dist_0_rec<<" (src) "<<dist_0_src<<std::endl;
  return (dist_0_rec-dist_0_src);
  
}

//calcC(z)
// Calculate speed of sound, c from depth z and speed of sound at depth 0, c_0
float AcousticPath::calcC(float z){
  return m_c_0+m_gradient*z;
}


//calcMidpt_r(r_1,r_2)
//Calculate midpoint between two points
float AcousticPath::calcMidpt(float r_1,float r_2){
  return (r_1+r_2)/2;
}

//calcPerpSlope(r_1,z_1,r_2,z_2)
//Calculate perpendicular slope from two points
float AcousticPath::calcPerpSlope(float r_1,float z_1, float r_2, float z_2){
  float slope=(z_2-z_1)/(r_2-r_1); 
  return -1/slope;
}

//Calculate z-intercept for a given slope and r,z points.
//In this case, that's the perpendicular slope and midpt.
float AcousticPath::calcPerpIntercept(float slope, float midpt_r,float midpt_z){
  // y =mx+b, solve for b
  return  midpt_z-slope*midpt_r;
}


float AcousticPath::calcCircCenter_r(float z_center, float int_slope, float int_b){
  //Given intercepting line with circle center
  float r_center=(z_center-int_b)/int_slope;
  return r_center;
}

float AcousticPath::calcCircCenter_z(){
  return -m_c_0/m_gradient;
}


float AcousticPath::calcRBisect(float r_1,float z_1,float r_center, float z_center){
  return sqrt(pow((r_center-r_1),2)+pow((z_center-z_1),2));

}


//Calculate transmitter/source angle for a given radius and source depth
float AcousticPath::calcThetaSrc(float R, float z_src){
  //Undid for now - Negative accounting for coordinate system (z is positive, increasing depth)
  return acos((calcC(z_src))/(R*m_gradient));
}

float AcousticPath::convertRad2Degrees(float angle){
  return angle * 180 / PI_KR;
}

//Check if radius from circle is less than water depth
bool AcousticPath::checkValidR(float R){
  if ((R-m_c_0/m_gradient) > m_water_depth){
    return false;
  }
  else{
    return true;
  }
}

//Calculate a valid radius by accounting for water depth
float AcousticPath::calcValidR(float R_current){
  float R_new= R_current-(R_current-m_water_depth);
  return R_new;
}

float AcousticPath::calcNewCircCenter_r(float z_rec, float r_rec, float R_new,float circ_z_center){
  //Recalculate circle center ,since R is invalid assume depth is the same (z_src)
  float temp_term=sqrt(pow(R_new,2)-pow((z_rec-circ_z_center),2));
  float temp_r_new=-(temp_term-r_rec);
  return temp_r_new;  
  }

float AcousticPath::calcPosOnCirc_r(float circ_z_center,float circ_r_center, float z_src, float R){
  // Calculate r given circle center and z-pos of source
  float temp_term=sqrt(pow(R,2)-pow((z_src-circ_z_center),2));
  float r_pos=(temp_term+circ_r_center);
  return r_pos;
}

float AcousticPath::calcArcLength(float R, float theta_src, float theta_rec){
  float s = R*(theta_src-theta_rec);
  return s;
}

float AcousticPath::calcThetaRec(float z_src, float z_rec, float theta_src){
  float c_rec = calcC(z_rec);
  float c_src = calcC(z_src);
  return acos(c_rec/c_src*cos(theta_src));
}

//Calculte r from theta and arc length
//r(s) = R(sin(theta_src)+sin(s/R - theta_src))
float AcousticPath::calc_r(float theta_src,float s, float R){
  return R*(sin(theta_src)+sin(s/R-theta_src));
}

float AcousticPath::calcJ(float s, float theta_src, float d_theta, float R){
  float r_i=calc_r(theta_src,s,R);
  float r_i1=calc_r(theta_src+d_theta,s,R);
  return ((r_i)/sin(theta_src))*((r_i1-r_i)/d_theta);
}

float AcousticPath::calcPfromArcLength(float z_src, float z_rec, float J, float theta_src){
  float temp = std::abs((calcC(z_rec)*cos(theta_src))/(calcC(z_src)*J));
  return 1/(4*PI_KR)*sqrt(temp);

}

float AcousticPath::calcTransmissionLoss(float theta_src, float z_src, float z_rec, float R, float d_theta){
  float theta_rec = calcThetaRec(z_src, z_rec, theta_src);
  float s = calcArcLength(R,theta_src, theta_rec);
  float J = calcJ(s,theta_src,d_theta, R);
  float P_of_s = calcPfromArcLength(z_src, z_rec, J, theta_src);
  float P_1 = 1/(4*PI_KR);
  float TL = -20*log10(P_of_s/P_1);
  return TL;

}
////////////////////////////////////////////////////////////////////////////


// Example Code for running acoustic path
/*int main(){
  // x = r dimension
  float r_rec,z_rec,r_src,z_src;
  float c_0=1480.0; //surface sound speed
  float sound_speed_gradient=0.016;
  float water_depth=6000;
//Declare Acoustic Path object
AcousticPath my_acoustic_path;
// setRPosRec( ...); 

  //Find circle equation, given source and receiver position
  float midpt_r=calcMidpt(r_src,r_rec);
  float midpt_z=calcMidpt(z_src,z_rec);
  float int_slope =calc_perp_slope(r_src,z_src, r_rec, z_rec);
  float int_b=calc_perp_b(int_slope, midpt_r,midpt_z);
  float circ_z_center=calc_circ_z_center(c_0);
  float circ_r_center = calc_circ_r_center(circ_z_center, int_slope, int_b);
  float R_bisect = calc_R_bisec(r_src,z_src,circ_r_center,circ_z_center);
  //Check if valid R.   
  bool valid_R=check_valid_R(water_depth,R_bisect,c_0);


  if (valid_R){

      //Calculate steering angle
    float theta = calc_theta_src(R_bisect,z_src,c_0);

    }
  else{
    //Calc new R.
    float R_new = calc_valid_R(R_bisect,water_depth);

    //Recalculate position, keep depth (z_src)
    r_src= calc_r_pos_circ(circ_z_center, circ_r_center, R_new, z_src);
  }

  
  // TEST CODE
  std::cout<<"Points (1,3), (4,10), midpoint: "<<calc_midpt_r(1,4)<<", "<<calc_midpt_z(3,10)<<std::endl;

  std::cout<<"Perpendicular Slope: "<<calc_perp_slope(2,16,-1,3)<<std::endl;
  std::cout<<"Calcluate y intercept: "<<calc_perp_b(2,-1,10)<<std::endl;
  std::cout<<"Calc R bisect: "<< calc_R_bisec(2,-100,15,30)<<std::endl;



  return 0;
  }*/

