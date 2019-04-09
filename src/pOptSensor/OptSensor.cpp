/************************************************************/
/*    NAME: Kristen Railey                                              */
/*    ORGN: MIT                                             */
/*    FILE: OptSensor.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "OptSensor.h"
#include <iostream>

using namespace std;

//---------------------------------------------------------
// Constructor

OptSensor::OptSensor()
{
  m_penalty_missed_hazard = 0.0;
  m_penalty_nonopt_hazard = 0.0; //?
  m_penalty_false_alarm = 0.0;
  m_penalty_max_time_over = 0.0;
  m_max_time =0.0;
  m_penalty_max_time_rate = 0.0;
  m_transit_path_width = 0.0; //?
  m_search_reg_x_min = 10000.0;
  m_search_reg_y_min = 10000.0;
  m_search_reg_x_max = -10000.0;
  m_search_reg_y_max = -10000.0;

  
}

//---------------------------------------------------------
// Destructor

OptSensor::~OptSensor()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool OptSensor::OnNewMail(MOOSMSG_LIST &NewMail)
{
  MOOSMSG_LIST::iterator p;
   
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key   = msg.GetKey();
    string sval  = msg.GetString();
    std::cout<<"Message: "<<key<<", val: "<<sval<<std::endl;
    if (key == "UHZ_OPTIONS_SUMMARY"){
      //      std::cout<<"options summary: "<<sval<<std::endl;
      handleMailSensorOptionsSummary(sval);
    }
    if(key == "UHZ_MISSION_PARAMS"){
      std::cout<<"mission params: "<<sval<<std::endl;
      handleMailMissionParams(sval);

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

bool OptSensor::OnConnectToServer()
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

bool OptSensor::Iterate()
{
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool OptSensor::OnStartUp()
{
  list<string> sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
    list<string>::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
      string line  = *p;
      string param = tolower(biteStringX(line, '='));
      string value = line;
      
      if(param == "foo") {
        //handled
      }
      else if(param == "bar") {
        //handled
      }
    }
  }
  
  RegisterVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void OptSensor::RegisterVariables()
{
  // Register("FOOBAR", 0);
  Register("UHZ_MISSION_PARAMS",0);
  Register("UHZ_OPTIONS_SUMMARY",0);
}

//---------------------------------------------------------                                      
// Procedure: handleMailMissionParams                                                            
//   Example: UHZ_MISSION_PARAMS = penalty_missed_hazard=100,                                    
//                       penalty_nonopt_hazard=55,                                               
//                       penalty_false_alarm=35,                                                 
//                       penalty_max_time_over=200,                                              
//                       penalty_max_time_rate=0.45,                                             
//                       transit_path_width=25,                                                  
//                       search_region = pts={-150,-75:-150,-50:40,-50:40,-75}                   



void OptSensor::handleMailMissionParams(std::string str)
{ 
  std::cout<<"handle mail mission param: "<<str<<std::endl;
  vector<string> svector = parseStringZ(str, ',', "{");
  unsigned int i, vsize = svector.size();
  for(i=0; i<vsize; i++) {
    string param = biteStringX(svector[i], '=');
    string value = svector[i];
    // This needs to be handled by the developer. Just a placeholder.                            
    std::cout<<"mission params: "<<param<<", "<<value<<std::endl;
    stringstream ss;
    ss<<value;
    if (i == 0){
      ss>>m_penalty_missed_hazard;
      std::cout<<"missed hazard:"<<m_penalty_missed_hazard<<std::endl;
    }
    else if (i==1){
      ss>>m_penalty_nonopt_hazard;
    }
    else if (i==2){
      ss>>m_penalty_false_alarm;
    }
    else if (i==3){
      ss>>m_penalty_max_time_over;
      std::cout<<"max time over: "<<m_penalty_max_time_over<<std::endl;
    }
    else if (i==4){
      ss>>m_max_time;
    }
    else if (i==5){
      ss>>m_penalty_max_time_rate;
    }
    else if (i==6){
      ss>>m_transit_path_width;
    }
    else if (i==7){ //Search region
      std::cout<<"search region"<< value<<std::endl;
      string pts_str = tokStringParse(value, "search_region = pts=", ',', '=');
      string search_region_str_left = biteStringX(value, '{');
      std::cout<<"new string: "<<pts_str<<std::endl;
      string final_pts_str = biteStringX(value,'}');
      std::cout<<"final string: "<<final_pts_str<<std::endl;
      vector<string> str_vector_pts = parseString(final_pts_str, ':');
      
      
      
      
      
      
      for (int i = 0; i<4; i++){
	vector<string> str_vector_xy = parseString(str_vector_pts[i], ',');
	stringstream xx,yy;
	double temp_x = 0.0;
	double temp_y=0.0;
	xx<<str_vector_xy[0];
	xx>>temp_x;
	yy<<str_vector_xy[1];
	yy>>temp_y;
	std::cout<<"temp x: "<<temp_x<<", temp y: "<<temp_y<<std::endl;
	if (temp_x>m_search_reg_x_max){
	  m_search_reg_x_max = temp_x;
	}
	if (temp_y>m_search_reg_y_max){
	  m_search_reg_y_max = temp_y;
	}
	if (temp_x<m_search_reg_x_min){
	  m_search_reg_x_min = temp_x;
	}
	if (temp_y<m_search_reg_y_min){
	  m_search_reg_y_min = temp_y;
	}
      }
      std::cout<<"min x: "<<m_search_reg_x_min<<" max x: "<<m_search_reg_x_max<<std::endl;
      std::cout<<"min y: "<<m_search_reg_y_min<<" max y: "<<m_search_reg_y_max<<std::endl;

     
    
    
    
    

    }
  }
}


//----------------------------------------------------
// Procedure: handleMailSensorOptionsSummary
//    UHZ_OPTIONS_SUMMARY = width=10,exp=6,class=0.91:width=25,exp=4,class=0.85:width=50,exp=2,class=0.78                 

void OptSensor::handleMailSensorOptionsSummary(std::string str) {
  //std::cout<<"handle sensor options"<<std::endl;
  vector<string> svector = parseStringZ(str, ',', "{");
  unsigned int i, vsize = svector.size();
  for(i=0; i<vsize; i++) {
    string param = biteStringX(svector[i], '=');
    string value = svector[i];
    // This needs to be handled by the developer. Just a placeholder.                                                       
    // std::cout<<"sensor: "<<param<<", "<<value<<std::endl;
  }

 
}

