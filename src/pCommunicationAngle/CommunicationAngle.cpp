/************************************************************/
/*    NAME: Kristen Railey                                              */
/*    ORGN: MIT                                             */
/*    FILE: CommunicationAngle.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "CommunicationAngle.h"

using namespace std;

//---------------------------------------------------------
// Constructor

CommunicationAngle::CommunicationAngle()
{
}

//---------------------------------------------------------
// Destructor

CommunicationAngle::~CommunicationAngle()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool CommunicationAngle::OnNewMail(MOOSMSG_LIST &NewMail)
{
  MOOSMSG_LIST::iterator p;
   
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key   = msg.GetKey();
    double dval  = msg.GetDouble();

    if (key == "NAV_X"){
      m_r_src = dval;
    }
    else if (key == "NAV_DEPTH"){
      m_z_src = dval;
    }
    else if (key == "'collaborator'_NAV_X"){
      m_r_rec = dval;
    }
    else if (key == "'collaborator'_NAV_DEPTH"){
      m_z_rec = dval;
    }
    


#if 0 // Keep these around just for template
    string key   = msg.GetKey();
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool CommunicationAngle::OnConnectToServer()
{
   // register for variables here
   // possibly look at the mission file?
   // m_MissionReader.GetConfigurationParam("Name", <string>);
   // m_Comms.Register("VARNAME", 0);
	
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool CommunicationAngle::Iterate()
{
   //Find circle equation, given source and receiver position
  m_midpt_r=m_acoustic_path.calcMidpt(m_r_src,m_r_rec);
  m_midpt_z=m_acoustic_path.calcMidpt(m_z_src,m_z_rec);
  m_int_slope =m_acoustic_path.calcPerpSlope(m_r_src,m_z_src, m_r_rec, m_z_rec);
  m_int_b=m_acoustic_path.calcPerpIntercept(m_int_slope, m_midpt_r,m_midpt_z);
  m_circ_z_center=m_acoustic_path.calcCircCenter_z();
  m_circ_r_center = m_acoustic_path.calcCircCenter_r(m_circ_z_center, m_int_slope, m_int_b);
  m_R_bisect = m_acoustic_path.calcRBisect(m_r_src,m_z_src,m_circ_r_center,m_circ_z_center);
  
  //Check if valid R.   
  m_valid_R=m_acoustic_path.checkValidR(m_R_bisect);


  if (m_valid_R){
      //Calculate steering angle
    m_theta_src = m_acoustic_path.calcThetaSrc(m_R_bisect,m_z_src);
    std::cout<<"Steering angle: "<<m_theta_src<<std::endl;

    }
  else{
    //Calc new R and circle (r) center
    m_R_new = m_acoustic_path.calcValidR(m_R_bisect);
    m_circ_r_center_new=m_acoustic_path.calcNewCircCenter_r(m_z_rec, m_r_rec,m_R_new,m_circ_z_center);
    //Recalculate position, keep depth (z_src)
    m_r_src= m_acoustic_path.calcPosOnCirc_r(m_circ_z_center, m_circ_r_center_new,m_z_src, m_R_new);
    std::cout<<"Recalculated new position; m_R_bisect: "<<m_R_bisect<<", m_R_new"<<std::endl;
  }

  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool CommunicationAngle::OnStartUp()
{
  list<string> sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
    list<string>::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
      string original_line = *p;
      string param = stripBlankEnds(toupper(biteString(*p, '=')));
      string value = stripBlankEnds(*p);
      std::cout<<"param: "<<param<<std::endl;
      //Convert value to float
      stringstream temp(value);
      float fvalue = 0;
      temp >> fvalue;
      
      if(param == "SURFACE_SOUND_SPEED") {
        m_acoustic_path.setSurfaceSoundSpeed(fvalue);
      }
      else if(param == "SOUND_SPEED_GRADIENT") {
	m_acoustic_path.setSoundSpeedGrad(fvalue);
	
      }
      else if(param == "WATER_DEPTH"){
	m_acoustic_path.setWaterDepth(fvalue);
      }
      else if(param == "TIME_INTERVAL"){
	
	m_acoustic_path.setTimeInterval(fvalue);
      }
    }
  }
  
  RegisterVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void CommunicationAngle::RegisterVariables()
{
  // Register("FOOBAR", 0);
  Register("VEHICLE_NAME",0);
  Register("COLLABORATOR_NAME",0);
  Register("NAV_X",0);
  Register("NAV_Y",0);
  Register("NAV_DEPTH",0);
  Register("NAV_HEADING",0);
  Register("NAV_SPEED",0);
  Register("'collaborator'_NAV_X",0);
  Register("'collaborator'_NAV_Y",0);
  Register("'collaborator'_NAV_DEPTH",0);
  Register("'collaborator'_NAV_HEADING",0);
  Register("'collaborator'_NAV_SPEED",0);
  
}

