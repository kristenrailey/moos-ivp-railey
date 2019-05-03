/************************************************************/
/*    NAME: appcasting                                              */
/*    ORGN: MIT                                             */
/*    FILE: VehicleDataExchange.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef VehicleDataExchange_HEADER
#define VehicleDataExchange_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <string>
class VehicleDataExchange : public AppCastingMOOSApp
{
 public:
   VehicleDataExchange();
   ~VehicleDataExchange();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();

 private: // Configuration variables
   std::string m_vname;
   std::string m_dest_name;
 private: // State variables
};

#endif 
