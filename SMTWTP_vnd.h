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
 protected:
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

  std::vector<long> do_vnd(Instance &instance, std::vector<long> init_sol);
  std::vector<long> get_solution(Instance &instance) override;
  std::string get_name() override;
};

#endif
