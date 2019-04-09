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
  m_search_config_received = false;
  m_update_lawnmower = false;
  m_sensor_options_received = false;
  m_name_received = false;
  // ADD THIS LATER  m_buffer =1000.0;
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
   
      //    std::cout<<"message: "<<key<<"value: "<<sval<<std::endl;
   
    if (key == "UHZ_OPTIONS_SUMMARY"){
      //      std::cout<<"options summary: "<<sval<<std::endl;
      handleMailSensorOptionsSummary(sval);
    }
    if(key == "UHZ_MISSION_PARAMS"){
      //std::cout<<"mission params: "<<sval<<std::endl;
      handleMailMissionParams(sval);

    }

    if (key == "PHI_HOST_INFO"){ 
      //PHI_HOST_INFO                               "community=jake,hostip=18.21.140.34,port_db=9001,pshare_iroutes=18.21.140.34:9301,hostip_alts=,timewarp=10"
      
      m_vname = tokStringParse(sval, "community", ',', '=');
     
      m_name_received = true;
      
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
  // std::cout<<"status: "<<m_name_received<<","<<m_search_config_received<<","<<m_update_lawnmower<<std::endl;
  if ((m_name_received==true)&&(m_search_config_received == true)&&(m_update_lawnmower ==false)){
    std::cout<<"********************* SENSOR WIDTH*****"<<std::endl;
    //    msensor_width.push_back(temp_width);

    std::cout<<m_sensor_width[0]<<", "<<m_sensor_width[1]<<","<<m_sensor_width[2]<<","<<m_sensor_width[3]<<std::endl;
      //calcSearchTime(sensor_width,search_area_width,search_area_height);
    int best_sensor_width_index;
    double sa_height = m_search_reg_y_max-m_search_reg_y_min;
    double sa_width = (m_search_reg_x_max-m_search_reg_x_min)/2.0; //Areas are split between the two robots
    std::cout<<"sa width: "<<sa_width<<", sa height: "<<sa_height<<std::endl;
    double best_time = 0.0;
    for (int i = 0; i<m_sensor_width.size(); i++){
      double current_time = calcSearchTime(m_sensor_width[i],sa_width,sa_height);
      std::cout<<"current time: "<<current_time<<std::endl;
      if ((current_time<m_max_time)&&(current_time>best_time)){
	  //New best
	  best_time = current_time;
	  best_sensor_width_index = i;
      }
    }  
      std::cout<<"updated lawnmower, best sensor width: "<< m_sensor_width[best_sensor_width_index]<<std::endl;
      //lawnmower str:  points = format=lawnmower, label=foxtrot, x=0, y=40, height=60, width=180,lane_width=15, rows=north-south, startx=20, starty=-300, degs=45
      ostringstream os;
      //      double lane_width = m_sensor_width[best_sensor_width_index]/2.0;
      double lane_width = m_sensor_width[best_sensor_width_index]; //FACTOR OF TWO 

      double y_center = m_search_reg_y_min +(m_search_reg_y_max-m_search_reg_y_min)/2.0; 

      //Assuming split btw two robots
      double x_center;
      if (m_vname =="jake"){
	x_center =m_search_reg_x_min +(m_search_reg_x_max-m_search_reg_x_min)/4.0;
      }
      else{ //other vehicle
        x_center =m_search_reg_x_max -(m_search_reg_x_max-m_search_reg_x_min)/4.0;
      }
      os << "points = format=lawnmower, label="<<m_vname<<"search, x=" <<x_center<<",y="<< y_center<<",height="<<sa_height<<",width="<<sa_width<<",lane_width="<<lane_width<<",rows=ns,startx="<<m_search_reg_x_min<<",starty="<<m_search_reg_y_max;
      string lawnmower_str = os.str();
      std::cout<<"lawnmower update: "<<lawnmower_str<<std::endl;
      Notify("LAWNMOWER_UPDATES", lawnmower_str);
      
      ostringstream os_config;
      // UHZ_CONFIG_REQUEST                   "vname=jake,width=50,pd=0.7"
      //TO DO : Add vname
      // Pd function , this is the first time so very large
      os_config<<"vname="<<m_vname<<",width="<<m_sensor_width[best_sensor_width_index]<<",pd=0.9";
      string uhz_config_request_str = os_config.str();
      Notify("UHZ_CONFIG_REQUEST",uhz_config_request_str);

      m_update_lawnmower = true;
  };

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
  Register("PHI_HOST_INFO",0);
  
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
  //std::cout<<"handle mail mission param: "<<str<<std::endl;
  vector<string> svector = parseStringZ(str, ',', "{");
  unsigned int i, vsize = svector.size();
  for(i=0; i<vsize; i++) {
    string param = biteStringX(svector[i], '=');
    string value = svector[i];
    // This needs to be handled by the developer. Just a placeholder.                            
    //std::cout<<"mission params: "<<param<<", "<<value<<std::endl;
    stringstream ss;
    ss<<value;
    if (i == 0){
      ss>>m_penalty_missed_hazard;
      //std::cout<<"missed hazard:"<<m_penalty_missed_hazard<<std::endl;
    }
    else if (i==1){
      ss>>m_penalty_nonopt_hazard;
    }
    else if (i==2){
      ss>>m_penalty_false_alarm;
    }
    else if (i==3){
      ss>>m_penalty_max_time_over;
      //std::cout<<"max time over: "<<m_penalty_max_time_over<<std::endl;
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
      //std::cout<<"search region"<< value<<std::endl;
      string pts_str = tokStringParse(value, "search_region = pts=", ',', '=');
      string search_region_str_left = biteStringX(value, '{');
      //std::cout<<"new string: "<<pts_str<<std::endl;
      string final_pts_str = biteStringX(value,'}');
      //std::cout<<"final string: "<<final_pts_str<<std::endl;
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
	//std::cout<<"temp x: "<<temp_x<<", temp y: "<<temp_y<<std::endl;
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
      //std::cout<<"min x: "<<m_search_reg_x_min<<" max x: "<<m_search_reg_x_max<<std::endl;
      //std::cout<<"min y: "<<m_search_reg_y_min<<" max y: "<<m_search_reg_y_max<<std::endl;
      m_search_config_received = true;
     
    
    
    
    

    }
  }
}


