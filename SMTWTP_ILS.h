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
  int seconds;

 public:
  SMTWTP_ILS(int instance_size,
             Init_Mode init,
             Select_Mode select,
             std::vector<Neighbour_Mode> confs,
             int intensity,
             int amplification,
             int seconds):
     SMTWTP_vnd(instance_size, init, select, confs),
     perturbation_intensity(intensity),
     amplification(amplification),
     seconds(seconds)
    {}

  std::vector<long> get_solution(Instance &instance) override;
  std::string get_name() override;
};

#endif
