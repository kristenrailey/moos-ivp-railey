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
#include <map>


using namespace std;

//---------------------------------------------------------
// Constructor

GenPath::GenPath()
{
  m_all_points_mail = false;
  m_all_points_posted = false;
  m_visit_radius = 0.0;
  m_first_time = true;

  m_regenerate = false;
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
    std::cout<<"receiving new mail"<<std::endl;
    CMOOSMsg &msg = *p;
    string key   = msg.GetKey();
    string sval  = msg.GetString(); 

    if (key == "VISIT_POINT"){
      std::string id_str = tokStringParse(sval, "id", ',', '=');  
      
      if(std::find(m_id_points.begin(), m_id_points.end(), id_str) != m_id_points.end()) { //Check if requested visit point is part of list
	m_all_points_mail=true;
	  }
      else {
	 m_id_points.push_back(id_str);
	 m_visit_points.push_back(sval);

      }
 
    }
    // For calculating distance to goal point
    if (key == "WPT_STAT"){
       std::string dist_str = tokStringParse(sval,"dist",',','=');
       std::string index_str = tokStringParse(sval,"index",',','=');
       std::cout<<"WPTSTATE received, value: "<<sval<<std::endl;
       // Change dist string to double
       double dist_double = 0.0;
       stringstream dd;
       dd<<dist_str;
       dd>>dist_double;
	   
       int wpt_ii=0; //index tracker
       
       //check if id_str is in list already
       std::cout<<"dist_str: "<<dist_str<<"index_str, "<<index_str<<std::endl;
       std::cout<<"have index point: "<< ((std::find(m_index_points.begin(), m_index_points.end(), index_str))== m_index_points.end())<<std::endl;
       
       if(std::find(m_index_points.begin(), m_index_points.end(), index_str) == m_index_points.end()){ //don't have index
	 std::cout<<"don't have index, adding to list"<<std::endl;
	 if (wpt_ii ==0){ // First value	
	   m_dist_to_point.push_back(dist_double);
	   m_index_points.push_back(index_str);
	 }
	 else{ //Tracking new visit point
	   m_dist_to_point.push_back(dist_double); //Add to list of distances
	   m_index_points.push_back(index_str);
	   wpt_ii++;
	 }
       }
       else{ //Update distance for current wpt index
	 std::cout<<"update dist, size: "<<m_dist_to_point.size()<<", index: "<<wpt_ii<<std::endl;
	 m_dist_to_point[wpt_ii] = dist_double;
         }
       
    }

    if (key=="GENPATH_REGENERATE"){
      if (sval =="true"){
	m_regenerate = true;
      }
      else{
	m_regenerate = false;
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
  // Add points to visit list if distance is less than N
    int temp_index=0;
  for (std::vector<double>::iterator k = m_dist_to_point.begin(); k != m_dist_to_point.end(); ++k){
    std::cout<<"loop 1"<<std::endl;
    if (*k>m_visit_radius){ //Miss
      m_revisit_points.push_back(m_points_ordered[temp_index]);
      temp_index++;
    }
    else{ //Hit
      temp_index++;
    }
  }
  
  if (m_regenerate==true){
    //Add revisit points to visit points
    std::cout<<"if statement"<<std::endl;
    m_visit_points.insert(m_visit_points.end(), m_revisit_points.begin(), m_revisit_points.end());

  }
  
  //Imaginary starting point
 double start_x = 0.0;
 double start_y = 0.0;
 double current_x, current_y;
 std::string current_visit_point_str;


 //List of points
 XYSegList my_seglist;
 
 if (m_all_points_mail==true && m_all_points_posted == false){
   std::cout<<"loop 2 "<<m_visit_points.size()<<std::endl;
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
      //   std::string id_str = tokStringParse(*k,"id",',','=');
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
	current_visit_point_str = *k;
      }
    }
    //Add to seglist. Pop off best value.Update start point.

    my_seglist.add_vertex(current_x,current_y);
    std::cout<<"visit points size: "<<m_visit_points.size()<<std::endl;
    m_points_ordered.push_back(current_visit_point_str); //Added this
    std::cout<<"current k value: "<<*current_k<<std::endl;
    m_visit_points.erase(current_k); //Updated for generating path to traverse
    start_x = current_x;
    start_y = current_y;
    
    
   }
   std::cout<<"Sending updates"<<std::endl;
    std::string updates_str = "points = ";
    updates_str +=my_seglist.get_spec();
    Notify("TRANSIT_UPDATES",updates_str);
  
    Notify("SEARCH","true");

    //If first time
    if (m_first_time ==true){
      Notify("FIRST_TIME","true");
      m_first_time = false; //Next time will be revisit points
    }
    else{
      Notify("GENPATH_REGENERATE","true");
    }
    //else
    
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
      
      if(param == "visit_radius") { //Added this
        //handled
	//Convert value to double
       stringstream vr;
       
       vr<<value;
       vr>>m_visit_radius;
      
      }
      else if(param == "BAR") {
        //handled
      }
    }
  }
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
  Register("WPT_STAT",0);
  Register("WPT_INDEX",0);
  Register("GENPATH_REGENERATE",0);
}

