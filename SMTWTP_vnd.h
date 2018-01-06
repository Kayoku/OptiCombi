#ifndef SMTWTP_vnd_define
#define SMTWTP_vnd_define

#include <iostream>
#include <vector>

#include "SMTWTP_initializer.h"
#include "SMTWTP_climbing.h"
#include "Instance.h"

class SMTWTP_vnd : public SMTWTP_initializer
{
 protected:
  std::vector<SMTWTP_climbing> vnd;
  bool random;
  int depth;

 public:
  SMTWTP_vnd(int instance_size,
             Init_Mode init,
             int depth):
   SMTWTP_initializer(instance_size, init),
   random(true),
   depth(depth)
   {
    vnd.push_back(SMTWTP_climbing(instance_size, Select_Mode::FIRST, Neighbour_Mode::EXCHANGE, init, depth));
    vnd.push_back(SMTWTP_climbing(instance_size, Select_Mode::FIRST, Neighbour_Mode::INSERT, init, depth));
    vnd.push_back(SMTWTP_climbing(instance_size, Select_Mode::FIRST, Neighbour_Mode::SWAP, init, depth));
    vnd.push_back(SMTWTP_climbing(instance_size, Select_Mode::BEST, Neighbour_Mode::SWAP, init, depth));
    vnd.push_back(SMTWTP_climbing(instance_size, Select_Mode::BEST, Neighbour_Mode::INSERT, init, depth));
    vnd.push_back(SMTWTP_climbing(instance_size, Select_Mode::BEST, Neighbour_Mode::EXCHANGE, init, depth));
   }

  SMTWTP_vnd(int instance_size,
             Init_Mode init,
             Select_Mode select,
             std::vector<Neighbour_Mode> neighbours):
     SMTWTP_initializer(instance_size, init),
     random(false),
     depth(1)
    {
     for(auto neighbour : neighbours)
      vnd.push_back(SMTWTP_climbing(instance_size, select, neighbour, init));
    }

  std::vector<long> do_vnd(Instance &instance, std::vector<long> init_sol);
  std::vector<long> get_solution(Instance &instance) override;
  std::string get_name() override;
};

#endif
