/************************************************************/
/*    NAME: Kristen Railey                                              */
/*    ORGN: MIT                                             */
/*    FILE: Odometry.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "Odometry.h"
#include <cmath>

using namespace std;

//---------------------------------------------------------
// Constructor

Odometry::Odometry()
{
  //Initialize the member variables
  m_first_reading_x = false;
  m_first_reading_y = false;
  m_current_x = 0.0;
  m_current_y = 0.0;
  m_previous_x = 0.0;
  m_previous_y = 0.0;
  m_total_distance = 0.0;
  m_dist_temp = 0.0;
}

//---------------------------------------------------------
// Destructor

Odometry::~Odometry()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool Odometry::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail); //Added for appcasting
  MOOSMSG_LIST::iterator p;
  //Reset first reading to true
  m_first_reading_x = true;
  m_first_reading_y = true;
    
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key = msg.GetKey();
    double dval = msg.GetDouble(); 
    
    if (key =="NAV_X"){
      // Check if first reading, x
      if (m_first_reading_x){
        m_first_x = msg.GetDouble();
	m_current_x = m_first_x;
	m_first_reading_x=false;
      }
      else{
	m_previous_x=m_current_x; 
	m_current_x = dval;
      }
    }
     if (key =="NAV_Y"){
      // Check if first reading, y
      if (m_first_reading_y){
        m_first_y = msg.GetDouble();
	m_current_y = m_first_y;
	m_first_reading_y=false;
      }
      else{
	m_previous_y=m_current_y;
	m_current_y = dval;
      }
     }

     
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

	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool Odometry::OnConnectToServer()
{
   // register for variables here
   // m_MissionReader.GetConfigurationParam("Name", <string>);
   // m_Comms.Register("VARNAME", 0);
	
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool Odometry::Iterate()
{
  AppCastingMOOSApp::Iterate(); //Added for appcasting
  m_dist_temp=sqrt(pow((m_current_x-m_first_x),2)+pow((m_current_y-m_first_y),2));
  m_total_distance+=m_dist_temp;
  Notify("ODOMETRY_DIST",m_total_distance);

  AppCastingMOOSApp::PostReport(); //Added for appcasting
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool Odometry::OnStartUp()
{
  AppCastingMOOSApp::OnStartUp(); 
  list<string> sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
    list<string>::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
      string original_line = *p;
      string param = stripBlankEnds(toupper(biteString(*p, '=')));
      string value = stripBlankEnds(*p);
      
      if(param == "FOO") {
        //handled
      }
      else if(param == "BAR") {
        //handled
      }
    }
  }
  
  RegisterVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void Odometry::RegisterVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  Register("NAV_X",0);
  Register("NAV_Y",0);
  
  
}

//------------------------------------------------------------
// Procedure: AppCasting BuildReport

bool Odometry::buildReport()
{
  m_msgs<<"Distance Travelled: "<<m_total_distance<<std::endl;
  return (true); //Added this for build
}
