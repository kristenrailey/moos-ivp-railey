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
#include <algorithm>


using namespace std;

//---------------------------------------------------------
// Constructor

GenPath::GenPath()
{
  m_all_points_mail = false;
  m_all_points_posted = false;

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
      std::string id_str = tokStringParse(sval, "id", ',', '=');
    
      
      if(std::find(m_id_points.begin(), m_id_points.end(), id_str) != m_id_points.end()) {
	/* v contains x */
	std::cout<<"already have this point"<<std::endl;
	m_all_points_mail=true;
	  }
      else {
	/* v does not contain x */
	 m_id_points.push_back(id_str);

	 m_visit_points.push_back(sval);
	 std::cout<<"added new point: "<<id_str<<std::endl;

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
  std::cout<<"starting iterate loop"<<std::endl;
  //Imaginary starting point
 double start_x = 0.0;
 double start_y = 0.0;
 double current_x, current_y;

 //List of points
 XYSegList my_seglist;
 
 // while (m_visit_points.size()>0){
 std::cout<<"mail rec: "<<m_all_points_mail<<"all posted: "<<m_all_points_posted<<std::endl;
 if (m_all_points_mail==true && m_all_points_posted == false){
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
    }
    //Add to seglist. Pop off best value.Update start point.
    std::cout<<"Current visit point size: "<<m_visit_points.size()<<std::endl;
    std::cout<<"current min: "<<current_min<<std::endl;
    my_seglist.add_vertex(current_x,current_y);
    m_visit_points.erase(current_k);
    start_x = current_x;
    start_y = current_y;
    
   }
    std::string updates_str = "points = ";
    updates_str +=my_seglist.get_spec();
    std::cout<<updates_str<<std::endl;
    Notify("TRANSIT_UPDATES",updates_str);
  
    Notify("SEARCH","true");
    m_all_points_posted = true;
  
 }

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
  std::cout<<"notified uts pause gen path"<<std::endl;
  Notify("UTS_PAUSE_GENPATH","false");
  
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

