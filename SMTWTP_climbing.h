#ifndef SMTWTP_climbing_define
#define SMTWTP_climbing_define

#include <iostream>
#include <vector>
#include <algorithm>

#include "SMTWTP_initializer.h"
#include "Instance.h"

class SMTWTP_climbing : public SMTWTP_initializer
{
 private:
  Select_Mode select;
  Neighbour_Mode neighbour;
  int depth;
  std::vector<long> best_solution;
  long best_cost;

  // Insert
  std::vector<long> insert_process(std::vector<long> solution,
                                   Instance &instance);
  bool insert_branch(std::vector<long> new_sol,
                     Instance &inst,
                     std::vector<int> indexs);
  bool insert_all(std::vector<long> new_sol,
                  Instance &inst,
                  long erased_value);

  // Swap
  std::vector<long> swap_process(std::vector<long> solution,
                                 Instance &instance);
  bool swap_branch(std::vector<long> new_sol,
                   Instance &inst,
                   std::vector<int> indexs);
  bool swap_all(std::vector<long> new_sol,
                Instance &inst,
                long offset);

  // Exchange
  std::vector<long> exchange_process(std::vector<long> solution,
                                     Instance &instance);
  bool exchange_branch(std::vector<long> new_sol,
                       Instance &inst,
                       std::vector<int> indexs);
  bool exchange_all(std::vector<long> new_sol,
                    Instance &inst,
                    long offset);

 public:
  SMTWTP_climbing(int instance_size,
                  Select_Mode select,
                  Neighbour_Mode neighbour,
                  Init_Mode init,
                  int depth=1):
     SMTWTP_initializer(instance_size, init),
     select(select),
     neighbour(neighbour),
     depth(depth),
     best_cost(100000000)
    {}

  std::vector<long> get_process(std::vector<long> init_solution,
                               Instance &instance);

  std::vector<long> get_solution(Instance &instance) override;
  std::string get_name() override;
  std::string get_spec();
};

#endif
