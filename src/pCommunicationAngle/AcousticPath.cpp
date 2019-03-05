/*****************************************************
 * Acoustic Path.cpp
 * Class for calculating acoustic path between receiver
 * and source, for a linear sound profile. 
 ****************************************************/
#include "AcousticPath.h"

//calcProj_r
//Calculate projected r from x- and y- position.
float AcousticPath::calcProj_r(float x_pos,float y_pos){
  return sqrt(pow(x_pos,2)+pow(y_pos,2));
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
  return acos((calcC(z_src))/(R*m_gradient))* 180.0 / 3.14159265;
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