//----------------------------------------------------
// Procedure: handleMailSensorOptionsSummary
//    UHZ_OPTIONS_SUMMARY = width=10,exp=6,class=0.91:width=25,exp=4,class=0.85:width=50,exp=2,class=0.78                 

void OptSensor::handleMailSensorOptionsSummary(std::string str) {
  //std::cout<<"handle sensor options"<<std::endl;
  if   (m_sensor_options_received == false){
    //Get rid of ':'
    vector<string> str_vector = parseString(str, ':');
    for(unsigned int i=0; i<str_vector.size(); i++)
      {//cout << "sensor options: [" << str_vector[i] << "]" << endl;
      }

    unsigned int i, vsize = str_vector.size();
    for(i=0; i<vsize; i++) {
      //std::cout<<"current string: "<<str_vector[i]<<std::endl;
      string param = biteStringX(str_vector[i], '=');
      string value = str_vector[i];
      // This needs to be handled by the developer. Just a placeholder.                                                       
      //std::cout<<"sensor: "<<param<<", "<<value<<std::endl;
      if (param=="width"){
	stringstream ww;
	double temp_width =0.0;
	ww<<value;
	ww>>temp_width;
	m_sensor_width.push_back(temp_width);
      }
    }
  m_sensor_options_received = true;
  }
 
}

//--------------------------
// Calc search area







double OptSensor::calcSearchTime(double sensor_width,double search_area_width,double search_area_height){
  // double lane_width = sensor_width/2.0;
  double lane_width = sensor_width; //Factor of 2.0 from swath width disagreement                                                                                                                                                                        

  double num_lanes = search_area_width/lane_width;
    double total_dist = search_area_height*(num_lanes+1)+search_area_width;
    std::cout<<"lane width: "<<lane_width<<"num of lanes: "<<num_lanes<<"total dist: "<<total_dist<<std::endl;
  return (total_dist/2.0);
}
