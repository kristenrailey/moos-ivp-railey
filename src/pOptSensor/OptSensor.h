/************************************************************/
/*    NAME: Kristen Railey                                              */
/*    ORGN: MIT                                             */
/*    FILE: OptSensor.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef OptSensor_HEADER
#define OptSensor_HEADER

#include "MOOS/libMOOS/MOOSLib.h"

class OptSensor : public CMOOSApp
{
 public:
   OptSensor();
   ~OptSensor();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected:
   void RegisterVariables();
   //From HazardMgr
   void handleMailSensorOptionsSummary(std::string);
   void handleMailMissionParams(std::string);


 private: // Configuration variables
   double m_penalty_missed_hazard;
   double m_penalty_nonopt_hazard; //?                                                                       
   double m_penalty_false_alarm;
   double m_penalty_max_time;
   double m_max_time;
   double m_penalty_max_time_rate;
   double m_transit_path_width; //?
 private: // State variables
};

#endif 
