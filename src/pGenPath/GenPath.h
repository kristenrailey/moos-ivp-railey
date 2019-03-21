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
   bool m_all_points_mail;
   bool m_all_points_posted;
   double m_visit_radius;
   bool m_regenerate;
   bool m_first_time;
   std::map<char,int> m_points_distances;

 private: // State variables
};

#endif 
