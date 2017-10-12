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
 protected:
  Init_Mode init;

 public:
  SMTWTP_initializer(int instance_size,
                     Init_Mode init):
     SMTWTP(instance_size),
     init(init) 
    {}
  
  std::vector<long> get_initializer(Instance &instance);

  std::vector<long> random_solution();
  std::vector<long> EDD_solution(Instance &instance);
  std::vector<long> MDD_solution(Instance &instance);

  std::vector<long> get_solution(Instance &instance) override;
  std::string get_name() override;
};

#endif
