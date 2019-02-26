// File: PrimeEntry.h

#include <string> 
#include <vector> 
#include <cmath>
#include <iostream>
#include <algorithm>
#include <sstream>
#include "MOOS/libMOOS/Utils/MOOSUtils.h"

#ifndef PRIME_ENTRY_HEADER
#define PRIME_ENTRY_HEADER

class PrimeEntry
 {
 public:

   PrimeEntry();
   ~PrimeEntry() {};

   void setOriginalVal(unsigned long int v); //Set original value
   void setReceivedIndex(unsigned int v);
   void setCalculatedIndex(unsigned int v);
   void setDone(bool v);
   bool   done();
   long unsigned int iter_calc();
   void   factor(unsigned long int max_steps); //Factoring function for a certain number of steps
   std::string  getReport(); //String to print out report

 protected:
   unsigned long int m_N; //Updated number to factor
   unsigned long int m_orig; //Original prime number
   bool m_done; //True or false if done with factoring
   unsigned long int  m_received_index;
   unsigned long int  m_calculated_index;
   unsigned long int  m_finished_iter;
   unsigned long int  m_maxiter;
   unsigned long int m_ii;
   unsigned long int m_k_start;
   unsigned long int m_k;
   double m_start_time, m_finish_time, m_solve_time;
   std::vector<unsigned long int> m_factors; //Vector of all the factors so far
 };

#endif
