/************************************************************/
/*    NAME: Kristen Railey                                              */
/*    ORGN: MIT                                             */
/*    FILE: GenPath.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef GenPath_HEADER
#define GenPath_HEADER

#include "MOOS/libMOOS/MOOSLib.h"
#include <vector>
#include <iostream>
#include <string>
#include <cmath>
#include "XYSegList.h"

class GenPath : public CMOOSApp
{
 public:
   GenPath();
   ~GenPath();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected:
   void RegisterVariables();

 private: // Configuration variables
   std::vector<std::string> m_visit_points;
   std::vector<std::string> m_index_points;
   std::vector<double> m_dist_to_point;
   std::vector<std::string> m_id_points;
   std::vector<std::string> m_points_ordered;
   std::vector<std::string> m_revisit_points;
   std::vector<int> m_dist_final_val;
   std::vector<std::string> m_id_revisit_points;
   std::vector<double> m_nav_x;
   std::vector<double> m_nav_y;
   bool m_all_points_mail;
   bool m_all_points_posted;
   double m_visit_radius;
   bool m_regenerate;
   bool m_first_time;
   bool m_revisit_points_add;
   std::map<char,int> m_points_distances;
   bool m_finished_search;
   bool m_first_time_regen;
   int m_current_size;
   int m_previous_size;
   int m_first_dist_to_point;
   double m_old_x;
   double m_old_y;
   bool m_first_nav_x;
   bool m_first_nav_y;
   bool m_collect_nav;
 private: // State variables
};

#endif 
