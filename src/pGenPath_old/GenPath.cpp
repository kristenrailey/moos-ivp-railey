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
  m_first_time = false;
  m_first_time_regen = false;
  m_finished_search = false;
  m_current_size = 0;
  m_previous_size = 0;
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
       // Change dist string to double
       double dist_double = 0.0;
       int index_int = 0;
       stringstream dd;
       stringstream ii;
       dd<<dist_str;
       ii<<index_str;
       dd>>dist_double;
       ii>>index_int;
	   
       //check if id_str is in list already       
       if(std::find(m_index_points.begin(), m_index_points.end(), index_str) == m_index_points.end()){ 
	 if (index_int ==0){ // First value	
	   m_dist_to_point.push_back(dist_double);
	   m_index_points.push_back(index_str);
	   m_dist_final_val.push_back(0); //Keep track of final values	  
	 }
	 else{ //Tracking new visit point
	   m_dist_to_point.push_back(dist_double); //Add to list of distances
	   m_index_points.push_back(index_str);
	   m_dist_final_val.push_back(0);
	   m_dist_final_val[index_int-1]=1;
	 }
       }
       else{ //Update distance for current wpt index
	 m_dist_to_point[index_int] = dist_double;
         }
       
    }

    if (key=="GENPATH_REGENERATE"){
      if (sval =="true"){
	m_regenerate = true;
	std::cout<<"received regen is true"<<std::endl;
      }
      else{
	m_regenerate = false;
      }
    }

    if (key=="FINISHED_SEARCH"){
      if (sval =="true"){
	m_finished_search = true;
	std::cout<<"received finishedsearch  is true"<<std::endl;
      }
      else{
	m_finished_search = false;
	std::cout<<"recevied finished search is false"<<std::endl;
      }
    }

    
    if (key=="FIRST_TIME_REGEN"){
      if (sval =="true"){
	m_first_time_regen = true;
	std::cout<<"first time regen is true"<<std::endl;
      }
      else{
	m_first_time_regen = false;
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

  if ((m_all_points_mail==true && m_all_points_posted == false)){
    m_first_time = true;
  }

  if (m_first_time==false){   
    if (m_regenerate==true){
      
      // Add points to visit list if distance is less than N
      int temp_index=0;
      m_current_size =m_dist_to_point.size();
      if (m_current_size>m_previous_size){ //Keep track of adding distances
	for (std::vector<double>::iterator k = m_dist_to_point.begin(); k != m_dist_to_point.end(); ++k){	
	  if ((*k>m_visit_radius)&&(m_dist_final_val[temp_index]==1)){ //Miss
	  //Check if already have number
	     std::string id_str = tokStringParse(m_points_ordered[temp_index], "id", ',', '=');     
	     if(std::find(m_id_revisit_points.begin(), m_id_revisit_points.end(), id_str) != m_id_revisit_points.end()) { //Check if requested visit point is part of list
	       temp_index++;
	     } //already have it
	     else{
	       //if unique id., push back
	       m_id_revisit_points.push_back(id_str);
	       m_revisit_points.push_back(m_points_ordered[temp_index]);
	       temp_index++;
	     }
	  }
	  else{ //Hit
	    temp_index++;
	  }
	}
	m_previous_size = m_current_size;
      }
      
      if ((m_revisit_points.size()==0)&&(m_finished_search==true)){ //DONE
	Notify("RETURN","true");
	Notify("SEARCH","false");
	Notify("MISSED_POINTS","false");
       	std::cout<<"Notified to return, finished search is true"<<std::endl;
	m_regenerate = false;
       	return (true);
      }
      else if ((m_revisit_points.size()>0)&&(m_finished_search==true)){ 
	Notify("MISSED_POINTS","true");
	m_visit_points.insert(m_visit_points.end(), m_revisit_points.begin(), m_revisit_points.end());     
	m_revisit_points.clear(); //Clear all lists
	m_dist_to_point.clear();
	m_index_points.clear();
	m_id_revisit_points.clear();
	m_dist_final_val.clear(); 
	m_current_size = 0;
	m_previous_size =0;
	m_finished_search = false;
	Notify("FINISHED_SEARCH","false");
       	std::cout<<"finished a search, visit points"<<std::endl;
      }
      else if((m_revisit_points.size()>0)&&(m_first_time_regen==true)){
	Notify("MISSED_POINTS","true");
	m_visit_points.insert(m_visit_points.end(), m_revisit_points.begin(), m_revisit_points.end());     
	m_revisit_points.clear(); //Clear all lists
	m_dist_to_point.clear();
	m_index_points.clear();
	m_dist_final_val.clear();
	m_id_revisit_points.clear();
       	m_current_size = 0;
	m_previous_size =0;
	m_first_time_regen=false;
       	std::cout<<"revisit points, first time"<<std::endl;
      }
      else if((m_revisit_points.size()==0)&&(m_first_time_regen==true)){ //DONE
	Notify("RETURN","true");
	Notify("SEARCH","false");
	Notify("MISSED_POINTS","false");
       	std::cout<<"Notified to return, no missed points"<<std::endl;
	m_regenerate = false;
       	return (true);	
      }
      else{ //Not finished
	//	continue;
      }
    }
    else{
        return (true);
    }
  }

  if (m_visit_points.size()>0){
    //Imaginary starting point
    double start_x = 0.0;
    double start_y = 0.0;
    double current_x, current_y;
    std::string current_visit_point_str;


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
	  current_visit_point_str = *k;
	}
      }
      //Add to seglist. Pop off best value.Update start point.

      my_seglist.add_vertex(current_x,current_y);
      m_points_ordered.push_back(current_visit_point_str); //Added this
      m_visit_points.erase(current_k); //Updated for generating path to traverse
      start_x = current_x;
      start_y = current_y;   
    }
    std::string updates_str = "points = ";
    updates_str +=my_seglist.get_spec();

    //If first time
    if (m_first_time ==true){
      Notify("FIRST_TIME","true");
      Notify("TRANSIT_UPDATES_FIRST_TIME",updates_str);
      Notify("SEARCH","true");
      std::cout<<"sent notifications"<<std::endl;
      m_all_points_posted = true;
      m_first_time = false; //Next time will be revisit points
    }
    else if (m_regenerate==true){
      std::cout<<"update missed points, regen: "<<updates_str<<std::endl;
      Notify("TRANSIT_UPDATES_MISSED_POINTS",updates_str);
      Notify("MISSED_POINTS","true");
    }
  
  }
  
  else{
    return(true);
  }
  
  return (true); 
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
  Register("FINISHED_SEARCH",0);
  Register("FIRST_TIME_REGEN",0);
}

