#ifndef SMTWTP_population_define
#define SMTWTP_population_define

#include <iostream>
#include <vector>

#include "SMTWTP_initializer.h"
#include "Instance.h"

class SMTWTP_population : public SMTWTP_initializer
{
 private:
  std::vector<std::pair<std::vector<long>, float>> populations;

 public:
  SMTWTP_population(int instance_size):
     SMTWTP_initializer(instance_size, Init_Mode::RND)
   {}

  std::vector<long> get_solution(Instance &instance) override;
  std::string get_name() override;
};

#endif
