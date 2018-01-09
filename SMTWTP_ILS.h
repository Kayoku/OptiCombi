#ifndef SMTWTP_ILS_define
#define SMTWTP_ILS_define

#include <iostream>
#include <vector>
#include <random>

#include "SMTWTP_vnd.h"
#include "Instance.h"

class SMTWTP_ILS : public SMTWTP_vnd
{
 private:
  int intensity;
  int seconds;
  std::random_device rd;
  std::mt19937 g;
  bool swap;

 public:
  SMTWTP_ILS(int instance_size,
             Init_Mode init,
             Select_Mode select,
             std::vector<Neighbour_Mode> confs,
             int intensity,
             int seconds,
             bool swap):
     SMTWTP_vnd(instance_size, init, select, confs),
     intensity(intensity),
     seconds(seconds),
     rd(),
     g(rd()),
     swap(swap)
    {}

  SMTWTP_ILS(int instance_size,
             Init_Mode init,
             int intensity,
             int seconds,
             bool swap):
     SMTWTP_vnd(instance_size, init, 1),
     intensity(intensity),
     seconds(seconds),
     rd(),
     g(rd()),
     swap(swap)
    {}

  std::vector<long> insert_perturb(std::vector<long> parent);
  std::vector<long> swap_perturb(std::vector<long> parent);
  std::vector<long> get_solution(Instance &instance) override;
  std::string get_name() override;
};

#endif
