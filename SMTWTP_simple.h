#ifndef SMTWTP_simple_define
#define SMTWTP_simple_define

#include <iostream>
#include <vector>

#include "SMTWTP.h"
#include "Instance.h"

class SMTWTP_simple : public SMTWTP
{
 public:
  SMTWTP_simple(int instance_size):
      SMTWTP(instance_size)
    {}

  std::vector<int> random_solution(); 
  std::vector<int> EDD_solution(Instance &instance);
  std::vector<int> MDD_solution(Instance &instance);
};

#endif
