/************************************************************/
/*    NAME: Kristen Railey                                              */
/*    ORGN: MIT                                             */
/*    FILE: GenPath.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "GenPath.h"
#include "XYObject.h"
#include "ColorPack.h"

using namespace std;

//---------------------------------------------------------
// Constructor

GenPath::GenPath()
{
}

//---------------------------------------------------------
// Destructor

GenPath::~GenPath()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool GenPath::OnNewMail(MOOSMSG_LIST &NewMail)
{
  MOOSMSG_LIST::iterator p;
   
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key   = msg.GetKey();
    string sval  = msg.GetString(); 

    if (key == "VISIT_POINT"){
      m_visit_points.push_back(sval);
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

bool GenPath::OnConnectToServer()
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

bool GenPath::Iterate()
{
  //Imaginary starting point
 double start_x = 0.0;
 double start_y = 0.0;
 double current_x, current_y;

 //List of points
 XYSegList my_seglist;
 
 while (m_visit_points.size()>0){
    current_x=start_x;
    current_y=start_y;
    std::vector<std::string>::iterator current_k;
    double current_min=10000.0;
    double temp_dist = 0.0;
        
    //Iterate through points 
    for (std::vector<std::string>::iterator k = m_visit_points.begin(); k != m_visit_points.end(); ++k){
      //Get x, y positions from string
      std::string x_str = tokStringParse(*k, "x", ',', '=');
      std::string y_str = tokStringParse(*k, "y", ',', '=');
      double x_double = 0.0;
      double y_double = 0.0;
      stringstream rr;
      stringstream ww;
      rr<<x_str;
      ww<<y_str;
      rr>>x_double;
      ww>>y_double;
      //Calculate distance
      temp_dist=sqrt(pow((x_double-start_x),2)+pow((y_double-start_y),2));
      if (temp_dist<current_min){  // Check if less than current min, update
	current_min=temp_dist;
	current_x=x_double;
	current_y=y_double;
	current_k=k;
      }
    };
    //Add to seglist. Pop off best value.Update start point. 
    my_seglist.add_vertex(current_x,current_y);
    m_visit_points.erase(current_k);
    start_x = current_x;
    start_y = current_y;
    
    
  }
  std::string updates_str = "points = ";
  updates_str +=my_seglist.get_spec();
  Notify("UPDATES_VAR",updates_str);
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool GenPath::OnStartUp()
{
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

void GenPath::RegisterVariables()
{
  // Register("FOOBAR", 0);
  Register("VISIT_POINT",0);
  Register("NAV_X",0);
  Register("NAV_Y",0);
}

