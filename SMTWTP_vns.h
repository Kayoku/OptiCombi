#ifndef SMTWTP_vns_define
#define SMTWTP_vns_define

#include <iostream>
#include <vector>

#include "SMTWTP_initializer.h"
#include "SMTWTP_climbing.h"
#include "Instance.h"

class SMTWTP_vns : public SMTWTP_initializer
{
 protected:
  std::vector<SMTWTP_climbing> vns;
  int max_no_evolution;

 public:
  SMTWTP_vns(int instance_size,
             Init_Mode init,
             int max_no_evolution):
     SMTWTP_initializer(instance_size, init),
     max_no_evolution(max_no_evolution)
    {
     std::vector<Select_Mode> select = {Select_Mode::FIRST,
                                        Select_Mode::BEST};
     std::vector<Neighbour_Mode> neighbour = {Neighbour_Mode::INSERT,
                                              Neighbour_Mode::SWAP,
                                              Neighbour_Mode::EXCHANGE};
     for (auto s: select)
      for (auto n: neighbour)
       vns.push_back(SMTWTP_climbing(instance_size, s, n, init));
    }

  std::vector<long> get_solution(Instance &instance) override;
  std::string get_name() override;
};

#endif
