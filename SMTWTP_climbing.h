#ifndef SMTWTP_climbing_define
#define SMTWTP_climbing_define

#include <iostream>
#include <vector>

#include "SMTWTP_initializer.h"
#include "Instance.h"

class SMTWTP_climbing : public SMTWTP_initializer
{
 private:
  Select_Mode select;
  Neighbour_Mode neighbour;

  std::vector<long> insert_process(std::vector<long> solution,
                                  Instance &instance);
  std::vector<long> swap_process(std::vector<long> solution,
                                Instance &instance);
  std::vector<long> exchange_process(std::vector<long> solution,
                                    Instance &instance);

 public:
  SMTWTP_climbing(int instance_size,
                  Select_Mode select,
                  Neighbour_Mode neighbour,
                  Init_Mode init):
     SMTWTP_initializer(instance_size, init),
     select(select),
     neighbour(neighbour)
    {}

  std::vector<long> get_process(std::vector<long> init_solution,
                               Instance &instance);
  std::vector<long> get_solution(Instance &instance) override;
};

#endif
