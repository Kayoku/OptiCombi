#ifndef SMTWTP_initializer_define
#define SMTWTP_initializer_define

#include <iostream>
#include <vector>

#include "SMTWTP.h"

enum class Select_Mode { FIRST, BEST };
enum class Neighbour_Mode { INSERT, SWAP, EXCHANGE };
enum class Init_Mode { RND, EDD, MDD };

class SMTWTP_initializer: public SMTWTP
{
 private:
  Init_Mode init;

 public:
  SMTWTP_initializer(int instance_size,
                     Init_Mode init):
     SMTWTP(instance_size),
     init(init) 
    {}
  
  std::vector<int> get_initializer(Instance &instance);

  std::vector<int> random_solution();
  std::vector<int> EDD_solution(Instance &instance);
  std::vector<int> MDD_solution(Instance &instance);

  std::vector<int> get_solution(Instance &instance) override;
};

#endif
