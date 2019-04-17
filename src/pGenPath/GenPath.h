/************************************************************/
/*    NAME:                                               */
/*    ORGN: MIT                                             */
/*    FILE: GenPath.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef GenPath_HEADER
#define GenPath_HEADER
#include "XYSegList.h"

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include "MOOS/libMOOS/MOOSLib.h"
#include <vector>
#include <iostream>
#include <string>
#include <cmath>


class GenPath : public AppCastingMOOSApp
{
 public:
   GenPath();
   ~GenPath();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();
   void handleMailVisitPoint(std::string sval);
   void handleMailNavX(double dval);
   void handleMailNavY(double dval); 
   void handleMailWayPointStat(std::string sval);
   void handleMailRegenerate(std::string sval);
   void handleMailFinishedSearch(std::string sval);
   void handleMailRegenerateFirstTime(std::string sval);
   void handleMailNodeReportLocal(std::string sval);
   void findRevisitPoints();
   void resetAllLists();
   std::string updateTransitPointsTSM();
   void updateMinimumDistanceToPoint();
protected:
   void registerVariables();

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
   bool m_first_node_report;
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
   std::string m_finished_status;

 private: // State variables
};

#endif 
