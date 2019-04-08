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
    std::cout<<"Message: "<<key<<std::endl;
    if (key == "UHZ_OPTIONS_SUMMARY"){
      std::cout<<"options summary: "<<sval<<std::endl;
      handleMailSensorOptionsSummary(sval);

    if(key == "UHZ_MISSION_PARAMS")
      // std::cout<<"sval: "<<sval<<std::endl;
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
  std::cout<<"handle mail mission param"<<std::endl;
  vector<string> svector = parseStringZ(str, ',', "{");
  unsigned int i, vsize = svector.size();
  for(i=0; i<vsize; i++) {
    string param = biteStringX(svector[i], '=');
    string value = svector[i];
    // This needs to be handled by the developer. Just a placeholder.                            
    std::cout<<"mission params: "<<param<<", "<<value<<std::endl;
  }
}


//----------------------------------------------------
// Procedure: handleMailSensorOptionsSummary
//    UHZ_OPTIONS_SUMMARY = width=10,exp=6,class=0.91:width=25,exp=4,class=0.85:width=50,exp=2,class=0.78                 

void OptSensor::handleMailSensorOptionsSummary(std::string str) {
  std::cout<<"handle sensor options"<<std::endl;
  vector<string> svector = parseStringZ(str, ',', "{");
  unsigned int i, vsize = svector.size();
  for(i=0; i<vsize; i++) {
    string param = biteStringX(svector[i], '=');
    string value = svector[i];
    // This needs to be handled by the developer. Just a placeholder.                                                       
    std::cout<<"sensor: "<<param<<", "<<value<<std::endl;
  }

 
}

