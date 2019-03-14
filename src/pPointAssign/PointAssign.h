/************************************************************/
/*    NAME: Kristen Railey                                              */
/*    ORGN: MIT                                             */
/*    FILE: PointAssign.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef PointAssign_HEADER
#define PointAssign_HEADER
#include <iostream>
#include <string>
#include "MOOS/libMOOS/MOOSLib.h"
#include <vector>
#include <sstream>

class PointAssign : public CMOOSApp
{
 public:
   PointAssign();
   ~PointAssign();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected:
   void RegisterVariables();
   bool PointRegionIsEast(double x_val);

 private: // Configuration variables
   std::vector<std::string> m_vname_list;
   std::vector<std::string> m_visit_points;
   bool m_assign_by_region;
 private: // State variables
};

#endif 
