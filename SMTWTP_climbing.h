#ifndef SMTWTP_climbing_define
#define SMTWTP_climbing_define

#include <iostream>
#include <vector>

#include "SMTWTP_simple.h"
#include "Instance.h"

enum class Select_Mode { FIRST, BEST };
enum class Neighbour_Mode { INSERT, SWAP, EXCHANGE };
enum class Init_Mode { RND, EDD, MDD };

class SMTWTP_climbing : public SMTWTP_simple
{
 private:
  Select_Mode select;
  Neighbour_Mode neighbour;
  Init_Mode init;

 public:
  SMTWTP_climbing(int instance_size,
                  Select_Mode select,
                  Neighbour_Mode neighbour,
                  Init_Mode init):
     SMTWTP_simple(instance_size),
     select(select),
     neighbour(neighbour),
     init(init)  
    {}

  std::vector<int> get_solution(Instance &instance);

  std::vector<int> insert_process(std::vector<int> solution,
                                  Instance &instance);
  std::vector<int> swap_process(std::vector<int> solution,
                                Instance &instance);
  std::vector<int> exchange_process(std::vector<int> solution,
                                    Instance &instance);
};

#endif
