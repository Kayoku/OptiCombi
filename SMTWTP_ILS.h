#ifndef SMTWTP_ILS_define
#define SMTWTP_ILS_define

#include <iostream>
#include <vector>

#include "SMTWTP_vnd.h"
#include "Instance.h"

class SMTWTP_ILS : public SMTWTP_vnd
{
 private:
  int perturbation_intensity;
  int amplification;

 public:
  SMTWTP_ILS(int instance_size,
             Init_Mode init,
             std::vector<Config_VND> confs,
             int intensity,
             int amplification):
     SMTWTP_vnd(instance_size, init, confs),
     perturbation_intensity(intensity),
     amplification(amplification)
    {}

  std::vector<long> get_solution(Instance &instance) override;
  std::string get_name() override;
};

#endif
