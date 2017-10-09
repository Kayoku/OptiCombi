#ifndef SMTWTP_vnd_define
#define SMTWTP_vnd_define

#include <iostream>
#include <vector>

#include "SMTWTP_initializer.h"
#include "SMTWTP_climbing.h"
#include "Instance.h"

struct Config_VND
{
 Select_Mode select;
 Neighbour_Mode neighbour;

 Config_VND(Select_Mode select,
            Neighbour_Mode neighbour):
  select(select),
  neighbour(neighbour)
 {}
};

class SMTWTP_vnd : public SMTWTP_initializer
{
 private:
  std::vector<SMTWTP_climbing> vnd;

 public:
  SMTWTP_vnd(int instance_size,
             Init_Mode init,
             std::vector<Config_VND> confs):
     SMTWTP_initializer(instance_size, init)
    {
     for(auto conf : confs)
      vnd.push_back(SMTWTP_climbing(instance_size, conf.select, conf.neighbour, init));
    }

  std::vector<int> get_solution(Instance &instance) override;

  int get_full_compute_cpt();
};

#endif
